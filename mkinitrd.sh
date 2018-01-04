#!/bin/bash
echo "Making initrd.img..."
rm -f initrd.img
arm-none-eabi-ar r initrd.img initrd/*
