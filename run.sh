#!/bin/bash

qemu-system-i386 -drive format=raw,file=sd.img,index=0,media=disk -serial stdio