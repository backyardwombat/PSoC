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
#include "tsl2561_i2c_driver.h"
#include "common.h"

uint32 _WaitForWriteComplete(uint mode)
{
    uint32 status;
    uint32 timeout = I2C_TIMEOUT_INTERVAL;
    char statusMessage[5];
    
    if(1u == mode)
    {
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
    }
    #if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_UartPutString(statusMessage);
    UART_DEB_UartPutString("\n\r");
    #endif
    
    I2C_I2CMasterClearStatus();
    return status;
}

uint32 _WaitForReadComplete(uint mode)
{
    uint32 status;
    uint32 timeout = I2C_TIMEOUT_INTERVAL;
    char statusMessage[5];
    
    if(1u == mode)
    {
        /* Waits until master completes write transfer */
        while (0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT) && (0u != timeout))
        {
            #if (DEBUG_UART_ENABLED == ENABLED)
                if(0u == (timeout % DEBUG_WAIT_UPDATE_INTERVAL))
                {
                    UART_DEB_UartPutString("Waiting for Read Complete \r\n");
                }
            #endif
            timeout--;
        }
        status = I2C_I2CMasterStatus();
        sprintf(statusMessage, "%08lx", status);
        if(I2C_I2C_MSTAT_RD_CMPLT == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT))
        {
            /* Write complete */
            #if (DEBUG_UART_ENABLED == ENABLED)
            UART_DEB_UartPutString("Read Complete, status = "); 
            #endif
        }
        else
        {
            /* I2C timeout */
            #if (DEBUG_UART_ENABLED == ENABLED)
            UART_DEB_UartPutString("I2C Timeout, status = ");
            #endif
        }
    }
    #if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_UartPutString(statusMessage);
    UART_DEB_UartPutString("\n\r");
    #endif
    
    I2C_I2CMasterClearStatus();
    return status;
}

int main(void)
{
    uint32 i2cStatus;
    uint8 dataZero[2];
    uint8 dataOne[2];
    uint resultZero;
    uint resultOne;
    uint8 commandBuffer[1];
    char resultString[5];
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    #if (DEBUG_UART_ENABLED == ENABLED)
	    UART_DEB_Start();
    #endif /* (DEBUG_UART_ENABLED == ENABLED) */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Start();
    
    /******************* Disable the TSL2561 ************************/
    #if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_UartPutString("Disabling Sensor \r\n");
    #endif
    i2cStatus = TSL2561_WriteByte(ADDRESS_FLOAT, (COMMAND_CMD_MSK | CONTROL_REG), CONTROL_POWER_OFF);
    i2cStatus = _WaitForWriteComplete(1);
    
    CyDelay(50);
    
    /******************* Enable the TSL2561 *************************/
    #if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_UartPutString("Enabling Sensor \r\n");
    #endif
    i2cStatus = TSL2561_WriteByte(ADDRESS_FLOAT, (COMMAND_CMD_MSK | CONTROL_REG), CONTROL_POWER_ON);
    i2cStatus = _WaitForWriteComplete(1);

    
    for(;;)
    {
        CyDelay(500);   //Wait 500 ms
        TSL2561_ReadWord(ADDRESS_FLOAT, (uint8) (COMMAND_CMD_MSK | COMMAND_WORD_MSK | DATA0LOW_REG), dataZero);
        resultZero = dataZero[0];
        resultZero |= (dataZero[1] << 8);
        
        TSL2561_ReadWord(ADDRESS_FLOAT, (uint8) (COMMAND_CMD_MSK | COMMAND_WORD_MSK | DATA1LOW_REG), dataOne);
        resultOne = dataOne[0];
        resultOne |= (dataOne[1] << 8);
        
        //i2cStatus = TSL2561_ReadWord(ADDRESS_FLOAT, (COMMAND_CMD_MSK | COMMAND_WORD_MSK | DATA0LOW_REG), dataZero);
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
