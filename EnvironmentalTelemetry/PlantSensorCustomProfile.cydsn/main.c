/* ========================================
 *
 * Copyright Majellatech, 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF MAJELLATECH
 *
 * ========================================
*/
#include "project.h"
#include "ble_process.h"
#include "common.h"
#include <tsl2561_i2c_driver.h>

/***************************************
*           API Constants
***************************************/
#define LED_ON                      (0u)
#define LED_OFF                     (1u)
#define LED_TIMEOUT                 (1000u/300u)    /* Сounts depend on advertising interval parameter */

/***************************************
*           Globals
***************************************/
uint16 moisture = 0;
uint16 temperature = 0;
uint16 humidity = 0;
uint8 battery = 0;

/*******************************************************************************
* Function Name: AppCallBack()
********************************************************************************
*
* Summary:
*   This is an event callback function to receive events from the BLE Component.
*
*  event - the event code
*  *eventParam - the event parameters
*
* Return:
*   None
*
*******************************************************************************/
void AppCallBack(uint32 event, void* eventParam)
{

    switch (event)
	{
        /**********************************************************
        *                       General Events
        ***********************************************************/
		case CYBLE_EVT_STACK_ON: /* This event is received when the component is Started */
            /* Enter into discoverable mode so that remote can search it. */
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            break;
            
        /**********************************************************
        *                       GAP Events
        ***********************************************************/
        case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
            if(CYBLE_STATE_DISCONNECTED == CyBle_GetState())
            {   
                /* Fast and slow advertising period complete, go to low power  
                 * mode (Hibernate mode) and wait for an external
                 * user event to wake up the device again */
                Advertising_LED_Write(LED_OFF);
                Disconnect_LED_Write(LED_ON);
                LowPower_LED_Write(LED_OFF);
            }
            break;
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            Advertising_LED_Write(LED_OFF);
            break;
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            LowPower_LED_Write(LED_OFF);
            /* Put the device into discoverable mode so that a remote can search it. */
           CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            break;
        default:
			break;
	}
}

void HandleLeds()
{
    static uint32 ledTimer = LED_TIMEOUT;
    static uint8 led = LED_OFF;
    
   /* Blink blue LED to indicate that device is advertising */
    if(CyBle_GetState() == CYBLE_STATE_ADVERTISING)
    {
        if(--ledTimer == 0u) 
        {
            ledTimer = LED_TIMEOUT;
            led ^= LED_OFF;
            Advertising_LED_Write(led);
        }
    }
}

void SpoofMoisture(void)
{
    if(moisture < 0x1000)
    {
        moisture += 0x20;
    }
    else
    {
        moisture = 0x0000;
    }
}

uint32 _WaitForWriteComplete(void)
{
    uint32 status;
    uint32 timeout = I2C_TIMEOUT_INTERVAL;
    char statusMessage[5];

    /* Waits until master completes write transfer */
    while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT) && (0u != timeout))
    {
        #if (DEBUG_UART_ENABLED == ENABLED)
            if(0u == (timeout % DEBUG_WAIT_UPDATE_INTERVAL))
            {
                UART_DEB_UartPutString("Waiting for Write Complete \r\n");
            }
        #endif
        timeout--;
    }
    status = I2C_I2CMasterStatus();
    sprintf(statusMessage, "%08lx", status);
    if(I2C_I2C_MSTAT_WR_CMPLT == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT))
    {
        /* Write complete */
        #if (DEBUG_UART_ENABLED == ENABLED)
        UART_DEB_UartPutString("Write Complete, status = "); 
        #endif
    }
    else
    {
        /* I2C timeout */
        #if (DEBUG_UART_ENABLED == ENABLED)
        UART_DEB_UartPutString("I2C Timeout, status = ");
        #endif
    }

    #if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_UartPutString(statusMessage);
    UART_DEB_UartPutString("\n\r");
    #endif
    
    I2C_I2CMasterClearStatus();
    return status;
}


union TwoBytes
{
    uint16 u16;
    uint8 u8[2];
};

int main(void)
{
    uint8 moist_array[MOISTURE_DATA_CHAR_LEN];
    uint32 i2cStatus;
    uint8 battery = 0x69;
    uint8 data_block[5];
    uint8 ill_result_zero[2];
    uint8 ill_result_one[2];
    
    CYBLE_GATT_HANDLE_VALUE_PAIR_T battHandle;
    battHandle.attrHandle = CYBLE_PLANTBOT_BATTERY_LEVEL_CHAR_HANDLE;
    battHandle.value.val = &battery;
    battHandle.value.len = 1;
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    #if (DEBUG_UART_ENABLED == ENABLED)
	    UART_DEB_Start();
    #endif /* (DEBUG_UART_ENABLED == ENABLED) */
    
    I2C_Start();
    I2C_EnableInt(); 
    
        /******************* Disable the TSL2561 ************************/
    #if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_UartPutString("Disabling Sensor \r\n");
    #endif
    i2cStatus = TSL2561_WriteByte(ADDRESS_FLOAT, (COMMAND_CMD_MSK | CONTROL_REG), CONTROL_POWER_OFF);
    i2cStatus = _WaitForWriteComplete();
    
    CyDelay(50);
    
    /******************* Enable the TSL2561 *************************/
    #if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_UartPutString("Enabling Sensor \r\n");
    #endif
    i2cStatus = TSL2561_WriteByte(ADDRESS_FLOAT, (COMMAND_CMD_MSK | CONTROL_REG), CONTROL_POWER_ON);
    i2cStatus = _WaitForWriteComplete();
    
    /* Using the same LED format as the CY code examples */
    Disconnect_LED_Write(LED_OFF);
    Advertising_LED_Write(LED_OFF);
    LowPower_LED_Write(LED_OFF);

    CyBle_Start(AppCallBack);
    
    for(;;)
    {
        CyBle_GattsWriteAttributeValue(&battHandle, 0x00, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED); 
        
        /* Moisture Measurement */
        SpoofMoisture();
        moist_array[0] = (uint8) (moisture & 0x00FF);
        moist_array[1] = (uint8) ((moisture & 0xFF00) >> 8);
        
        /* Light Measurement */
                /************ READ BLOCK ***********************/
        TSL2561_SendCmd(ADDRESS_FLOAT, COMMAND_READ_BLOCK);
        
        /* Waits until master completes write transfer */
        while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT));
        i2cStatus = I2C_I2CMasterStatus();
        I2C_I2CMasterClearStatus();
        
        TSL2561_ReadData(ADDRESS_FLOAT, BLOCK_SIZE, data_block);
        
        /* Wait until master completes read */
        while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT));
        i2cStatus = I2C_I2CMasterStatus();
        
        ill_result_zero[0] = data_block[1];
        ill_result_zero[1] = (data_block[2] << 8);
        ill_result_one[0] = data_block[3];
        ill_result_one[1] = (data_block[4] << 8);        
        
        UpdateMoistureCharacteristic(ill_result_zero, sizeof(ill_result_zero),CYBLE_PLANTBOT_ILLUMINANCE_CHAR_HANDLE);
        UpdateMoistureCharacteristic(moist_array, sizeof(moist_array),CYBLE_PLANTBOT_SOIL_MOISTURE_CHAR_HANDLE);
        
        HandleLeds();
        CyBle_ProcessEvents();
        CyDelay(50);
    }
}

/* [] END OF FILE */
