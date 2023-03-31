#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore = NULL;

void Task1(void *pvParameters) {
    printf("count:");
    xSemaphore = xSemaphoreCreateMutex();
    vTaskDelete(NULL);
}

void Task2(void *pvParameters) {
    int i = 1;
    while(i > 0) {
        printf(" %d \n", i++);
        if( xSemaphore != NULL ) {
            while(i > 0) {
                printf(" %d \n", i++);
                xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE );
            }
        }        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void) {
    xTaskCreate( Task1, "Task 1", 2500, NULL, 1, NULL );
    xTaskCreate( Task2, "Task 2", 2500, NULL, 2, NULL );
}
