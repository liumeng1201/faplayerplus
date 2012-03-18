
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc/vlc.h>
#include <vlc/libvlc.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_media_player.h>
/* XXX: NG */
#include "control/media_player_internal.h"
#include <vlc_common.h>
#include <vlc_input.h>

#ifdef ANDROID

#include <jni.h>
#include <android/log.h>
#include <stdlib.h>

#include "libvlcjni.h"

#define NAME1(CLZ, FUN) Java_##CLZ##_##FUN
#define NAME2(CLZ, FUN) NAME1(CLZ, FUN)

#define NAME(FUN) NAME2(CLASS, FUN)

#define CLASS org_stagex_danmaku_player_VlcMediaPlayer
#define PREFIX "org/stagex/danmaku/player/"

/* the JVM */
JavaVM *gJVM = NULL;

/* JNI fields */
static jclass clz_VlcEvent = 0;
static jfieldID f_VlcEvent_eventType = 0;
static jfieldID f_VlcEvent_booleanValue = 0;
static jfieldID f_VlcEvent_intValue = 0;
static jfieldID f_VlcEvent_longValue = 0;
static jfieldID f_VlcEvent_floatValue = 0;
static jfieldID f_VlcEvent_stringValue = 0;
static jmethodID m_VlcMediaPlayer_onVlcEvent = 0;

/* */

static void *vlc_jni_player_gc_thread(void *);

libvlc_instance_t *s_vlc_instance = 0;

typedef struct _vlc_jni_player
{
    int status;
    jobject object;
    jobject reference;
    libvlc_media_player_t *player;
    libvlc_media_t *media;
    int parse_status;
    vlc_mutex_t parse_lock;
    vlc_cond_t parse_cond;
    int buffering;
    void *surface;
    vlc_mutex_t surface_lock;
} vlc_jni_player_t;

static void *s_surface = 0;
static vlc_mutex_t s_surface_lock;

static vlc_mutex_t s_VlcMediaPlayer_lock;
static vlc_cond_t s_VlcMediaPlayer_cond;
static vlc_array_t *s_VlcMediaPlayer_array = 0;
static int s_gc_thread = 0;

static inline int vlc_jni_player_count()
{
    vlc_mutex_lock(&s_VlcMediaPlayer_lock);
    int count = vlc_array_count(s_VlcMediaPlayer_array);
    vlc_mutex_unlock(&s_VlcMediaPlayer_lock);
    return count;
}

static inline vlc_jni_player_t *vlc_jni_player_find_or_throw(JNIEnv *env, jobject obj)
{
    vlc_jni_player_t *vj = 0;
    vlc_mutex_lock(&s_VlcMediaPlayer_lock);
    for (int i = 0; i < vlc_array_count(s_VlcMediaPlayer_array); i++)
    {
        vlc_jni_player_t *t = (vlc_jni_player_t *) vlc_array_item_at_index(s_VlcMediaPlayer_array, i);
        if(t->object == obj || t->reference == obj)
        {
            vj = t;
            break;
        }
    }
    vlc_mutex_unlock(&s_VlcMediaPlayer_lock);
    if (!vj)
    {
        /* XXX: throw */
        __android_log_print(ANDROID_LOG_ERROR, "faplayer", "could not find %p", obj);
    }
    return vj;
}

static inline void vlc_jni_player_push(vlc_jni_player_t *vj)
{
    vlc_mutex_lock(&s_VlcMediaPlayer_lock);
    vlc_array_append(s_VlcMediaPlayer_array, vj);
    vlc_mutex_unlock(&s_VlcMediaPlayer_lock);
}

static inline void vlc_jni_player_kill(jobject obj)
{
    vlc_mutex_lock(&s_VlcMediaPlayer_lock);
    for (int i = 0; i < vlc_array_count(s_VlcMediaPlayer_array); i++)
    {
        vlc_jni_player_t *t = (vlc_jni_player_t *) vlc_array_item_at_index(s_VlcMediaPlayer_array, i);
        if(t->object == obj || t->reference == obj)
        {
            t->status = 0;
            vlc_cond_signal(&s_VlcMediaPlayer_cond);
            break;
        }
    }
    vlc_mutex_unlock(&s_VlcMediaPlayer_lock);
}

