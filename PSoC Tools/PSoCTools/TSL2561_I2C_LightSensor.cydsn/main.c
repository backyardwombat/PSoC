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
#include "tsl2561_i2c_driver.h"
#include "common.h"

#define READ_WORD_TEST 0

int main(void)
{
    uint32 i2cStatus;    
    uint resultZero;
    uint resultOne;
    char resultString[5];
    #if(READ_WORD_TEST)
        uint8 dataZero[2];
        uint8 dataOne[2];
    #else
        uint8 data_block[5];
    #endif
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    #if (DEBUG_UART_ENABLED == ENABLED)
	    UART_DEB_Start();
    #endif /* (DEBUG_UART_ENABLED == ENABLED) */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Start();
    I2C_EnableInt();    
    
    /******************* Disable the TSL2561 ************************/
    #if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_UartPutString("Disabling Sensor \r\n");
    #endif
    i2cStatus = TSL2561_WriteByte(ADDRESS_FLOAT, (COMMAND_CMD_MSK | CONTROL_REG), CONTROL_POWER_OFF);
    i2cStatus = TSL2561_WaitForWriteComplete();
    
    CyDelay(50);
    
    /******************* Enable the TSL2561 *************************/
    #if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_UartPutString("Enabling Sensor \r\n");
    #endif
    i2cStatus = TSL2561_WriteByte(ADDRESS_FLOAT, (COMMAND_CMD_MSK | CONTROL_REG), CONTROL_POWER_ON);
    i2cStatus = TSL2561_WaitForWriteComplete();

    
    for(;;)
    {
        CyDelay(500);   //Wait 500 ms between reads
        
        /************ READ WORD ***********************/
        #if (READ_WORD_TEST)
        /************ READ WORDS ***********************/
        TSL2561_SendCmd(ADDRESS_FLOAT, (COMMAND_READ_WORD | DATA0LOW_REG));
        
        /* Waits until master completes write transfer */
        while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT));
        i2cStatus = I2C_I2CMasterStatus();
        I2C_I2CMasterClearStatus();
    
        TSL2561_ReadData(ADDRESS_FLOAT, WORD_SIZE, dataZero);
        resultZero = dataZero[0];
        resultZero |= (dataZero[1] << 8);
        
        TSL2561_SendCmd(ADDRESS_FLOAT, (COMMAND_READ_WORD | DATA1LOW_REG));
        
        /* Waits until master completes write transfer */
        while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT));
        i2cStatus = I2C_I2CMasterStatus();
        I2C_I2CMasterClearStatus();
        
        TSL2561_ReadData(ADDRESS_FLOAT, WORD_SIZE, dataOne);
        resultOne = dataOne[0];
        resultOne |= (dataOne[1] << 8);
        #else
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
        
        resultZero = data_block[1];
        resultZero |= (data_block[2] << 8);
        resultOne = data_block[3];
        resultOne |= (data_block[4] << 8);
       
        #endif
        #if (DEBUG_UART_ENABLED == ENABLED)    
        sprintf(resultString, "%d", resultZero);
        UART_DEB_UartPutString("Data 0 : ");
        UART_DEB_UartPutString(resultString);
        UART_DEB_UartPutString("\n\r");
        
        sprintf(resultString, "%d", resultOne);
        UART_DEB_UartPutString("Data 1 : ");
        UART_DEB_UartPutString(resultString);
        UART_DEB_UartPutString("\n\r");
        #endif
    }
}

/* [] END OF FILE */
