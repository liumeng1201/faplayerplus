#!/bin/sh

rm -rf libs
ruby pre-build.rb
ndk-build $@ || exit 1
rm -rf assets/lib
ruby post-build.rb
rm -rf bin
ant release
test -f bin/faplayer-unsigned.apk || exit 1
java -jar signapk.jar testkey.x509.pem testkey.pk8 bin/faplayer-unsigned.apk bin/faplayer.apk
mv bin/faplayer.apk bin/faplayer-not-aligned.apk
zipalign -f 4 bin/faplayer-not-aligned.apk bin/faplayer.apk
echo "Done"

