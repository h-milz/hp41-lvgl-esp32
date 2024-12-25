#!/bin/bash

pushd $HOME/local

if [ -d esp-idf ] ; then
    cd esp-idf
    git checkout v5.2.2  
    cd ..
else
    git clone -b v5.2.2 --recursive https://github.com/espressif/esp-idf.git
fi
    
cd esp-idf
git submodule update --init --recursive
./install.sh 
cd ..

if [ -d lvgl ] ; then
    cd lvgl 
    git checkout v9.2.2
    cd ..
else
    git clone -b v9.2.2 --recursive git@github.com:lvgl/lvgl.git
fi

if [ -d nonpareil ] ; then
    cd nonpareil
    git pull
    cd ..
else
    git clone git@github.com:brouhaha/nonpareil.git
fi

popd # we're back 
. ../esp-idf/export.sh 
idf.py set-target esp32s3
idf.py -p /dev/ttyUSB0 erase-flash flash
esptool.py --chip esp32s3 write_flash 0x1E0000 image.bin
idf.py -p /dev/ttyUSB0 monitor




exit


$ cat partitions.csv 
# Name,   Type, SubType, Offset,  Size
nvs,      data, nvs,     0x9000,  0x5000
phy_init, data, phy,     0xe000,  0x1000
factory,  app,  factory, 0x10000, 0x1E0000
storage,  data, spiffs,  0x200000,0x200000
 2751  idf.py -p /dev/ttyUSB0 flash 
 2752  /home/hm/local/esp-idf/components/spiffs/spiffsgen.py 0x200000 main image.bin
 2753  esptool.py --chip esp32s3 write_flash 0x200000 image.bin
 2754  idf.py monitor




