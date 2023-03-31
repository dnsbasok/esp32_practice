#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore = NULL;

void Task1(void *pvParameters) {
    while(1) { 
        xSemaphoreTake(xSemaphore, portMAX_DELAY); 
        for (int i = 0; i <= 10; i++) {
            printf("H");
            printf("e");
            printf("l");
            printf("l");
            printf("o\n");
        }
        xSemaphoreGive(xSemaphore); 
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    } 
    vTaskDelete(NULL); 
}

void Task2(void *pvParameters) {
    while(1) { 
        xSemaphoreTake(xSemaphore, portMAX_DELAY); 
        for (int i = 0; i <= 10; i++) {
            printf("W");
            printf("o");
            printf("r");
            printf("l");
            printf("d\n");
        }
        xSemaphoreGive(xSemaphore); 
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    } 
    vTaskDelete(NULL);
}

void app_main(void) {
    /* Create a mutex type semaphore. */
    xSemaphore = xSemaphoreCreateMutex();
    if( xSemaphore != NULL ) {
       /* The semaphore was created successfully and
       can be used. */
    }

    xTaskCreate( Task1, "Task 1", 2500, NULL, 1, NULL );
    xTaskCreate( Task2, "Task 2", 2500, NULL, 1, NULL );
}