static inline vlc_jni_player_t *vlc_jni_player_find_by_vout(vlc_object_t *p_vout)
{
    vlc_jni_player_t *vj = 0;
    vlc_mutex_lock(&s_VlcMediaPlayer_lock);
    for (int i = 0; i < vlc_array_count(s_VlcMediaPlayer_array); i++)
    {
        vlc_jni_player_t *t = (vlc_jni_player_t *) vlc_array_item_at_index(s_VlcMediaPlayer_array, i);
        input_thread_t *p_input = libvlc_get_input_thread(t->player);
        if (!p_input)
            continue;
        size_t n = 0;
        vout_thread_t **pp_vouts = NULL;
        if (input_Control(p_input, INPUT_GET_VOUTS, &pp_vouts, &n))
        {
            vlc_object_release(p_input);
            continue;
        }
        for (size_t c = 0; c < n; c++)
        {
            if ((int) p_vout == (int) pp_vouts[c])
                vj = t;
            vlc_object_release((vlc_object_t *) pp_vouts[c]);
        }
        free(pp_vouts);
        vlc_object_release(p_input);
        if (vj)
            break;
    }
    vlc_mutex_unlock(&s_VlcMediaPlayer_lock);
    return vj;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    gJVM = vm;
    const char *argv[] = {"-I", "dummy", "-vvv", "--no-plugins-cache", "--no-drop-late-frames", "--input-timeshift-path", "/data/local/tmp"};
    s_vlc_instance = libvlc_new_with_builtins(sizeof(argv) / sizeof(*argv), argv, vlc_builtins_modules);
    vlc_mutex_init(&s_surface_lock);
    s_VlcMediaPlayer_array = vlc_array_new();
    vlc_mutex_init(&s_VlcMediaPlayer_lock);
    vlc_cond_init(&s_VlcMediaPlayer_cond);

    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
{
    vlc_mutex_destroy(&s_surface_lock);
    /* TODO: release all left player instances */
    vlc_mutex_destroy(&s_VlcMediaPlayer_lock);
    vlc_cond_destroy(&s_VlcMediaPlayer_cond);
}

JNIEXPORT int Java_org_stagex_danmaku_helper_SystemUtility_setenv(JNIEnv *env, jclass klz, jstring key, jstring val, jboolean overwrite)
{
    const char *key_utf8 = (*env)->GetStringUTFChars(env, key, NULL);
    const char *val_utf8 = (*env)->GetStringUTFChars(env, val, NULL);
    int err = setenv(key_utf8, val_utf8, overwrite);
    (*env)->ReleaseStringUTFChars(env, key, key_utf8);
    (*env)->ReleaseStringUTFChars(env, val, val_utf8);
    return err;
}

static void vlc_event_callback(const libvlc_event_t *ev, void *data)
{
    JNIEnv *env;
    jobject obj_VlcMediaPlayer;
    jobject obj_VlcEvent;
    int trigger = 1;

    if ((*gJVM)->AttachCurrentThread(gJVM, &env, 0) < 0)
        return;
    obj_VlcEvent = (*env)->AllocObject(env, clz_VlcEvent);
    if (!obj_VlcEvent)
        return;
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, (jobject) data);
    char *mrl = libvlc_media_get_mrl(vj->media);
    (*env)->SetIntField(env, obj_VlcEvent, f_VlcEvent_eventType, ev->type);
    switch (ev->type) {
    case libvlc_MediaDurationChanged: {
        int64_t duration = ev->u.media_duration_changed.new_duration;
        (*env)->SetLongField(env, obj_VlcEvent, f_VlcEvent_longValue, (jlong) duration);
        break;
    }
    case libvlc_MediaStateChanged: {
        int state = ev->u.media_state_changed.new_state;
        (*env)->SetIntField(env, obj_VlcEvent, f_VlcEvent_intValue, state);
        /* wake up if there is an error */
        if (state == libvlc_MediaPlayerEncounteredError) {
            vlc_mutex_lock(&vj->parse_lock);
            vj->parse_status = 2;
            vlc_cond_broadcast(&vj->parse_cond);
            vlc_mutex_unlock(&vj->parse_lock);
        }
        break;
    }
    case libvlc_MediaParsedChanged: {
        libvlc_media_player_play(vj->player);
        trigger = 0;
        break;
    }
    case libvlc_MediaPlayerBuffering: {
        float cache = ev->u.media_player_buffering.new_cache;
        (*env)->SetFloatField(env, obj_VlcEvent, f_VlcEvent_floatValue, cache);
        if ((int) cache == 100) {
            vj->buffering += 1;
            /* if it's the first time */
            if (vj->buffering == 1) {
                /* send buffering update event now */
                (*env)->CallVoidMethod(env, vj->reference, m_VlcMediaPlayer_onVlcEvent, obj_VlcEvent);
                libvlc_media_player_set_pause(vj->player, 1);
                /* asynchonous preparing is done */
                vlc_mutex_lock(&vj->parse_lock);
                vj->parse_status = 1;
                vlc_cond_broadcast(&vj->parse_cond);
                vlc_mutex_unlock(&vj->parse_lock);
                /* simulate a media prepared event */
                (*env)->SetIntField(env, obj_VlcEvent, f_VlcEvent_eventType, libvlc_MediaParsedChanged);
                (*env)->SetBooleanField(env, obj_VlcEvent, f_VlcEvent_booleanValue, 1);
            }
        }
        break;
    }
    case libvlc_MediaPlayerTimeChanged: {
        int64_t time = ev->u.media_player_time_changed.new_time;
        (*env)->SetLongField(env, obj_VlcEvent, f_VlcEvent_longValue, (jlong) time);
        break;
    }
    case libvlc_MediaPlayerPositionChanged: {
        float position = ev->u.media_player_position_changed.new_position;
        (*env)->SetFloatField(env, obj_VlcEvent, f_VlcEvent_floatValue, position);
        break;
    }
    case libvlc_MediaPlayerSeekableChanged: {
        int seekable = ev->u.media_player_seekable_changed.new_seekable;
        (*env)->SetBooleanField(env, obj_VlcEvent, f_VlcEvent_booleanValue, seekable > 0);
        break;
    }
    case libvlc_MediaPlayerPausableChanged: {
        int pausable = ev->u.media_player_pausable_changed.new_pausable;
        (*env)->SetBooleanField(env, obj_VlcEvent, f_VlcEvent_booleanValue, pausable > 0);
        break;
    }
    case libvlc_MediaPlayerTitleChanged: {
        int title = ev->u.media_player_title_changed.new_title;
        (*env)->SetIntField(env, obj_VlcEvent, f_VlcEvent_intValue, title);
        break;
    }
    case libvlc_MediaPlayerSnapshotTaken: {
        char *p = ev->u.media_player_snapshot_taken.psz_filename;
        jstring path = (*env)->NewStringUTF(env, p);
        (*env)->SetObjectField(env, obj_VlcEvent, f_VlcEvent_stringValue, path);
        break;
    }
    case libvlc_MediaPlayerLengthChanged: {
        int64_t length = ev->u.media_player_length_changed.new_length;
        (*env)->SetLongField(env, obj_VlcEvent, f_VlcEvent_longValue, (jlong) length);
        break;
    }
    default:
        break;
    }
    if (trigger)
        (*env)->CallVoidMethod(env, vj->reference, m_VlcMediaPlayer_onVlcEvent, obj_VlcEvent);
    (*env)->DeleteLocalRef(env, obj_VlcEvent);
    free(mrl);
    /* EXPLAIN: this is called in pthread wrapper routines */
    // (*gJVM)->DetachCurrentThread(gJVM);
}

