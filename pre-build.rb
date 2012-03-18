#!/usr/bin/env ruby

abi = `cat jni/Application.mk | grep ^APP_ABI | cut -d' ' -f3`.strip!
flag = `cat jni/Application.mk | grep "^OPT_CFLAGS +="`.split("\n")
no_neon = abi != 'armeabi-v7a'
if abi == 'armeabi-v7a'
    flag = flag[0]
    temp = flag.scan(/-mfpu=([^\s]+)/)
    fpu = temp[0][0].to_s
    no_neon = true if fpu != 'neon'
end
all = Array.new
list = `find . -name Android.mk`.split("\n")
list.each { |l|
    next if ((l =~ /^\.\/jni\/vlc\/modules/) == nil)
    temp = l.scan(/modules\/([^\/]+)\//)
    next if temp == nil || temp.size == 0
    File.open(l) { |f|
        while !f.eof?
            ln = f.readline
            next if ((ln =~ /^LOCAL_MODULE/) == nil)
            temp = ln.scan(/\s*LOCAL_MODULE\s*:=\s*([^\s]+)/)
            next if temp == nil || temp.size == 0
            name = temp[0][0].to_s
            name = name[3..-1] if (name =~ /^lib/) != nil
            next if no_neon && (name =~ /_neon_plugin$/) != nil
            all.push(name)
        end
    }
}
all.sort!
jni_h_old = `cat jni/vlc/src/libvlcjni.h`
jni_h_new = ''
jni_h_new += "/* auto generated */\n"
all.each { |m|
    a = m.sub(/_plugin$/, '')
    jni_h_new += "vlc_declare_plugin(#{a});\n"
}
jni_h_new += "const void *vlc_builtins_modules[] = {\n"
all.each { |m|
    a = m.sub(/_plugin$/, '')
    jni_h_new += "\tvlc_plugin(#{a}),\n"
}
jni_h_new += "\tNULL\n"
jni_h_new += "};\n"
jni_h_new += "/* auto generated */\n"
if jni_h_old != jni_h_new
    f = File.open('jni/vlc/src/libvlcjni.h', 'w') { |f|
        f.write(jni_h_new)
    }
end
n = `grep -n '# modules' jni/vlc/Modules.mk | cut -d: -f1`
n = n.to_i + 1
modules_old = `sed -n #{n}p jni/vlc/Modules.mk`.strip!
modules_new = 'LOCAL_STATIC_LIBRARIES += ' + all.join(' ')
if modules_old != modules_new
    `sed -i "#{n} c\\#{modules_new}" jni/vlc/Modules.mk`
end

