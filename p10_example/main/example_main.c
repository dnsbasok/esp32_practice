#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"

#define GPIO_LED_NUM 2 

// Определение ручки таймера мигания светодиода
TimerHandle_t LED_Timer_Handle;
// Объявление функции обратного вызова таймера
void LED_Timer_Callback(TimerHandle_t xTimer);

void app_main(void)
{
    BaseType_t ret = 0;

    // Определяем структуру конфигурации gpio
    gpio_config_t gpio_config_structure;

    // Инициализируем структуру конфигурации gpio
    gpio_config_structure.pin_bit_mask = (1ULL << GPIO_LED_NUM);// выбираем gpio2
    gpio_config_structure.mode = GPIO_MODE_OUTPUT;              // Режим вывода
    gpio_config_structure.pull_up_en = 0;                       // Нет подтягивания
    gpio_config_structure.pull_down_en = 0;                     // Нет опускания
    gpio_config_structure.intr_type = GPIO_PIN_INTR_DISABLE;    // Отключить прерывание 

    // Инициализировать и включить в соответствии с заданными параметрами  
	gpio_config(&gpio_config_structure);

    // Вывод высокого уровня, загорается светодиод
    gpio_set_level(GPIO_LED_NUM, 1);

    LED_Timer_Handle = xTimerCreate((const char*)"LED Timer",                   // Название программного таймера
						            (TickType_t )(1000 / portTICK_PERIOD_MS),   // Временной период, единица измерения - тактовая частота
							        (UBaseType_t)pdTRUE,                        // Режим таймера, будь то периодический режим таймера
							        (void*)1,                                   // ID-номер таймера
							        (TimerCallbackFunction_t)LED_Timer_Callback);// Функция обратного вызова таймера
    if((LED_Timer_Handle != NULL))
        ret = xTimerStart(LED_Timer_Handle, 0);	    // Создано успешно, запускаем таймер      
}

// Функция обратного вызова таймера
void LED_Timer_Callback(TimerHandle_t xTimer)
{
    static int led_flag = 0;
    led_flag = !led_flag;                   // переворот уровня светодиода   
    gpio_set_level(GPIO_LED_NUM, led_flag); // Установите уровень светодиода в соответствии с led_flag, чтобы светодиод мигал
}