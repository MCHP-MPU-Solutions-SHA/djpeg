#!/bin/bash

if [ ! -d "images" ]; then
    echo "The 'images' directory does not exist. Please check!"
    exit 1
fi

if [ ! -e "jpeg2yuv" ]; then
    echo "The 'jpeg2yuv' file does not exist. Please check!"
    exit 1
fi

EGT_KMS_BUFFERS=2 ./jpeg2yuv
