/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

/* Addresses based on ADDR SEL pin of the TSL2561 Light Sensor */
#define ADDRESS_GND             (0x29)
#define ADDRESS_FLOAT           (0x39)
#define ADDRESS_VDD             (0x49)

/* Packet sizes */
#define BYTE_SIZE               (1)
#define WORD_SIZE               (2)

/* Command valid status */
#define TRANSFER_CMPLT          (0x00u)
#define TRANSFER_ERROR          (0xFFu)
/* [] END OF FILE */
