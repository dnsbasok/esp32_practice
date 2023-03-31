#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void Task1(void *pvParameters) {
    for (int i = 0; i <= 10; i++) {
        printf("Task1: %d \n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task2(void *pvParameters) {
    for (int i = 0; i <= 10; i++) {
        printf("Task2: %d \n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void) {
    xTaskCreate( Task1, "Task 1", 2500, NULL, 1, NULL );
    xTaskCreate( Task2, "Task 2", 2500, NULL, 1, NULL );
}


//---------------------
//-- ------------------

void Task1(void *pvParameters) {
    printf("count:");
    vTaskDelete(NULL); 
}

void Task2(void *pvParameters) {
    for (int i = 1; i<100; i++) {
        printf(" %d \n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void) {
    xTaskCreate( Task1, "Task 1", 2500, NULL, 1, NULL );
    xTaskCreate( Task2, "Task 2", 2500, NULL, 2, NULL );
}
