#!/bin/bash

qemu-system-i386 -drive format=raw,file=image.bin,index=0,media=disk \
                 -drive format=raw,file=sd.img,index=1,media=disk -serial stdio