JNIEXPORT void JNICALL NAME(nativeAttachSurface)(JNIEnv *env, jobject thiz, jobject s)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    jint surface = 0;
    jclass clz;
    jfieldID f_Surface_mSurface;
    clz = (*env)->GetObjectClass(env, s);
    f_Surface_mSurface = (*env)->GetFieldID(env, clz, "mSurface", "I");
    if (f_Surface_mSurface == 0)
    {
        jthrowable e = (*env)->ExceptionOccurred(env);
        if (e)
        {
            (*env)->DeleteLocalRef(env, e);
            (*env)->ExceptionClear(env);
        }
        f_Surface_mSurface = (*env)->GetFieldID(env, clz, "mNativeSurface", "I");
    }
    (*env)->DeleteLocalRef(env, clz);
    surface = (*env)->GetIntField(env, s, f_Surface_mSurface);
    vlc_mutex_lock(&vj->surface_lock);
    vj->surface = (void *) surface;
    vlc_mutex_unlock(&vj->surface_lock);
}

JNIEXPORT void JNICALL NAME(nativeDetachSurface)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    vlc_mutex_lock(&vj->surface_lock);
    vj->surface = 0;
    vlc_mutex_unlock(&vj->surface_lock);
}

static libvlc_event_type_t md_listening[] = {
    libvlc_MediaDurationChanged,
    libvlc_MediaParsedChanged,
    libvlc_MediaStateChanged,
};

