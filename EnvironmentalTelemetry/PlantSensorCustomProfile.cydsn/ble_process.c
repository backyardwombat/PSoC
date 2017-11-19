/* ========================================
 *
 * Copyright Majellatack, 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your Majellatech.
 *
 * ========================================
*/
#if !defined (BLEPROCESS_H)
#define BLEPROCESS_H
#include <project.h>
    
 
/*******************************************************************************
* Function Name: UpdateLightCharacteristic
********************************************************************************
* Summary:
*        Update the GATT DB with new Light value received.
*
* Parameters:
*  moistData:   array pointer where light value is stored
*  len: 		length of the array
*  attrHandle:	attribute handle of the characteristic
*
* Return:
*  void
*
*******************************************************************************/
void UpdateLightCharacteristic(uint8* lightData, uint8 len, uint16 attrHandle)
{
    CYBLE_GATT_HANDLE_VALUE_PAIR_T lightHandle;
    
    lightHandle.attrHandle = attrHandle;
    lightHandle.value.val = lightData;
    lightHandle.value.len = len;    
    
    //TODO, What should the 2nd argument be? 91162 has it set to FALSE which is defined as zero
    CyBle_GattsWriteAttributeValue(&lightHandle, 0x00, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);  
}

/*******************************************************************************
* Function Name: UpdateMoisturecharacteristic
********************************************************************************
* Summary:
*        Update the GATT DB with new Moisture value received.
*
* Parameters:
*  moistData:   array pointer where moisture value is stored
*  len: 		length of the array
*  attrHandle:	attribute handle of the characteristic
*
* Return:
*  void
*
*******************************************************************************/
void UpdateMoistureCharacteristic(uint8* moistData, uint8 len, uint16 attrHandle)
{
    CYBLE_GATT_HANDLE_VALUE_PAIR_T moistureHandle;
    
    moistureHandle.attrHandle = attrHandle;
    moistureHandle.value.val = moistData;
    moistureHandle.value.len = len;    
    
    //TODO, What should the 2nd argument be? 91162 has it set to FALSE which is defined as zero
    CyBle_GattsWriteAttributeValue(&moistureHandle, 0x00, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);  
}
#endif
/* [] END OF FILE */
