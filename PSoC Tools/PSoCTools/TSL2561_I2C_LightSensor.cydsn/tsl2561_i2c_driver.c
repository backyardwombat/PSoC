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
/* ========================================
 * Provides the SMBus Write Byte via I2C to the TSL2561 
 * ========================================
*/
#include "tsl2561_i2c_driver.h"

/*******************************************************************************
* Function Name: Init
****************************************************************************//**
*  Initialize the TSL2561 Timing, Threshold and Interrupt registers.
*
*  \param address: 7-bit slave address.
*  \param *config: Structure defining the configuration of the sensor.
*
* \return
*  Error status.
*  - Returns the error status directly from the I2C component. See the I2C 
*    datasheet for details for I2CMasterWriteBuf().
*******************************************************************************/
uint32 TSL2561_Init(uint8 address, const tsl2561_config *config)
{
    uint32 status = TRANSFER_ERROR;
    
    /* Set the timing register */
    TSL2561_WriteByte(address, (COMMAND_CMD_MSK | TIMING_REG), config->timing);
    while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT));
    status = I2C_I2CMasterStatus();
    
    /* Set the threshold low low register */
    TSL2561_WriteByte(address, (COMMAND_CMD_MSK | THRESHLOWLOW_REG), config->threshLowLow);
    while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT));
    status = I2C_I2CMasterStatus();
    
    /* Set the threshold low high register */
    TSL2561_WriteByte(address, (COMMAND_CMD_MSK | THRESHLOWHIGH_REG), config->threshLowHigh);
    while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT));
    status = I2C_I2CMasterStatus();
    
    I2C_I2CMasterClearStatus();
    
    return status;
}

/*******************************************************************************
* Function Name: WriteByte
****************************************************************************//**

*
*  \param address: 7-bit slave address.
*  \param command: Value sent to the command register. Includes register address.
*  \Param data: Byte to write to the provided register address.
*
* \return
*  Error status.
*  - Returns the error status directly from the I2C component. See the I2C 
*    datasheet for details for I2CMasterWriteBuf().
*******************************************************************************/
uint32 TSL2561_WriteByte(uint8 address, uint8 command, uint8 data)
{
    uint8 buffer[BYTE_COMMAND_SIZE];
    uint32 status = TRANSFER_ERROR;
    
    buffer[0] = command;
    buffer[1] = data;
    status = I2C_I2CMasterWriteBuf(address, buffer, BYTE_COMMAND_SIZE, \
                            I2C_I2C_MODE_COMPLETE_XFER);
    return status;
}

/*******************************************************************************
* Function Name: SendReadCmd
****************************************************************************//**

*
*  \param address: 7-bit slave address.
*  \param command: Value sent to the command register. Includes register address.
*  \Param data: Word to read from the provided register address.
*
* \return
*  Error status.
*  - Returns the error status directly from the I2C component. See the I2C 
*    datasheet for details for I2CMasterReadBuf().
*******************************************************************************/
uint32 TSL2561_SendCmd(uint8 address, uint8 command)
{
    uint8 buffer[BYTE_SIZE];
    uint32 status = TRANSFER_ERROR;
    
    buffer[0] = command;
    status = I2C_I2CMasterWriteBuf(address, buffer, BYTE_SIZE, \
                            I2C_I2C_MODE_COMPLETE_XFER);
    
    return status;
}

/*******************************************************************************
* Function Name: ReadWord
****************************************************************************//**

*
*  \param address: 7-bit slave address.
*  \param command: Value sent to the command register. Includes register address.
*  \Param data: Word to read from the provided register address.
*
* \return
*  Error status.
*  - Returns the error status directly from the I2C component. See the I2C 
*    datasheet for details for I2CMasterReadBuf().
*******************************************************************************/
uint32 TSL2561_ReadData(uint8 address, uint32 size, uint8 * readBuffer)
{
    uint8 buffer[BYTE_COMMAND_SIZE];
    uint32 status = TRANSFER_ERROR;
       
    status = I2C_I2CMasterReadBuf(address, readBuffer, size, I2C_I2C_MODE_COMPLETE_XFER);
    
    return status;
}

/* [] END OF FILE */
