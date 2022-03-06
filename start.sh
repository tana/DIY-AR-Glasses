#!/bin/sh

FBCP=$HOME/rpi-fbcp/build/fbcp
AR=$HOME/ar/build/ar

# Wait until framebuffers become available
while [ ! \( -e /dev/fb1 -a -e /dev/fb2 \) ]
do
    sleep 1
done

# Start fbcp for two framebuffers
$FBCP -p 0 -s /dev/fb1 -l 0 -t 0 -w 240 -h 240 -r 90 -fv &
$FBCP -p 0 -s /dev/fb2 -l 0 -t 0 -w 240 -h 240 -r 270 -fv &

exec $AR