static libvlc_event_type_t mp_listening[] = {
    libvlc_MediaPlayerOpening,
    libvlc_MediaPlayerBuffering,
    libvlc_MediaPlayerPlaying,
    libvlc_MediaPlayerPaused,
    libvlc_MediaPlayerStopped,
    libvlc_MediaPlayerEndReached,
    libvlc_MediaPlayerEncounteredError,
    libvlc_MediaPlayerTimeChanged,
    libvlc_MediaPlayerSeekableChanged,
    libvlc_MediaPlayerPausableChanged,
    libvlc_MediaPlayerLengthChanged,
};

JNIEXPORT void JNICALL NAME(nativeCreate)(JNIEnv *env, jobject thiz)
{
    /* setup JNI fields if needed */
    jclass clz;
    if (!m_VlcMediaPlayer_onVlcEvent)
    {
        clz = (*env)->GetObjectClass(env, thiz);
        m_VlcMediaPlayer_onVlcEvent = (*env)->GetMethodID(env, clz, "onVlcEvent", "(L" PREFIX "VlcMediaPlayer$VlcEvent;)V");
        (*env)->DeleteLocalRef(env, clz);
    }
    if (!clz_VlcEvent)
    {
        clz = (*env)->FindClass(env, PREFIX "VlcMediaPlayer$VlcEvent");
        clz_VlcEvent = (*env)->NewGlobalRef(env, clz);
        f_VlcEvent_eventType = (*env)->GetFieldID(env, clz, "eventType", "I");
        f_VlcEvent_booleanValue = (*env)->GetFieldID(env, clz, "booleanValue", "Z");
        f_VlcEvent_intValue = (*env)->GetFieldID(env, clz, "intValue", "I");
        f_VlcEvent_longValue = (*env)->GetFieldID(env, clz, "longValue", "J");
        f_VlcEvent_floatValue = (*env)->GetFieldID(env, clz, "floatValue", "F");
        f_VlcEvent_stringValue = (*env)->GetFieldID(env, clz, "stringValue", "Ljava/lang/String;");
        (*env)->DeleteLocalRef(env, clz);
    }
    /* */
    if (!s_gc_thread)
    {
        vlc_thread_t gc;
        if (vlc_clone(&gc, vlc_jni_player_gc_thread, NULL, VLC_THREAD_PRIORITY_LOW))
        {
            /* XXX: wtf? */
        }
        s_gc_thread = -1;
    }
    /* */
    vlc_jni_player_t *vj = calloc(1, sizeof(vlc_jni_player_t));
    vj->object = thiz;
    vj->reference = (*env)->NewGlobalRef(env, thiz);
    vlc_mutex_init(&vj->parse_lock);
    vlc_cond_init(&vj->parse_cond);
    vlc_mutex_init(&vj->surface_lock);
    vj->status = 1;
    vj->player = libvlc_media_player_new(s_vlc_instance);
    libvlc_event_manager_t *em = libvlc_media_player_event_manager(vj->player);
    for (int i = 0; i < sizeof(mp_listening) / sizeof(*mp_listening); i++)
    {
        libvlc_event_attach(em, mp_listening[i], vlc_event_callback, thiz);
    }
    vlc_jni_player_push(vj);
}

