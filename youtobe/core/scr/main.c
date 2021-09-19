#include <stdio.h>

#include "main.h"
#include "esp_log.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE

static char *TAG = "main"





extern "C" void app_main(void)
{
    while(1)
    {
    ESP_LOGI(TAG,"hello world !\n");
    vTaskDelay(1);
    }
}