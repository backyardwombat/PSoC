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

int main(void)
{
    uint8 moist_array[MOISTURE_DATA_CHAR_LEN];
    
    uint8 battery = 0x69;
    
    CYBLE_GATT_HANDLE_VALUE_PAIR_T battHandle;
    battHandle.attrHandle = CYBLE_PLANTBOT_BATTERY_LEVEL_CHAR_HANDLE;
    battHandle.value.val = &battery;
    battHandle.value.len = 1;
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Using the same LED format as the CY code examples */
    Disconnect_LED_Write(LED_OFF);
    Advertising_LED_Write(LED_OFF);
    LowPower_LED_Write(LED_OFF);

    CyBle_Start(AppCallBack);
    
    for(;;)
    {
        CyBle_GattsWriteAttributeValue(&battHandle, 0x00, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED); 
        
        SpoofMoisture();
        moist_array[0] = (uint8) (moisture & 0x00FF);
        moist_array[1] = (uint8) ((moisture & 0xFF00) >> 8);
        
        UpdateMoistureCharacteristic(moist_array, sizeof(moist_array),CYBLE_PLANTBOT_SOIL_MOISTURE_CHAR_HANDLE);
        
        HandleLeds();
        CyBle_ProcessEvents();
        CyDelay(50);
    }
}

/* [] END OF FILE */
