/*****************************************************************************
 * demux.c: demuxer using ffmpeg (libavformat).
 *****************************************************************************
 * Copyright (C) 2004-2009 the VideoLAN team
 * $Id: e20d2495f023000e90b10fef2ee3ee2b35755901 $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Gildas Bazin <gbazin@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_demux.h>
#include <vlc_stream.h>
#include <vlc_meta.h>
#include <vlc_input.h>
#include <vlc_charset.h>
#include <vlc_avcodec.h>
#include <vlc_plugin.h>

#include <libavformat/avformat.h>

#include "../codec/avcodec/avcodec.h"
#include "../codec/avcodec/chroma.h"
#include "../demux/avformat/avformat.h"
#include "../demux/xiph.h"
#include "../demux/vobsub.h"

//#define AVFORMAT_DEBUG 1

/* Version checking */
#if defined(HAVE_FFMPEG_AVFORMAT_H) || defined(HAVE_LIBAVFORMAT_AVFORMAT_H)

#if (LIBAVCODEC_VERSION_INT >= ((51<<16)+(50<<8)+0) )
#   define HAVE_FFMPEG_CODEC_ATTACHMENT 1
#endif

vlc_module_begin ()
#endif /* MERGE_FFMPEG */
    add_shortcut( "ffmpeg", "avformat" )
    set_category( CAT_INPUT )
    set_subcategory( SUBCAT_INPUT_ACCESS )
    set_description( N_("FFmpeg access demuxer" ) )
    set_shortname( N_("Avformat") )
    set_capability( "access_demux", -1 )
    set_callbacks( OpenDemux, CloseDemux )
#ifndef MERGE_FFMPEG
vlc_module_end ()
#endif

/*****************************************************************************
 * demux_sys_t: demux descriptor
 *****************************************************************************/
struct demux_sys_t
{
    AVInputFormat  *fmt;
    AVFormatContext *ic;

    int             i_tk;
    es_out_id_t     **tk;

    int64_t     i_pcr;
    int64_t     i_pcr_inc;
    int         i_pcr_tk;

    unsigned    i_ssa_order;

    int                i_attachments;
    input_attachment_t **attachments;

    /* Only one title with seekpoints possible atm. */
    input_title_t *p_title;
};

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int Demux  ( demux_t *p_demux );
static int Control( demux_t *p_demux, int i_query, va_list args );

static block_t *BuildSsaFrame( const AVPacket *p_pkt, unsigned i_order );
static void UpdateSeekPoint( demux_t *p_demux, int64_t i_time );

/*****************************************************************************
 * Open
 *****************************************************************************/
