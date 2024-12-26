# hp41-lvgl-esp32
HP41 simulator based on nonpareil and LVGL 

## Installation

Clone the repository and its submodules: 

    git clone git@github.com:h-milz/hp41-lvgl-esp32.git
    cd hp41-lvgl-esp32
    git submodule update --init

## Building

First, be sure to configure the ESP-IDF correctly: 

    idf.py set-target esp32s3
    idf.py menuconfig

and set flash size, partition table, and other values according to your hardware. You will probably at least have to set the following:

    (Top) → Serial flasher config → Flash size
    (Top) → Partition Table → Custom partition table CSV
    (Top) → Component config → ESP PSRAM 
    (Top) → Component config → ESP System Settings → CPU frequency
    (Top) → Component config → Log output

then **S**ave and **Q**uit. 