#include "uart.h"

/*
 * Description: Initializes UART.
 * Parameters: UINT32 base - Base address of the UART.
 * Return: UART_ERROR - Result of the operation.
 */
UART_ERROR uart_init(UINT32 base) {
    //Disable UART
    UINT32 control_reg = UART_REG_CTRL(base);
    //Clear TXE and RXE
    control_reg &= ~(UART_REG_CTRL_TXE_MASK | UART_REG_CTRL_RXE_MASK); 
    UART_REG_CTRL(base) = control_reg;

    //Set baud rate
    //Integer part of baud rate
    UART_REG_IBRD(base) = 1;
    //Fractional part of baud rate 
    UART_REG_FBRD(base) = 40; 

    //Configure 8 data bits, no parity, 1 stop bit
    //8 bits, no parity, 1 stop bit
    UART_REG_LCRH(base) = (3 << 5); 

    //Enable UART
    //Set TXE and RXE
    control_reg |= (UART_REG_CTRL_TXE_MASK | UART_REG_CTRL_RXE_MASK);
    UART_REG_CTRL(base) = control_reg;

    return UART_SUCCESS;
}//end of uart_init(...)

/*
 * Description: Sends a character to the UART.
 * Parameters: UINT32 base - Base address of the UART.
 *             char c - Character to send.
 * Return: UART_ERROR - Result of the operation.
 */
UART_ERROR uart_send_char(UINT32 base, char c) {
    //Wait until TXFF is 0
    while (UART_REG_FLAG(base) & UART_REG_FLAG_TXFF_MASK); 
    //Write the character
    UART_REG_DATA(base) = c; 
    return UART_SUCCESS;
}//end of uart_send_char(...)

/*
 * Description: Sends a string to the UART.
 * Parameters: UINT32 base - Base address of the UART.
 *             const char *str - String to send.
 * Return: UART_ERROR - Result of the operation.
 */
UART_ERROR uart_send_string(UINT32 base, const char *str) {
    
    while (*str) {
        UART_ERROR rtn = uart_send_char(base, *str++);
        if (rtn != UART_SUCCESS) {
            return rtn;
        }//end of if (rtn != UART_SUCCESS)
    }//end of while (*str)

    return UART_SUCCESS;

}//end of uart_send_string(...)

/*
 * Description: Checks for errors in received data.
 * Parameters: UINT32 base - Base address of the UART.
 * Return: UART_ERROR - Result of the operation.
 */
UART_ERROR uart_check_errors(UINT32 base) {
    UINT32 flag_reg = UART_REG_FLAG(base);

    //Check PE
    if (flag_reg & UART_REG_FLAG_PE_MASK) { 
        uart_handle_error(UART_PARITY_ERROR);
        return UART_PARITY_ERROR;
    }//end of if (flag_reg & UART_REG_FLAG_PE_MASK)

    //Check OE
    if (flag_reg & UART_REG_FLAG_OE_MASK) {
        uart_handle_error(UART_OVERFLOW_ERROR);
        return UART_OVERFLOW_ERROR;
    }//end of if (flag_reg & UART_REG_FLAG_OE_MASK)

    return UART_SUCCESS;

}//end of uart_check_errors(...)

/*
 * Description: Performs startup tests on the UART.
 * Parameters: UINT32 base - Base address of the UART.
 * Return: UART_ERROR - Result of the operation.
 */
UART_ERROR uart_startup_tests(UINT32 base) {
    UINT32 line_control_reg = UART_REG_LCRH(base);
    UINT32 control_reg = UART_REG_CTRL(base);

    //Check for 8 data bits to see if UART is configured correctly
    if ((line_control_reg & (3 << 5)) != (3 << 5)) { 
        uart_handle_error(UART_DATA_BITS_ERROR);
        return UART_DATA_BITS_ERROR;
    }//end of if ((line_control_reg & (3 << 5)) != (3 << 5))

    //Check Transmit Enable
    if (!(control_reg & UART_REG_CTRL_TXE_MASK)) { 
        uart_handle_error(UART_TRANSMIT_ENABLE_ERROR);
        return UART_TRANSMIT_ENABLE_ERROR;
    }//end of if (!(control_reg & UART_REG_CTRL_TXE_MASK))

    //Check Receive Enable
    if (!(control_reg & UART_REG_CTRL_RXE_MASK)) { 
        uart_handle_error(UART_RECEIVE_ENABLE_ERROR);
        return UART_RECEIVE_ENABLE_ERROR;
    }//end of if (!(control_reg & UART_REG_CTRL_RXE_MASK))

    return UART_SUCCESS;
}//end of uart_startup_tests(...)

/*
 * Description: Performs integration testing.
 * Parameters: UINT32 base - Base address of the UART.
 * Return: UART_ERROR - Result of the operation.
 */
UART_ERROR uart_integration_tests(UINT32 base) {
    //Access integration test registers
    volatile UINT32 *test_control_reg = (volatile UINT32 *)UART_REG_ITCP(base);
    volatile UINT32 *test_input_reg = (volatile UINT32 *)UART_REG_ITIP(base);
    volatile UINT32 *test_output_reg = (volatile UINT32 *)UART_REG_ITOP(base);

    //Enable integration testing
    *test_control_reg = 1; 

    //Provide test input
    *test_input_reg = 0x55;

    //Perform the integration test and check output
    if ((*test_output_reg & 0xFF) != 0xAA) {
        uart_handle_error(UART_INTEGRATION_TEST_FAILED);
        return UART_INTEGRATION_TEST_FAILED;
    }//end of if ((*test_output_reg & 0xFF) != 0xAA)

    //Disable integration testing after tests
    *test_control_reg = 0; //Reset test_control_reg to disable testing

    return UART_SUCCESS;
}//end of uart_integration_tests(...)

/*
 * Description: Handles UART errors.
 * Parameters: UART_ERROR error_code - Error code to handle.
 * Return: void.
 */
void uart_handle_error(UART_ERROR error_code) {
    //Implement error handling logic
    while (1) {
        //Error handling goes here
    }//end of infinite while loop
}//end of uart_handle_error(...)
