#!/bin/bash

# prepare the repo and its submodules

ESP_IDF="git@github.com:espressif/esp-idf.git"
IDF_VER="v5.3.2"

LVGL="git@github.com:lvgl/lvgl.git"
LVGL_VER="v9.2.2"

NONPAREIL="git@github.com:brouhaha/nonpareil.git"
# only master seems to be usable, no versioned branches (yet)

mkdir -p lib
cd lib

# clone the submodules
git clone -b ${IDF_VER} ${ESP_IDF} esp-idf
# we will add required components later to keep things tidy
git clone -b ${LVGL_VER} ${LVGL} lvgl
git clone ${NONPAREIL} nonpareil

cd .. 

# add submodules
git submodule add ${ESP_IDF} lib/esp-idf/
git submodule add ${LVGL} lib/lvgl/
git submodule add ${NONPAREIL} lib/nonpareil/

# make permanent
#git add lib/esp-idf/ lib/lvgl/ lib/nonpareil/ 
#git commit -m "Added submodules"
#git push origin main


