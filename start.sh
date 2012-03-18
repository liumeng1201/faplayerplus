#!/bin/sh

adb $@ uninstall org.stagex.danmaku
adb $@ install bin/faplayer.apk
adb $@ shell 'am start -a android.intent.action.MAIN -n org.stagex.danmaku/.activity.TestActivity'