int OpenDemux( vlc_object_t *p_this )
{
    demux_t       *p_demux = (demux_t*)p_this;
    demux_sys_t   *p_sys;
    AVFormatContext *ic;
    AVInputFormat *fmt;
    unsigned int  i;
    int64_t       i_start_time = -1;
    bool          b_can_seek;
    char         *psz_url;

    if( p_demux->psz_file )
        psz_url = strdup( p_demux->psz_file );
    else
    {
        if ( !strcmp( p_demux->psz_access, "avformat"))
            psz_url = strdup ( p_demux->psz_location );
        else
            if (asprintf( &psz_url, "%s://%s", p_demux->psz_access, p_demux->psz_location ) < 0 )
                return VLC_ENOMEM;
    }

    if (psz_url == NULL)
        return VLC_ENOMEM;

    msg_Dbg( p_demux, "trying url: %s", psz_url);

    vlc_avcodec_lock();
    av_register_all(); /* Can be called several times */
    vlc_avcodec_unlock();

    ic = avformat_alloc_context();
    if ( !ic )
    {
        free( psz_url );
        return VLC_ENOMEM;
    }

    int err = av_open_input_file( &ic, psz_url, NULL, 0, NULL );
    if ( err < 0 )
    {
        msg_Dbg ( p_demux, "av_open_input_file failed to open %s", psz_url );
        free( ic );
        free( psz_url );
        return VLC_EGENERIC;
    }

    fmt = ic->iformat;

    /* Don't try to handle MPEG unless forced */
    if( !p_demux->b_force &&
        ( !strcmp( fmt->name, "mpeg" ) ||
          !strcmp( fmt->name, "vcd" ) ||
          !strcmp( fmt->name, "vob" ) ||
          !strcmp( fmt->name, "mpegts" ) ||
          /* libavformat's redirector won't work */
          !strcmp( fmt->name, "redir" ) ||
          !strcmp( fmt->name, "sdp" ) ) )
    {
        avformat_free_context( ic );
        free( psz_url );
        return VLC_EGENERIC;
    }

    /* Don't trigger false alarms on bin files */
    if( !p_demux->b_force && !strcmp( fmt->name, "psxstr" ) )
    {
        int i_len;

        if( !p_demux->psz_file )
        {
            avformat_free_context( ic );
            free( psz_url );
            return VLC_EGENERIC;
        }

        i_len = strlen( p_demux->psz_file );
        if( i_len < 4 )
        {
            avformat_free_context( ic );
            free( psz_url );
            return VLC_EGENERIC;
        }

        if( strcasecmp( &p_demux->psz_file[i_len - 4], ".str" ) &&
            strcasecmp( &p_demux->psz_file[i_len - 4], ".xai" ) &&
            strcasecmp( &p_demux->psz_file[i_len - 3], ".xa" ) )
        {
            avformat_free_context( ic );
            free( psz_url );
            return VLC_EGENERIC;
        }
    }

    msg_Dbg( p_demux, "detected format: %s", fmt->name );

    free( psz_url );
    psz_url = NULL;

    /* Fill p_demux fields */
    p_demux->pf_demux = Demux;
    p_demux->pf_control = Control;
    p_demux->p_sys = p_sys = malloc( sizeof( demux_sys_t ) );
    p_sys->ic = ic;
    p_sys->fmt = fmt;
    p_sys->i_tk = 0;
    p_sys->tk = NULL;
    p_sys->i_pcr_tk = -1;
    p_sys->i_pcr = -1;
    p_sys->i_ssa_order = 0;
    TAB_INIT( p_sys->i_attachments, p_sys->attachments);
    p_sys->p_title = NULL;

    vlc_avcodec_lock(); /* avformat calls avcodec behind our back!!! */
    if( av_find_stream_info( p_sys->ic ) < 0 )
    {
        msg_Warn( p_demux, "av_find_stream_info failed" );
    }
    vlc_avcodec_unlock();

    for( i = 0; i < p_sys->ic->nb_streams; i++ )
    {
        AVStream *s = p_sys->ic->streams[i];
        AVCodecContext *cc = s->codec;

        es_out_id_t  *es;
        es_format_t  fmt;
        vlc_fourcc_t fcc;
        const char *psz_type = "unknown";

        if( !GetVlcFourcc( cc->codec_id, NULL, &fcc, NULL ) )
            fcc = VLC_FOURCC( 'u', 'n', 'd', 'f' );

        switch( cc->codec_type )
        {
        case AVMEDIA_TYPE_AUDIO:
            es_format_Init( &fmt, AUDIO_ES, fcc );
            fmt.i_bitrate = cc->bit_rate;
            fmt.audio.i_channels = cc->channels;
            fmt.audio.i_rate = cc->sample_rate;
            fmt.audio.i_bitspersample = cc->bits_per_coded_sample;
            fmt.audio.i_blockalign = cc->block_align;
            psz_type = "audio";
            break;

        case AVMEDIA_TYPE_VIDEO:
            es_format_Init( &fmt, VIDEO_ES, fcc );

            /* Special case for raw video data */
            if( cc->codec_id == CODEC_ID_RAWVIDEO )
            {
                msg_Dbg( p_demux, "raw video, pixel format: %i", cc->pix_fmt );
                if( GetVlcChroma( &fmt.video, cc->pix_fmt ) != VLC_SUCCESS)
                {
                    msg_Err( p_demux, "was unable to find a FourCC match for raw video" );
                }
                else
                    fmt.i_codec = fmt.video.i_chroma;
            }
            /* We need this for the h264 packetizer */
            else if( cc->codec_id == CODEC_ID_H264 && ( !strcmp( p_sys->fmt->name, "flv" ) ||
                !strcmp( p_sys->fmt->name, "matroska" ) || !strcmp( p_sys->fmt->name, "mp4" ) ) )
                fmt.i_original_fourcc = VLC_FOURCC( 'a', 'v', 'c', '1' );

            fmt.video.i_width = cc->width;
            fmt.video.i_height = cc->height;
            if( cc->palctrl )
            {
                fmt.video.p_palette = malloc( sizeof(video_palette_t) );
                *fmt.video.p_palette = *(video_palette_t *)cc->palctrl;
            }
            psz_type = "video";
            fmt.video.i_frame_rate = cc->time_base.den;
            fmt.video.i_frame_rate_base = cc->time_base.num * __MAX( cc->ticks_per_frame, 1 );
            break;

        case AVMEDIA_TYPE_SUBTITLE:
            es_format_Init( &fmt, SPU_ES, fcc );
            if( strncmp( p_sys->ic->iformat->name, "matroska", 8 ) == 0 &&
                cc->codec_id == CODEC_ID_DVD_SUBTITLE &&
                cc->extradata != NULL &&
                cc->extradata_size > 0 )
            {
                char *psz_start;
                char *psz_buf = malloc( cc->extradata_size + 1);
                if( psz_buf != NULL )
                {
                    memcpy( psz_buf, cc->extradata , cc->extradata_size );
                    psz_buf[cc->extradata_size] = '\0';

                    psz_start = strstr( psz_buf, "size:" );
                    if( psz_start &&
                        vobsub_size_parse( psz_start,
                                           &fmt.subs.spu.i_original_frame_width,
                                           &fmt.subs.spu.i_original_frame_height ) == VLC_SUCCESS )
                    {
                        msg_Dbg( p_demux, "original frame size: %dx%d",
                                 fmt.subs.spu.i_original_frame_width,
                                 fmt.subs.spu.i_original_frame_height );
                    }
                    else
                    {
                        msg_Warn( p_demux, "reading original frame size failed" );
                    }

                    psz_start = strstr( psz_buf, "palette:" );
                    if( psz_start &&
                        vobsub_palette_parse( psz_start, &fmt.subs.spu.palette[1] ) == VLC_SUCCESS )
                    {
                        fmt.subs.spu.palette[0] =  0xBeef;
                        msg_Dbg( p_demux, "vobsub palette read" );
                    }
                    else
                    {
                        msg_Warn( p_demux, "reading original palette failed" );
                    }
                    free( psz_buf );
                }
            }

            psz_type = "subtitle";
            break;

        default:
            es_format_Init( &fmt, UNKNOWN_ES, 0 );
#ifdef HAVE_FFMPEG_CODEC_ATTACHMENT
            if( cc->codec_type == AVMEDIA_TYPE_ATTACHMENT )
            {
                input_attachment_t *p_attachment;

                psz_type = "attachment";
                if( cc->codec_id == CODEC_ID_TTF )
                {
                    AVMetadataTag *filename = av_metadata_get( s->metadata, "filename", NULL, 0 );
                    if( filename && filename->value )
                    {
                        p_attachment = vlc_input_attachment_New(
                                filename->value, "application/x-truetype-font",
                                NULL, cc->extradata, (int)cc->extradata_size );
                        TAB_APPEND( p_sys->i_attachments, p_sys->attachments,
                                p_attachment );
                    }
                }
                else msg_Warn( p_demux, "unsupported attachment type (%u) in ffmpeg demux", cc->codec_id );
            }
            break;
#endif

            if( cc->codec_type == AVMEDIA_TYPE_DATA )
                psz_type = "data";

            msg_Warn( p_demux, "unsupported track type in ffmpeg demux" );
            break;
        }

        AVMetadataTag *language = av_metadata_get( s->metadata, "language", NULL, 0 );
        if ( language && language->value )
            fmt.psz_language = strdup( language->value );

        if( s->disposition & AV_DISPOSITION_DEFAULT )
            fmt.i_priority = 1000;

#ifdef HAVE_FFMPEG_CODEC_ATTACHMENT
        if( cc->codec_type != AVMEDIA_TYPE_ATTACHMENT )
#endif
        {
            const bool    b_ogg = !strcmp( p_sys->fmt->name, "ogg" );
            const uint8_t *p_extra = cc->extradata;
            unsigned      i_extra  = cc->extradata_size;

            if( cc->codec_id == CODEC_ID_THEORA && b_ogg )
            {
                unsigned pi_size[3];
                const void *pp_data[3];
                unsigned i_count;
                for( i_count = 0; i_count < 3; i_count++ )
                {
                    if( i_extra < 2 )
                        break;
                    pi_size[i_count] = GetWBE( p_extra );
                    pp_data[i_count] = &p_extra[2];
                    if( i_extra < pi_size[i_count] + 2 )
                        break;

                    p_extra += 2 + pi_size[i_count];
                    i_extra -= 2 + pi_size[i_count];
                }
                if( i_count > 0 && xiph_PackHeaders( &fmt.i_extra, &fmt.p_extra,
                                                     pi_size, pp_data, i_count ) )
                {
                    fmt.i_extra = 0;
                    fmt.p_extra = NULL;
                }
            }
            else if( cc->codec_id == CODEC_ID_SPEEX && b_ogg )
            {
                const uint8_t p_dummy_comment[] = {
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                };
                unsigned pi_size[2];
                const void *pp_data[2];

                pi_size[0] = i_extra;
                pp_data[0] = p_extra;

                pi_size[1] = sizeof(p_dummy_comment);
                pp_data[1] = p_dummy_comment;

                if( pi_size[0] > 0 && xiph_PackHeaders( &fmt.i_extra, &fmt.p_extra,
                                                        pi_size, pp_data, 2 ) )
                {
                    fmt.i_extra = 0;
                    fmt.p_extra = NULL;
                }
            }
            else if( cc->extradata_size > 0 )
            {
                fmt.p_extra = malloc( i_extra );
                if( fmt.p_extra )
                {
                    fmt.i_extra = i_extra;
                    memcpy( fmt.p_extra, p_extra, i_extra );
                }
            }
            es = es_out_Add( p_demux->out, &fmt );
            if( s->disposition & AV_DISPOSITION_DEFAULT )
                es_out_Control( p_demux->out, ES_OUT_SET_ES_DEFAULT, es );
            es_format_Clean( &fmt );

            msg_Dbg( p_demux, "adding es: %s codec = %4.4s",
                     psz_type, (char*)&fcc );
            TAB_APPEND( p_sys->i_tk, p_sys->tk, es );
        }
    }
    if( p_sys->ic->start_time != (int64_t)AV_NOPTS_VALUE )
        i_start_time = p_sys->ic->start_time * 1000000 / AV_TIME_BASE;

    msg_Dbg( p_demux, "AVFormat supported stream" );
    msg_Dbg( p_demux, "    - format = %s (%s)",
             p_sys->fmt->name, p_sys->fmt->long_name );
    msg_Dbg( p_demux, "    - start time = %"PRId64, i_start_time );
    msg_Dbg( p_demux, "    - duration = %"PRId64,
             ( p_sys->ic->duration != (int64_t)AV_NOPTS_VALUE ) ?
             p_sys->ic->duration * 1000000 / AV_TIME_BASE : -1 );

    if( p_sys->ic->nb_chapters > 0 )
        p_sys->p_title = vlc_input_title_New();
    for( i = 0; i < p_sys->ic->nb_chapters; i++ )
    {
        seekpoint_t *s = vlc_seekpoint_New();

        AVMetadataTag *title = av_metadata_get( p_sys->ic->metadata, "title", NULL, 0);
        if( title && title->value )
        {
            s->psz_name = strdup( title->value );
            EnsureUTF8( s->psz_name );
            msg_Dbg( p_demux, "    - chapter %d: %s", i, s->psz_name );
        }
        s->i_time_offset = p_sys->ic->chapters[i]->start * 1000000 *
            p_sys->ic->chapters[i]->time_base.num /
            p_sys->ic->chapters[i]->time_base.den -
            (i_start_time != -1 ? i_start_time : 0 );
        TAB_APPEND( p_sys->p_title->i_seekpoint, p_sys->p_title->seekpoint, s );
    }

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close
 *****************************************************************************/
void CloseDemux( vlc_object_t *p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys = p_demux->p_sys;

    FREENULL( p_sys->tk );

    if( p_sys->ic ) av_close_input_file( p_sys->ic );

    for( int i = 0; i < p_sys->i_attachments; i++ )
        free( p_sys->attachments[i] );
    TAB_CLEAN( p_sys->i_attachments, p_sys->attachments);

    if( p_sys->p_title )
        vlc_input_title_Delete( p_sys->p_title );

    free( p_sys );
}

/*****************************************************************************
 * Demux:
 *****************************************************************************/
static int Demux( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    AVPacket    pkt;
    block_t     *p_frame;
    int64_t     i_start_time;

    /* Read a frame */
    if( av_read_frame( p_sys->ic, &pkt ) )
    {
        return 0;
    }
    if( pkt.stream_index < 0 || pkt.stream_index >= p_sys->i_tk )
    {
        av_free_packet( &pkt );
        return 1;
    }
    const AVStream *p_stream = p_sys->ic->streams[pkt.stream_index];
    if( p_stream->time_base.den <= 0 )
    {
        msg_Warn( p_demux, "Invalid time base for the stream %d", pkt.stream_index );
        av_free_packet( &pkt );
        return 1;
    }
    if( p_stream->codec->codec_id == CODEC_ID_SSA )
    {
        p_frame = BuildSsaFrame( &pkt, p_sys->i_ssa_order++ );
        if( !p_frame )
        {
            av_free_packet( &pkt );
            return 1;
        }
    }
    else
    {
        if( ( p_frame = block_New( p_demux, pkt.size ) ) == NULL )
        {
            av_free_packet( &pkt );
            return 0;
        }
        memcpy( p_frame->p_buffer, pkt.data, pkt.size );
    }

    if( pkt.flags & AV_PKT_FLAG_KEY )
        p_frame->i_flags |= BLOCK_FLAG_TYPE_I;

    i_start_time = ( p_sys->ic->start_time != (int64_t)AV_NOPTS_VALUE ) ?
        ( p_sys->ic->start_time * 1000000 / AV_TIME_BASE )  : 0;

    p_frame->i_dts = ( pkt.dts == (int64_t)AV_NOPTS_VALUE ) ?
        VLC_TS_INVALID : (pkt.dts) * 1000000 *
        p_stream->time_base.num /
        p_stream->time_base.den - i_start_time + VLC_TS_0;
    p_frame->i_pts = ( pkt.pts == (int64_t)AV_NOPTS_VALUE ) ?
        VLC_TS_INVALID : (pkt.pts) * 1000000 *
        p_stream->time_base.num /
        p_stream->time_base.den - i_start_time + VLC_TS_0;
    if( pkt.duration > 0 && p_frame->i_length <= 0 )
        p_frame->i_length = pkt.duration * 1000000 *
            p_stream->time_base.num /
            p_stream->time_base.den;

    if( pkt.dts != AV_NOPTS_VALUE && pkt.dts == pkt.pts &&
        p_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO )
    {
        /* Add here notoriously bugged file formats/samples regarding PTS */
        if( !strcmp( p_sys->fmt->name, "flv" ) )
            p_frame->i_pts = VLC_TS_INVALID;
    }
#ifdef AVFORMAT_DEBUG
    msg_Dbg( p_demux, "tk[%d] dts=%"PRId64" pts=%"PRId64,
             pkt.stream_index, p_frame->i_dts, p_frame->i_pts );
#endif

    if( p_frame->i_dts > VLC_TS_INVALID  &&
        ( pkt.stream_index == p_sys->i_pcr_tk || p_sys->i_pcr_tk < 0 ) )
    {
        p_sys->i_pcr_tk = pkt.stream_index;
        p_sys->i_pcr = p_frame->i_dts;

        es_out_Control( p_demux->out, ES_OUT_SET_PCR, (int64_t)p_sys->i_pcr );
    }

    es_out_Send( p_demux->out, p_sys->tk[pkt.stream_index], p_frame );

    UpdateSeekPoint( p_demux, p_sys->i_pcr);
    av_free_packet( &pkt );
    return 1;
}

static void UpdateSeekPoint( demux_t *p_demux, int64_t i_time )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    int i;

    if( !p_sys->p_title )
        return;

    for( i = 0; i < p_sys->p_title->i_seekpoint; i++ )
    {
        if( i_time < p_sys->p_title->seekpoint[i]->i_time_offset )
            break;
    }
    i--;

    if( i != p_demux->info.i_seekpoint && i >= 0 )
    {
        p_demux->info.i_seekpoint = i;
        p_demux->info.i_update |= INPUT_UPDATE_SEEKPOINT;
    }
}

