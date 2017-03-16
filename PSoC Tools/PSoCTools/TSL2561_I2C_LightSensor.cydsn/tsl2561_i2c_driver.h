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

uint32 TSL2561_WriteByte(uint8 address, uint8 command, uint8 data);
uint32 TSL2561_ReadWord(uint8 address, uint8 command, uint8 * readBuffer);

/* Addresses based on ADDR SEL pin of the TSL2561 Light Sensor */
#define ADDRESS_GND             (0x29)
#define ADDRESS_FLOAT           (0x39)
#define ADDRESS_VDD             (0x49)

/* Packet sizes */
#define BYTE_SIZE               (1)
#define WORD_SIZE               (2)
#define BYTE_COMMAND_SIZE       (2)
#define WORD_COMMAND_SIZE       (3)

/* Command valid status */
#define TRANSFER_CMPLT          (0x00u)
#define TRANSFER_ERROR          (0xFFu)

/* Registers */
#define CONTROL_REG             (0x0)
#define TIMING_REG              (0x1)
#define THRESHLOWLOW_REG        (0x2)
#define THRESHLOWHIGH_REG       (0x3)
#define THRESHHIGHLOW_REG       (0x4)
#define THRESHHIGHGHIGH_REG     (0x5)
#define INTERRUPT_REG           (0x6)
#define ID_REG                  (0xA)
#define DATA0LOW_REG            (0xC)
#define DATA0HIGH_REG           (0xD)
#define DATA1LOW_REG            (0xE)
#define DATA2HIGH_REG           (0xF)

/* Command Register */
#define COMMAND_CMD_MSK         (0x80)
#define COMMAND_CLEAR_INT_MSK   (0x40)
#define COMMAND_WORD_MSK        (0x20)
#define COMMAND_BLOCK_MSK       (0x10)

/* Control Register */
#define CONTROL_POWER_MSK       (0x03)
#define CONTROL_POWER_ON        (0x03)
#define CONTROL_POWER_OFF       (0x00)

/* Timing Register */
#define TIMING_GAIN_MSK         (0x10)
#define TIMING_MANUAL_MSK       (0x08)
#define TIMING_INTEG_MSK        (0x03)
#define TIMING_INTEG_13_MS      (0x00)
#define TIMING_INTEG_101_MS     (0x01)
#define TIMING_INTEG_401_MS     (0x02)

/* Interrupt Control Register */
#define INTERRUPT_CONTROL_MSK   (0x30)
#define INTERRUPT_CONTROL_SHFT  (0x04)

#define INTERRUPT_DISABLE       (0x00)
#define INTERRUPT_LEVEL         (0x01)
#define INTERRUPT_SMB_ALERT     (0x02)
#define INTERRUPT_TEST_MODE     (0x03)

#define mWriteByte(X,Y) {I2C_I2CMasterWriteBuf(X, Y, BYTE_SIZE, \
                            I2C_I2C_MODE_COMPLETE_XFER)};
/* [] END OF FILE */