JNIEXPORT void JNICALL NAME(nativeRelease)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_kill(thiz);
}

JNIEXPORT jint JNICALL NAME(nativeGetCurrentPosition)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    int64_t position = libvlc_media_player_get_time(vj->player);
    if (position < 0)
    {
        return -1;
    }
    return (jint) (position / 1000);
}

JNIEXPORT jint JNICALL NAME(nativeGetDuration)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    int64_t duration = libvlc_media_player_get_length(vj->player);
    if (duration < 0)
    {
        return -1;
    }
    return (jint) (duration / 1000);
}

JNIEXPORT jint JNICALL NAME(nativeGetVideoHeight)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    if (!vj->media || !libvlc_media_is_parsed(vj->media))
        return 0;
    /* FIXME: it returns the first video's information only */
    int i, n;
    int width = 0, height = 0;
    libvlc_media_track_info_t *track = 0;
    n = libvlc_media_get_tracks_info(vj->media, &track);
    if (n <= 0)
        return 0;
    for (i = 0; i < n; i++) {
        libvlc_media_track_info_t t = track[i];
        if (t.i_type == libvlc_track_video) {
            width = t.u.video.i_width;
            height = t.u.video.i_height;
            break;
        }
    }
    free(track);
    return height;
}

JNIEXPORT jint JNICALL NAME(nativeGetVideoWidth)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    if (!vj->media || !libvlc_media_is_parsed(vj->media))
        return 0;
    /* FIXME: it returns the first video's information only */
    int i, n;
    int width = 0, height = 0;
    libvlc_media_track_info_t *track = 0;
    n = libvlc_media_get_tracks_info(vj->media, &track);
    if (n <= 0)
        return 0;
    for (i = 0; i < n; i++) {
        libvlc_media_track_info_t t = track[i];
        if (t.i_type == libvlc_track_video) {
            width = t.u.video.i_width;
            height = t.u.video.i_height;
            break;
        }
    }
    free(track);
    return width;
}

JNIEXPORT jboolean JNICALL NAME(nativeIsLooping)(JNIEnv *env, jobject thiz)
{
    return 0;
}

JNIEXPORT jboolean JNICALL NAME(nativeIsPlaying)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    return (libvlc_media_player_is_playing(vj->player) != 0);
}

JNIEXPORT void JNICALL NAME(nativePause)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    libvlc_media_player_set_pause(vj->player, 1);
}

JNIEXPORT void JNICALL NAME(nativePrepare)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    char *mrl = libvlc_media_get_mrl(vj->media);
    if (strncmp(mrl, "file://", 7) == 0)
        libvlc_media_parse(vj->media);
    else {
        libvlc_media_player_play(vj->player);
        vlc_mutex_lock(&vj->parse_lock);
        while (!vj->parse_status)
            vlc_cond_wait(&vj->parse_cond, &vj->parse_lock);
        vlc_mutex_unlock(&vj->parse_lock);
    }
    free(mrl);
}

JNIEXPORT void JNICALL NAME(nativePrepareAsync)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    char *mrl = libvlc_media_get_mrl(vj->media);
    if (strncmp(mrl, "file://", 7) == 0)
        libvlc_media_parse_async(vj->media);
    else
        libvlc_media_player_play(vj->player);
    free(mrl);
}

JNIEXPORT void JNICALL NAME(nativeSeekTo)(JNIEnv *env, jobject thiz, jint msec)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    libvlc_media_player_set_time(vj->player, msec);
}

