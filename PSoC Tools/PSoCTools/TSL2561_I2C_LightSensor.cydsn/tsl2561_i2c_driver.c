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
/* ========================================
 * Provides the SMBus Write Byte via I2C to the TSL2561 
 * ========================================
*/
#include "tsl2561_i2c_driver.h"

/*******************************************************************************
* Function Name: TSL2561_Init
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
* Function Name: TSL2561_WriteByte
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
* Function Name: TSL2561_SendCmd
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
* Function Name: TSL2561_ReadData
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

/*******************************************************************************
* Function Name: TSL2561_WaitForWriteComplete
****************************************************************************//**
* This is a blocking function that waits for an I2C write to the sensor to
* complete.
*
* \return
*  Error status.
*  - Returns the error status directly from the I2C component. See the I2C 
*    datasheet for details for I2CMasterReadBuf(). Note: if the I2C times out
*    this function returns the error mask so it will appear that every I2C
*    error condition has been hit.
*******************************************************************************/
uint32 TSL2561_WaitForWriteComplete(void)
{
    uint32 status;
    uint32 timeout = TSL2561_I2C_TIMEOUT_INTERVAL;
    char statusMessage[5];

    /* Waits until master completes write transfer */
    while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT) && (0u != timeout))
    {
        timeout--;
    }
    status = I2C_I2CMasterStatus();
    if(I2C_I2C_MSTAT_WR_CMPLT != (status & I2C_I2C_MSTAT_WR_CMPLT))
    {
        status = I2C_I2C_MSTAT_ERR_MASK;
    }
    
    I2C_I2CMasterClearStatus();
    return status;
}
/* [] END OF FILE */
