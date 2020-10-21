/*
 * Copyright 2020, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software"), is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file spi_master.c
 *
 * @brief
 * sample application for SPI master
 *
 * This application demonstrates how to use SPI driver interface to send and
 * receive bytes or a stream of bytes over the SPI hardware as a master.
 *
 * Features demonstrated:
 * - SPI WICED APIs
 * - WICED RTOS APIs
 *
 * Requirements and Usage:
 * Program 1 kit with the spi_master app and another kit with
 * spi_slave app. Connect the SPI lines and ground on the 2 kits.
 * Power on the kits. You can see the logs through PUART
 *
 * Hardware Connections:
 *
 * This snip example configures the following SPI functionalities in
 * CYW920719B2Q40EVB-01 Evaluation board as follows:
 *
 * CLK     WICED_P38    D13
 * MISO    WICED_P01    D12
 * MOSI    WICED_P04    D7
 * CS      WICED_P07    D10
 * GND
 *
 */

/******************************************************************************
 *                                Includes
 ******************************************************************************/
#include "sparcommon.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_trace.h"
#include "wiced_hal_gpio.h"
#include "wiced_platform.h"
#include "wiced_hal_pspi.h"
#include "wiced_hal_puart.h"
#include "wiced_rtos.h"
#include "wiced_bt_stack.h"
#include "GeneratedSource/cycfg_pins.h"

/******************************************************************************
 *                                Constants
 ******************************************************************************/
/* Threads defines */
/* Sensible stack size for most threads*/
#define THREAD_STACK_MIN_SIZE                 (1024)
/* Defining thread priority levels*/
#define PRIORITY_MEDIUM                       (5)

#define DEFAULT_FREQUENCY                     (1000000u)

/* Header for SPI data packet ensures the SPI sensor connections are intact*/
#define PACKET_HEADER                         (0xC819)
/* Manufacturer ID denoting Cypress Semiconductor*/
#define MANUFACTURER_ID                       (0x000A)
/* Unit ID denoting temperature is in Celsius scale*/
#define UNIT_ID                               (0x000B)
/* Master interrogates sensor every 1 s for temperature reading*/
#define SLEEP_TIMEOUT                         (1000)
/* Delay between transmitting and receiving SPI messages from sensor, to prevent
 * reading earlier responses.*/
#define TX_RX_TIMEOUT                         (50)

/*To reset SPI master handling sensor when wrong data is sent repeatedly*/
#define MAX_RETRIES                           (5)
/* Resetting retry count variable to 0 when valid data is received.*/
#define RESET_COUNT                           (0)
/* Temperature data is received as 16 bit integer, the decimal and fractional
 * parts of temperature can be obtained from the quotient and remainder when the
 * temperature data is divided by 100*/
#define NORM_FACTOR                           (100)


/******************************************************************************
 *                                Structures
 ******************************************************************************/

/* pSPI data packet*/
typedef struct
{
    int16_t data ;
    uint16_t header;
}data_packet;

/* Enumeration listing SPI sensor commands
 * GET_MANUFACTURER_ID: Command to get Manufacturer ID.
 * GET_UNIT: Command to get unit scale.
 * MEASURE_TEMPERATURE: Command to get temperature reading.*/
typedef enum
{
    GET_MANUFACTURER_ID = 0x01,
    GET_UNIT,
    MEASURE_TEMPERATURE
}sensor_cmd;

/* Enumeration listing SPI Master states
 * SENSOR_DETECT: Master checks for presence of SLAVE by verifying received
 *                packet header and obtains response to Manufacturer ID on
 *                verification.
 * GET_UNIT: Master checks for presence of SLAVE by verifying received packet
 *           header and obtains response to Unit ID on verification.
 * GET_TEMPERATURE: Master checks for presence of SLAVE by verifying received
 *                  packet header and obtains response to Temperature reading on
 *                  verification.*/
typedef enum
{
    SENSOR_DETECT,
    READ_UNIT,
    READ_TEMPERATURE
}master_state;

/******************************************************************************
 *                                Variables Definitions
 ******************************************************************************/

static wiced_thread_t       *spi_1;

/******************************************************************************
 *                                Function Prototypes
 ******************************************************************************/

