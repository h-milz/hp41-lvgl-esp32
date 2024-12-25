/*
 * hello world
 */
 
#include <string.h>
#include <stdlib.h>
#include <stdio.h> 
#include <dirent.h> 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_eap_client.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_spiffs.h"
#include "esp_system.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "wear_levelling.h"

#define PARTITION_LABEL "storage"
#define MAXLEN 256

static const char *TAG = "hp41";
static const char *filename = "/storage/hello.txt";

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
// Mount path for the partition
const char *base_path = "/storage";
char data[MAXLEN];

static esp_err_t s_example_write_file(const char *path, char *data)
{
    ESP_LOGI(TAG, "Opening file");
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, data);
    fclose(f);
    ESP_LOGI(TAG, "File written");

    return ESP_OK;
}       


int list_dir(const char *dirname) 
{ 
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir(dirname); 
    ESP_LOGI (TAG, "Trying to open directory %s", dirname);   
    
    if (dr == NULL) { // opendir returns NULL if couldn't open directory 
        ESP_LOGE (TAG, "Could not open current directory"); 
        return 0; 
    } else {
        ESP_LOGI (TAG, "Success: mounted %s", dirname);
    }    
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    while ((de = readdir(dr)) != NULL) 
            ESP_LOGI (TAG, "found file %s", de->d_name); 
  
    closedir(dr);     
    return 0; 
} 

void init_spiffs(void) {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = base_path,  // Mount point
        .partition_label = PARTITION_LABEL,
        .max_files = 5,         // Max open files
        .format_if_mount_failed = true // Format if mounting fails
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "SPIFFS partition not found");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition info (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "SPIFFS total: %d, used: %d", total, used);
    }
}


void init_fatfs(void) {
    const char *partition_label = PARTITION_LABEL;
    size_t max_files = 4; // Max number of open files at the same time

    esp_vfs_fat_mount_config_t mount_config = {
        .max_files = max_files,
        .format_if_mount_failed = false // Do not auto-format
    };

    esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(base_path, partition_label, &mount_config, &s_wl_handle);

    if (err == ESP_FAIL) {
        ESP_LOGW(TAG, "FATFS not formatted, attempting to format...");

        // Try to format the partition
        err = esp_vfs_fat_spiflash_format_rw_wl(base_path, partition_label);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Partition successfully formatted and mounted");
        } else {
            ESP_LOGE(TAG, "Failed to format and mount FATFS (%s)", esp_err_to_name(err));
            return;
        }
    } else if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    } else {
        ESP_LOGI(TAG, "FATFS mounted at %s", base_path);
        snprintf(data, MAXLEN, "%s %s\n", "hello world, from ESP-IDF", esp_get_idf_version());
        err = s_example_write_file(filename, data);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "could not write file %s", filename);
            return;
        } else {
            ESP_LOGI(TAG, "wrote file %s", filename);
        }   
        list_dir(base_path);
    }

    
}



static void helloworld_task(void* pvParameters) {

    while (1) {
        ESP_LOGI(TAG, "hello world");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());

    init_spiffs();
    
    snprintf(data, MAXLEN, "%s %s\n", "hello world, from ESP-IDF", esp_get_idf_version());
    esp_err_t err = s_example_write_file(filename, data);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "could not write file %s", filename);
        return;
    } else {
        ESP_LOGI(TAG, "wrote file %s", filename);
    }       
    list_dir(base_path);


    xTaskCreate(&helloworld_task, "helloworld_task", 4096, NULL, 5, NULL);
}