JNIEXPORT void JNICALL NAME(nativeSetDataSource)(JNIEnv *env, jobject thiz, jstring path)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    const char *str = (*env)->GetStringUTFChars(env, path, 0);
    if (!str)
    {
        /* XXX: throw */
        return;
    }
    libvlc_media_t *media = (*str == '/') ? libvlc_media_new_path(s_vlc_instance, str) : libvlc_media_new_location(s_vlc_instance, str);
    if (media)
    {
        libvlc_event_manager_t *em = libvlc_media_event_manager(media);
        for (int i = 0; i < sizeof(md_listening) / sizeof(*md_listening); i++)
        {
            libvlc_event_attach(em, md_listening[i], vlc_event_callback, thiz);
        }
        /* this will cancel current input and start a new one */
        libvlc_media_player_set_media(vj->player, media);
        /* */
        vj->media = media;
        vj->buffering = 0;
    }
    (*env)->ReleaseStringUTFChars(env, path, str);
    if (!media)
    {
        /* XXX: throw */
        return;
    }
}

JNIEXPORT void JNICALL NAME(nativeSetLooping)(JNIEnv *env, jobject thiz, jboolean looping)
{

}

JNIEXPORT void JNICALL NAME(nativeStart)(JNIEnv *env, jobject thiz)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    libvlc_media_player_play(vj->player);
}

JNIEXPORT void JNICALL NAME(nativeStop)(JNIEnv *env, jobject thiz) {
    vlc_jni_player_t *vj = vlc_jni_player_find_or_throw(env, thiz);
    libvlc_media_player_stop(vj->player);
}

static void *vlc_jni_player_gc_thread(void *para)
{
    while (true)
    {
        vlc_jni_player_t *vj = 0;
        int status = 1;
        vlc_mutex_lock(&s_VlcMediaPlayer_lock);
        while (true)
        {
            for (int i = 0; i < vlc_array_count(s_VlcMediaPlayer_array); i++)
            {
                vlc_jni_player_t *t = (vlc_jni_player_t *) vlc_array_item_at_index(s_VlcMediaPlayer_array, i);
                status &= t->status;
                if (!status)
                {
                    vj = t;
                    vlc_array_remove(s_VlcMediaPlayer_array, i);
                    __android_log_print(ANDROID_LOG_DEBUG, "faplayer", "about to destroy %p", vj->player);
                    break;
                }
            }
            if (status)
                vlc_cond_wait(&s_VlcMediaPlayer_cond, &s_VlcMediaPlayer_lock);
            else
                break;
        }
        vlc_mutex_unlock(&s_VlcMediaPlayer_lock);
        /* wake up */
        vlc_mutex_lock(&vj->parse_lock);
        vj->parse_status = 4;
        vlc_cond_broadcast(&vj->parse_cond);
        vlc_mutex_unlock(&vj->parse_lock);
        /* free events */
        libvlc_event_manager_t *em;
        libvlc_media_t *md = libvlc_media_player_get_media(vj->player);
        if (md) {
            em = libvlc_media_event_manager(md);
            for (int i = 0; i < sizeof(md_listening) / sizeof(*md_listening); i++)
            {
                libvlc_event_detach(em, md_listening[i], vlc_event_callback, vj->object);
            }
        }
        em = libvlc_media_player_event_manager(vj->player);
        for (int i = 0; i < sizeof(mp_listening) / sizeof(*mp_listening); i++)
        {
            libvlc_event_detach(em, mp_listening[i], vlc_event_callback, vj->object);
        }
        libvlc_media_player_stop(vj->player);
        libvlc_media_player_release(vj->player);
        /* XXX: free global reference */

        /* */
        vlc_mutex_destroy(&vj->parse_lock);
        vlc_cond_destroy(&vj->parse_cond);
        vlc_mutex_destroy(&vj->surface_lock);
        free(vj);
    }
}

void *jni_LockAndGetAndroidSurface(vlc_object_t *p_vout)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_by_vout(p_vout);
    if (vj)
    {
        vlc_mutex_lock(&vj->surface_lock);
        return vj->surface;
    }
    return NULL;
}

void jni_UnlockAndroidSurface(vlc_object_t *p_vout)
{
    vlc_jni_player_t *vj = vlc_jni_player_find_by_vout(p_vout);
    if (vj)
        vlc_mutex_unlock(&vj->surface_lock);
}

void jni_SetAndroidSurfaceSize(vlc_object_t *p_vout, int width, int height)
{
}

#endif

