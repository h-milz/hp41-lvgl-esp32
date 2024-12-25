# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/hm/local/esp-idf/components/bootloader/subproject"
  "/home/hm/local/hp41-lvgl-esp32/build/bootloader"
  "/home/hm/local/hp41-lvgl-esp32/build/bootloader-prefix"
  "/home/hm/local/hp41-lvgl-esp32/build/bootloader-prefix/tmp"
  "/home/hm/local/hp41-lvgl-esp32/build/bootloader-prefix/src/bootloader-stamp"
  "/home/hm/local/hp41-lvgl-esp32/build/bootloader-prefix/src"
  "/home/hm/local/hp41-lvgl-esp32/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/hm/local/hp41-lvgl-esp32/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/hm/local/hp41-lvgl-esp32/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