wiced_result_t bt_cback( wiced_bt_management_evt_t event,
                         wiced_bt_management_evt_data_t *p_event_data );
void           initialize_app( void );
static void    spi_sensor_thread( uint32_t arg);
void           spi_sensor_utility (data_packet *send_msg, data_packet *rec_msg);

/**
 Function name:
 application_start

 Function Description:
 @brief    Starting point of your application. Entry point to the application.
           Set device configuration and start BT stack initialization.
           The actual application initialization will happen when stack reports
           that BT device is ready.

 @param  void

 @return void
 */

void application_start(void)
{
    wiced_set_debug_uart( WICED_ROUTE_DEBUG_TO_PUART );
    if(WICED_BT_SUCCESS != wiced_bt_stack_init( bt_cback, NULL, NULL ))
    {
        WICED_BT_TRACE("BT stack initialization failed \n\r");
    }
}

/**
 Function name:
 wiced_result_t bt_cback(wiced_bt_management_evt_t event,
*                                  wiced_bt_management_evt_data_t *p_event_data)

 Function Description:
 @brief     This is a BlueTooth management event handler function to receive
            events from BLE stack and process as per the application.

 @param  event          BLE event code of one byte length
 @param  *p_event_data  Pointer to BLE management event structures

 @return wiced_result_t  Error code from WICED_RESULT_LIST or BT_RESULT_LIST
 */

wiced_result_t bt_cback( wiced_bt_management_evt_t event,
                         wiced_bt_management_evt_data_t *p_event_data )
{
    wiced_result_t result = WICED_BT_SUCCESS;

    switch( event )
    {
    /* BlueTooth stack enabled*/
    case BTM_ENABLED_EVT:

        WICED_BT_TRACE("\n\rSample SPI Master Application\n\n\r");

        initialize_app();

        break;

    default:
        break;
    }
    return result;
}

/**
 Function name:
 initialize_app

 Function Description:
 @brief    This functions initializes the SPI Slave

 @param void
 @return void
 */

void initialize_app( void )
{
    wiced_hal_pspi_init(SPI1,
                        DEFAULT_FREQUENCY,
                        SPI_LSB_FIRST,
                        SPI_SS_ACTIVE_LOW,
                        SPI_MODE_0);

    spi_1 = wiced_rtos_create_thread();
    if ( WICED_SUCCESS == wiced_rtos_init_thread(spi_1,
                                                 PRIORITY_MEDIUM,
                                                 "SPI 1 instance",
                                                 spi_sensor_thread,
                                                 THREAD_STACK_MIN_SIZE,
                                                 NULL ) )
    {
        WICED_BT_TRACE( "SPI Sensor thread created\n\r" );
    }
    else
    {
        WICED_BT_TRACE( "Failed to create SPI Sensor thread \n\r" );
    }

}

/**
 Function name:
 spi_sensor_thread

 Function Description:
 @brief    Starts and maintains transfer of SPI sensor data.

 @param    arg  unused argument

 @return   none
 */

