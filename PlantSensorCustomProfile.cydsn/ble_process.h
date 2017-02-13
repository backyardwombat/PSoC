/* ========================================
 *
 * Copyright Maejellatech, 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Maejellatech.
 *
 * ========================================
*/
#if !defined (BLEPROCESS_H)
#define BLEPROCESS_H
#include <project.h>
    
#define MOISTURE_DATA_CHAR_LEN     (0x2)

void UpdateMoistureCharacteristic(uint8* moistData, uint8 len, uint16 attrHandle);

#endif
/* [] END OF FILE */
