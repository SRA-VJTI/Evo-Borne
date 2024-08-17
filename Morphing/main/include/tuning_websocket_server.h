#ifndef COMMS_WEBSOCKET_SERVER_H
#define COMMS_WEBSOCKET_SERVER_H

#include <string.h>
#include <fcntl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_spiffs.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mdns.h"
#include "lwip/apps/netbiosns.h"
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_vfs.h"
#include "cJSON.h"
#include "sdkconfig.h"
#include "wifi_handler.h"
#include "websocket_server.h"

#define MDNS_INSTANCE "tuning web server"
#define MDNS_HOST_NAME CONFIG_MDNS_HOST_NAME
#define WEB_MOUNT_POINT "/www"
#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)
#define SCRATCH_BUFSIZE (10240)
#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)


typedef struct comms_val
{
    float test;
    float speed;
    bool front;
    bool back;
    bool right_turn;
    bool left_turn;
    bool val_changed;
} comms_val_t;

comms_val_t read_comms();
void reset_val_changed_coms();
void start_websocket_server();

#endif