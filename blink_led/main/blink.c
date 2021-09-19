#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
//#include "module.h"

#define LED_PIN 2
#define TIM_DELAY_LED_MS 1000 
static const char *TAG = "example";


void app_main(void)
{
	//config led
	gpio_pad_select_gpio(LED_PIN);
	gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT);
	//config status led
	uint8_t state_led = 0; 
	//keyboard
	char c = 0;
	char str[100];
	memset(str,0,sizeof(str));
	//imformation chip
	printf("idf version : %s\n",esp_get_idf_version());
	uint8_t chipID[6];
	esp_efuse_mac_get_default(chipID);
	printf("mac/chipID %02x%02x%02x%02x%02x%02x\n\n",chipID[0],chipID[1],chipID[2],chipID[3],chipID[4],chipID[5]);
	ESP_LOGI(TAG, "xPortGetFreeHeapSize = %dk", xPortGetFreeHeapSize());

	int Dram = heap_caps_get_free_size(MALLOC_CAP_8BIT);
	int Iram = heap_caps_get_free_size(MALLOC_CAP_32BIT) - heap_caps_get_free_size(MALLOC_CAP_8BIT);

	ESP_LOGI(TAG, "DRAM \t\t%d",Dram);
	ESP_LOGI(TAG, "IRAM \t\t%d",Iram);
    while(1)
    {
    	//ESP_LOGI(TAG,"Terning led : %s",(state_led == 0) ? "ON" : "OFF");
		//blink led
    	state_led = !state_led;
    	gpio_set_level(LED_PIN,state_led);

		//keyboard
		/*
		c  =getchar();
		if(c != 0xFF)
		{
			str[strlen(str)] = c;
			printf("%c",c);
		}
		*/
    	vTaskDelay(TIM_DELAY_LED_MS / portTICK_PERIOD_MS);
    }
    //printf("you type: %s \n",str);
}