static block_t *BuildSsaFrame( const AVPacket *p_pkt, unsigned i_order )
{
    if( p_pkt->size <= 0 )
        return NULL;

    char buffer[256];
    const size_t i_buffer_size = __MIN( (int)sizeof(buffer) - 1, p_pkt->size );
    memcpy( buffer, p_pkt->data, i_buffer_size );
    buffer[i_buffer_size] = '\0';

    /* */
    int i_layer;
    int h0, m0, s0, c0;
    int h1, m1, s1, c1;
    int i_position = 0;
    if( sscanf( buffer, "Dialogue: %d,%d:%d:%d.%d,%d:%d:%d.%d,%n", &i_layer,
                &h0, &m0, &s0, &c0, &h1, &m1, &s1, &c1, &i_position ) < 9 )
        return NULL;
    if( i_position <= 0 || (unsigned)i_position >= i_buffer_size )
        return NULL;

    char *p;
    if( asprintf( &p, "%u,%d,%.*s", i_order, i_layer, p_pkt->size - i_position, p_pkt->data + i_position ) < 0 )
        return NULL;

    block_t *p_frame = block_heap_Alloc( p, p, strlen(p) + 1 );
    if( p_frame )
        p_frame->i_length = CLOCK_FREQ * ((h1-h0) * 3600 +
                                          (m1-m0) * 60 +
                                          (s1-s0) * 1) +
                            CLOCK_FREQ * (c1-c0) / 100;
    return p_frame;
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( demux_t *p_demux, int i_query, va_list args )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    double f, *pf;
    int64_t i64, *pi64;

    switch( i_query )
    {
        case DEMUX_GET_POSITION:
            pf = (double*) va_arg( args, double* ); *pf = 0.0;
            i64 = stream_Size( p_demux->s );
            if( i64 > 0 )
            {
                double current = stream_Tell( p_demux->s );
                *pf = current / (double)i64;
            }

            if( (p_sys->ic->duration != (int64_t)AV_NOPTS_VALUE) && (p_sys->i_pcr > 0) )
            {
                *pf = (double)p_sys->i_pcr / (double)p_sys->ic->duration;
            }

            return VLC_SUCCESS;

        case DEMUX_SET_POSITION:
            f = (double) va_arg( args, double );
            if( p_sys->i_pcr > 0 )
            {
                i64 = p_sys->ic->duration * f;
                if( p_sys->ic->start_time != (int64_t)AV_NOPTS_VALUE )
                    i64 += p_sys->ic->start_time;

                msg_Warn( p_demux, "DEMUX_SET_POSITION: %"PRId64, i64 );

                /* If we have a duration, we prefer to seek by time
                   but if we don't, or if the seek fails, try BYTE seeking */
                if( p_sys->ic->duration == (int64_t)AV_NOPTS_VALUE ||
                    (av_seek_frame( p_sys->ic, -1, i64, 0 ) < 0) )
                {
                    int64_t i_size = stream_Size( p_demux->s );
                    i64 = (i_size * f);

                    msg_Warn( p_demux, "DEMUX_SET_BYTE_POSITION: %"PRId64, i64 );
                    if( av_seek_frame( p_sys->ic, -1, i64, AVSEEK_FLAG_BYTE ) < 0 )
                        return VLC_EGENERIC;
                }
                else
                {
                    UpdateSeekPoint( p_demux, i64 );
                }
                p_sys->i_pcr = -1; /* Invalidate time display */
            }
            return VLC_SUCCESS;

        case DEMUX_GET_LENGTH:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            if( p_sys->ic->duration != (int64_t)AV_NOPTS_VALUE )
                *pi64 = p_sys->ic->duration * 1000000 / AV_TIME_BASE;
            else
                *pi64 = 0;
            return VLC_SUCCESS;

        case DEMUX_GET_TIME:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            *pi64 = p_sys->i_pcr;
            return VLC_SUCCESS;

        case DEMUX_SET_TIME:
            i64 = (int64_t)va_arg( args, int64_t );
            i64 = i64 *AV_TIME_BASE / 1000000;
            if( p_sys->ic->start_time != (int64_t)AV_NOPTS_VALUE )
                i64 += p_sys->ic->start_time;

            msg_Warn( p_demux, "DEMUX_SET_TIME: %"PRId64, i64 );

            if( av_seek_frame( p_sys->ic, -1, i64, 0 ) < 0 )
            {
                return VLC_EGENERIC;
            }
            p_sys->i_pcr = -1; /* Invalidate time display */
            UpdateSeekPoint( p_demux, i64 );
            return VLC_SUCCESS;

        case DEMUX_HAS_UNSUPPORTED_META:
        {
            bool *pb_bool = (bool*)va_arg( args, bool* );
            *pb_bool = true;
            return VLC_SUCCESS;
        }


        case DEMUX_GET_META:
        {
            vlc_meta_t *p_meta = (vlc_meta_t*)va_arg( args, vlc_meta_t* );

            AVMetadataTag *title = av_metadata_get( p_sys->ic->metadata, "language", NULL, 0 );
            AVMetadataTag *artist = av_metadata_get( p_sys->ic->metadata, "artist", NULL, 0 );
            AVMetadataTag *copyright = av_metadata_get( p_sys->ic->metadata, "copyright", NULL, 0 );
            AVMetadataTag *comment = av_metadata_get( p_sys->ic->metadata, "comment", NULL, 0 );
            AVMetadataTag *genre = av_metadata_get( p_sys->ic->metadata, "genre", NULL, 0 );

            if( title && title->value )
                vlc_meta_SetTitle( p_meta, title->value );
            if( artist && artist->value )
                vlc_meta_SetArtist( p_meta, artist->value );
            if( copyright && copyright->value )
                vlc_meta_SetCopyright( p_meta, copyright->value );
            if( comment && comment->value )
                vlc_meta_SetDescription( p_meta, comment->value );
            if( genre && genre->value )
                vlc_meta_SetGenre( p_meta, genre->value );
            return VLC_SUCCESS;
        }

        case DEMUX_GET_ATTACHMENTS:
        {
            input_attachment_t ***ppp_attach =
                (input_attachment_t***)va_arg( args, input_attachment_t*** );
            int *pi_int = (int*)va_arg( args, int * );
            int i;

            if( p_sys->i_attachments <= 0 )
                return VLC_EGENERIC;

            *pi_int = p_sys->i_attachments;;
            *ppp_attach = malloc( sizeof(input_attachment_t**) * p_sys->i_attachments );
            for( i = 0; i < p_sys->i_attachments; i++ )
                (*ppp_attach)[i] = vlc_input_attachment_Duplicate( p_sys->attachments[i] );
            return VLC_SUCCESS;
        }

        case DEMUX_GET_TITLE_INFO:
        {
            input_title_t ***ppp_title = (input_title_t***)va_arg( args, input_title_t*** );
            int *pi_int    = (int*)va_arg( args, int* );
            int *pi_title_offset = (int*)va_arg( args, int* );
            int *pi_seekpoint_offset = (int*)va_arg( args, int* );

            if( !p_sys->p_title )
                return VLC_EGENERIC;

            *pi_int = 1;
            *ppp_title = malloc( sizeof( input_title_t**) );
            (*ppp_title)[0] = vlc_input_title_Duplicate( p_sys->p_title );
            *pi_title_offset = 0;
            *pi_seekpoint_offset = 0;
            return VLC_SUCCESS;
        }
        case DEMUX_SET_TITLE:
        {
            const int i_title = (int)va_arg( args, int );
            if( !p_sys->p_title || i_title != 0 )
                return VLC_EGENERIC;
            return VLC_SUCCESS;
        }
        case DEMUX_SET_SEEKPOINT:
        {
            const int i_seekpoint = (int)va_arg( args, int );
            if( !p_sys->p_title )
                return VLC_EGENERIC;

            i64 = p_sys->p_title->seekpoint[i_seekpoint]->i_time_offset *AV_TIME_BASE / 1000000;
            if( p_sys->ic->start_time != (int64_t)AV_NOPTS_VALUE )
                i64 += p_sys->ic->start_time;

            msg_Warn( p_demux, "DEMUX_SET_TIME: %"PRId64, i64 );

            if( av_seek_frame( p_sys->ic, -1, i64, 0 ) < 0 )
            {
                return VLC_EGENERIC;
            }
            p_sys->i_pcr = -1; /* Invalidate time display */
            UpdateSeekPoint( p_demux, i64 );
            return VLC_SUCCESS;
        }


        default:
            return VLC_EGENERIC;
    }
}

