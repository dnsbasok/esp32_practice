#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "sdkconfig.h"
#include <string.h>
#include "freertos/timers.h"

#define LED_GPIO GPIO_NUM_2
#define BUF_SIZE (1024)
#define ECHO_TEST_TXD  (GPIO_NUM_17)
#define ECHO_TEST_RXD  (GPIO_NUM_16)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

typedef enum cmd_t {
    CMD_S1,
    CMD_S2,
    CMD_S3,
    CMD_S4,
    CMD_S5,
    CMD_S6,
    CMD_S7,
    CMD_Stop,
    CMD_UNKNOWN,
    CMD_UNKNOWN_STOP,
} cmd_t;

static bool activ_vibro = false;
static bool was_vibro = false;

QueueHandle_t xQueue1;
TimerHandle_t Timer1;
BaseType_t xQueueSendToFront;
int full_vibro_time = 0;

char* S1 = "S1";
char* S2 = "S2";
char* S3 = "S3";
char* S4 = "S4";
char* S5 = "S5";
char* S6 = "S6";
char* S7 = "S7";
char* Stop = "Stop";

static void vibr(void *arg) {
    //printf("%s:%d\n\r", __FUNCTION__, __LINE__);
    if(full_vibro_time >= 5000) {
        activ_vibro = false;  
        was_vibro = false;
        gpio_set_level(GPIO_NUM_2, 0);
        full_vibro_time = 0; 
    }

    static cmd_t CMD_Receive;
    if(!activ_vibro) {
        BaseType_t ret_Queue = xQueueReceive(xQueue1, &CMD_Receive, (TickType_t) 0);
        if(ret_Queue == pdFALSE) {
            return;
        }
    }

    switch(CMD_Receive) {
        case CMD_S1: { 
            activ_vibro = true;
            if(was_vibro == false) {
                gpio_set_level(GPIO_NUM_2, 1); 
                xTimerChangePeriod(Timer1, 100 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 100;
                was_vibro = true;
            }
            else {
                gpio_set_level(GPIO_NUM_2, 0);
                xTimerChangePeriod(Timer1, 100 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 100;
                was_vibro = false;
            }
        }
        break; 

        case CMD_S2: {
            activ_vibro = true;
            if(was_vibro == false) {
                gpio_set_level(GPIO_NUM_2, 1); 
                xTimerChangePeriod(Timer1, 150 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 100;
                was_vibro = true;
            }
            else {
                gpio_set_level(GPIO_NUM_2, 0);
                xTimerChangePeriod(Timer1, 100 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 150;
                was_vibro = false;
            }
        }
        break;

        case CMD_S3: {
            activ_vibro = true;
            if(was_vibro == false) {
                gpio_set_level(GPIO_NUM_2, 1); 
                xTimerChangePeriod(Timer1, 200 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 100;
                was_vibro = true;
            }
            else {
                gpio_set_level(GPIO_NUM_2, 0);
                xTimerChangePeriod(Timer1, 100 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 200;
                was_vibro = false;
            }
        } 
        break;

        case CMD_S4: {
            activ_vibro = true;
            if(was_vibro == false) {
                gpio_set_level(GPIO_NUM_2, 1); 
                xTimerChangePeriod(Timer1, 250 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 100;
                was_vibro = true;
            }
            else {
                gpio_set_level(GPIO_NUM_2, 0);
                xTimerChangePeriod(Timer1, 100 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 250;
                was_vibro = false;
            }
        }
        break;

        case CMD_S5: {
            activ_vibro = true;
            if(was_vibro == false) {
                gpio_set_level(GPIO_NUM_2, 1); 
                xTimerChangePeriod(Timer1, 300 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 100;
                was_vibro = true;
            }
            else {
                gpio_set_level(GPIO_NUM_2, 0);
                xTimerChangePeriod(Timer1, 100 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 300;
                was_vibro = false;
            }
        }
        break;

        case CMD_S6: {
            activ_vibro = true;
            if(was_vibro == false) {
                gpio_set_level(GPIO_NUM_2, 1); 
                xTimerChangePeriod(Timer1, 350 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 100;
                was_vibro = true;
            }
            else {
                gpio_set_level(GPIO_NUM_2, 0);
                xTimerChangePeriod(Timer1, 100 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 350;
                was_vibro = false;
            }
        }
        break;

        case CMD_S7: {
            activ_vibro = true;
            if(was_vibro == false) {
                gpio_set_level(GPIO_NUM_2, 1); 
                xTimerChangePeriod(Timer1, 500 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 100;
                was_vibro = true;
            }
            else {
                gpio_set_level(GPIO_NUM_2, 0);
                xTimerChangePeriod(Timer1, 100 / portTICK_PERIOD_MS, 1);
                full_vibro_time += 500;
                was_vibro = false;
            }
        }
        break;
        
        default:
            break;
    }
    xTimerStart(Timer1, 0);
}

void stop_vibr(void *arg) {
    xQueueReset(xQueue1);
    xTimerStop(Timer1, 0);
    activ_vibro = false;
    was_vibro = true;
    full_vibro_time = 0;
    gpio_set_level(GPIO_NUM_2, 0); 

}

void app_main(void) {
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);

    xQueue1 = xQueueCreate(10, sizeof(cmd_t));
    Timer1= xTimerCreate("Timer", pdMS_TO_TICKS(10), pdFALSE, NULL, vibr);
    char* data = (char *) malloc(BUF_SIZE);
    memset(data, 0, BUF_SIZE);

    for(;;) {
        int len = uart_read_bytes(UART_NUM_2, data, BUF_SIZE, 1);

        if(len == 0) 
            continue;

        cmd_t cmd = CMD_UNKNOWN;
        cmd_t cmd_stop = CMD_UNKNOWN_STOP;
        if(strcmp(data, Stop) == 0 ) { 
            stop_vibr(NULL);
        }
        else if(strcmp(data, S1) == 0) {
            cmd = CMD_S1;
        }
        else if(strcmp(data, S2) == 0) {
            cmd = CMD_S2; 
        }
        else if(strcmp(data, S3) == 0) {
            cmd = CMD_S3; 
        }
        else if(strcmp(data, S4) == 0) {
            cmd = CMD_S4; 
        }
        else if(strcmp(data, S5) == 0) {
            cmd = CMD_S5; 
        }
        else if(strcmp(data, S6) == 0) {
            cmd = CMD_S6; 
        }
        else if(strcmp(data, S7) == 0) {
            cmd = CMD_S7; 
        }
        memset(data, 0, len);
        xQueueSendToFront(xQueue1, (void *) &cmd_stop, 0);
        xQueueSend(xQueue1, (void *) &cmd, 0);
        if(!activ_vibro)
            vibr(NULL);
    }
}