void spi_sensor_thread(uint32_t arg )
{
    data_packet send_data;
    data_packet rec_data;
    wiced_result_t result;
    uint8_t num_retries = RESET_COUNT;
    uint32_t count;
    master_state curr_state = SENSOR_DETECT;
    int8_t dec_temp;
    uint8_t frac_temp;
    WICED_BT_TRACE("Inside SPI Sensor Thread\n\r");

    while(WICED_TRUE)
    {
        switch(curr_state)
        {
        case SENSOR_DETECT:

            /* Configuring send_data data packet to contain command
               GET_MANUFACTURER_ID*/
            send_data.data = GET_MANUFACTURER_ID;
            send_data.header = PACKET_HEADER;
            WICED_BT_TRACE("Sensor detect packet ready\n\r");

            /* This function is responsible for transmitting and receiving SPI
               data. It uses the send_data data packet, configured before, to
               transmit and stores the received data packet to rec_data*/
            spi_sensor_utility(&send_data,&rec_data);
            WICED_BT_TRACE("Data sent \n\r");
            if(PACKET_HEADER == rec_data.header)
            {
                if(MANUFACTURER_ID == rec_data.data)
                {
                    WICED_BT_TRACE("Manufacturer: Cypress Semiconductor\n\r");
                    curr_state = READ_UNIT;
                    num_retries = RESET_COUNT;
                }
                else
                {
                    num_retries++;
                    WICED_BT_TRACE("Unknown manufacturer \n\r");
                }
            }
            else
            {
                WICED_BT_TRACE("Failed to get manufacturer ID\n\r");
                num_retries++;
            }
            break;

        case READ_UNIT:

            /* Configuring send_data data packet to contain command GET_UNIT*/
            send_data.data = GET_UNIT;
            send_data.header = PACKET_HEADER;
            spi_sensor_utility(&send_data,&rec_data);
            if(PACKET_HEADER == rec_data.header)
            {
                if(UNIT_ID == rec_data.data)
                {
                    WICED_BT_TRACE("Unit: Celsius \n\r");
                    curr_state = READ_TEMPERATURE;
                    num_retries = RESET_COUNT;
                }
                else
                {
                    num_retries++;
                    WICED_BT_TRACE("Unknown unit \n\r");
                }
            }
            else
            {
                num_retries++;
            }
            break;

        case READ_TEMPERATURE:

            /* Configuring send_data data packet to contain command
               MEASURE_TEMPERATURE*/
            send_data.data = MEASURE_TEMPERATURE;
            send_data.header = PACKET_HEADER;
            spi_sensor_utility(&send_data,&rec_data);
            if(PACKET_HEADER == rec_data.header)
            {
                num_retries = RESET_COUNT;

                /* The temperature data received is 16 bit integer. Say if
                   temperature is 23.45 Celsius, the received temperature data
                   is 2345. So, to obtain the decimal and fractional parts, the
                   quotient and remainder are found.*/
                dec_temp = (rec_data.data / NORM_FACTOR);

                /* Fractional part cannot be negative */
                frac_temp = ABS(rec_data.data % NORM_FACTOR);
                WICED_BT_TRACE("Temperature Value %d.%d \r\n",
                                dec_temp,frac_temp);

            }
            else
            {
                num_retries++;
            }
            break;

        default:
            break;
        }
        if(num_retries > MAX_RETRIES)
        {
            /* If the number of retries exceeds the maximum, the current
               state is changed to SENSOR_DETECT and the SPI interface is reset.
               This reset resolves clock synchronization issues and ensures the
               data is interpreted correctly.*/
            curr_state = SENSOR_DETECT;
            num_retries = RESET_COUNT;
            wiced_hal_pspi_reset(SPI1);
        }
        wiced_rtos_delay_milliseconds(SLEEP_TIMEOUT, ALLOW_THREAD_TO_SLEEP);
    }
}

/**
 Function name:
 spi_sensor_utility

 Function Description:
 @brief    function that performs SPI transactions with SPI sensor

 @param   *send_msg  pointer to the data packet that is sent.
*@param   *rec_msg   pointer to the data packet that is received.

 @return void
 */

void spi_sensor_utility(data_packet *send_msg,data_packet *rec_msg)
{
    /* Chip select is set to LOW to select the slave for SPI transactions*/
    wiced_hal_gpio_set_pin_output(SPI1_CS, GPIO_PIN_OUTPUT_LOW);

    WICED_BT_TRACE("Sending data to slave\n\r");

    /* Sending command to slave*/
    wiced_hal_pspi_tx_data(SPI1,
                           sizeof(*send_msg),
                           (uint8_t*)send_msg);

    /*Allowing slave time to fill its rx buffers before receiving*/
    wiced_rtos_delay_milliseconds(TX_RX_TIMEOUT,ALLOW_THREAD_TO_SLEEP);

    WICED_BT_TRACE("Receiving data from slave\n\r");

    /* Receving response from slave*/
    wiced_hal_pspi_rx_data(SPI1,
                           sizeof(*rec_msg),
                           (uint8_t*)rec_msg);

    /* Chip select is set to HIGH to unselect the slave for SPI transactions*/
    wiced_hal_gpio_set_pin_output(SPI1_CS, GPIO_PIN_OUTPUT_HIGH);

    return;
}
