#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static TaskHandle_t receiveHandle = NULL;

static void sender(void *params)
{
	while (true)
	{
		xTaskNotifyGive(receiveHandle);
		xTaskNotifyGive(receiveHandle);

		xTaskNotifyGive(receiveHandle);
		xTaskNotifyGive(receiveHandle);
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}
static void receive(void *params)
{
	while(true)
	{
		int count = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		printf("receive notification %d time\n",count);
	}
}


void app_main(void)
{
	xTaskCreate(&receive, "sender", 2048, NULL, 2, &receiveHandle);
	xTaskCreate(&sender, "receive", 2048, NULL, 2, NULL);
	
	
}
