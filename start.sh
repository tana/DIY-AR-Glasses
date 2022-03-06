#!/bin/sh

FBCP=../rpi-fbcp/build/fbcp

# Start fbcp for two framebuffers
$FBCP -p 0 -s /dev/fb1 -l 0 -t 0 -w 240 -h 240 -r 90 -fv &
$FBCP -p 0 -s /dev/fb2 -l 0 -t 0 -w 240 -h 240 -r 270 -fv &
