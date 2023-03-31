#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <string.h>
#include "freertos/queue.h"

/**
 * This is an example which echos any data it receives on UART1 back to the sender,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: UART1
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below
 */

#define ECHO_TEST_TXD  (GPIO_NUM_17)
#define ECHO_TEST_RXD  (GPIO_NUM_16)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (1024)

QueueHandle_t xQueue1, xQueue2, xQueue3 ;

typedef enum cmd_t{
    CMD1,
    CMD2,
    CMD3,
    CMD_UNKNOWN,
} cmd_t ;


static void echo_task (void*arg)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 921600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);

    char* data = (char *) malloc(BUF_SIZE) ;
    memset(data, 0, BUF_SIZE) ;
    char* btn1 = "btn1" ;
    char* btn2 = "btn2" ;
    char* btn3 = "btn3" ;


    while (1) {
         int len = uart_read_bytes(UART_NUM_2, data, BUF_SIZE, 1);
        if (len <= 0)
            continue;
        // printf("data:") ;
        // for(int i = 0; i < len; i++) {
        //     printf(" [%c, %x],", data[i], data[i]);
        // }
        // printf("\r\n") ;
        cmd_t cmd = CMD_UNKNOWN;

        if(strcmp ( data, btn1 ) == 0 ) {
            cmd=CMD1 ;
        }
        else if( strcmp ( data, btn2 ) == 0 ) {
            cmd=CMD2 ;
        }
        else if ( strcmp ( data, btn3 ) == 0 ) {
            cmd=CMD3 ;
        }
        
        memset(data, 0, len) ;

        xQueueSend ( xQueue1, &cmd, 0 ) ;   
    }
}

static void Queue_Send_Receive (void*arg)
{
    cmd_t CMD_Receive ;
    for( ; ; ) {   
        xQueueReceive( xQueue1, &CMD_Receive, ( TickType_t ) portMAX_DELAY ) ;
        switch ( CMD_Receive )
        {
            case CMD1:
                // {char* btn1_text = "\r\n1Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?1\r\n\0";
                // while(*btn1_text != '\0')
                // {
                //     uart_write_bytes ( UART_NUM_2, btn1_text, sizeof ( char ) ) ;
                //     uart_wait_tx_done (UART_NUM_2, 100);
                //     btn1_text++;
                // }             
                //uart_wait_tx_done (UART_NUM_2, 100);
                //}

                for(uint32_t i = 0; i < 5; i++)
                {
                    printf("c #1: %d\r\n", i);
                    vTaskDelay(20);
                }
               // }
                break;

            case CMD2:
                {char* btn2_text = "\r\n2Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?2\r\n";
                uart_write_bytes ( UART_NUM_2, btn2_text, strlen ( btn2_text ) ) ;
                uart_wait_tx_done (UART_NUM_2, 100);
                }
                break;

            case CMD3:
                {char* btn3_text = "\r\n3Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?3\r\n";
                uart_write_bytes ( UART_NUM_2, btn3_text, strlen ( btn3_text ) ) ;
                uart_wait_tx_done (UART_NUM_2, 100);
                }
                break;

            case CMD_UNKNOWN:
            {
            char* err_text = "CMD_UNKNOWN\r\n";
            uart_write_bytes ( UART_NUM_2, err_text, strlen ( err_text ) ) ;
            }
            break;
            
            default:
                break;
    
        }
    }

}

void app_main(void)
{
    xQueue1 = xQueueCreate( 10 , sizeof(cmd_t ) ) ;
    xTaskCreate(echo_task, "uart_echo_task", 2024, NULL, 20, NULL) ;
    xTaskCreate(Queue_Send_Receive, "Queue_Send_Receive", 2024, NULL, 10, NULL) ;
}