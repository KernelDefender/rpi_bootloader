#include "uart.h"

/*
 * Description: Main function for the bootloader.
 * Parameters: None.
 * Return: int - Exit status
 */
int main() {
    UINT32 uart_base = UART0_BASE_ADDR; //Change this to UART1_BASE_ADDR for UART1, etc.
    UART_ERROR rtn;

    //Initialize UART
    rtn = uart_init(uart_base);
    if (rtn != UART_SUCCESS) {
        uart_handle_error(rtn);
    }//end of if (rtn != UART_SUCCESS)

    //Perform integration tests
    rtn = uart_integration_tests(uart_base);
    if (rtn != UART_SUCCESS) {
        uart_handle_error(rtn);
    }//end of if (rtn != UART_SUCCESS)

    //Perform startup tests
    rtn = uart_startup_tests(uart_base);
    if (rtn != UART_SUCCESS) {
        uart_handle_error(rtn);
    }//end of if (rtn != UART_SUCCESS)

    const char *message = "Hello World\n";
    rtn = uart_send_string(uart_base, message);
    if (rtn != UART_SUCCESS) {
        uart_handle_error(rtn);
    }//end of if (rtn != UART_SUCCESS)

    //Check for any errors
    rtn = uart_check_errors(uart_base);
    if (rtn != UART_SUCCESS) {
        uart_handle_error(rtn);
    }//end of if (rtn != UART_SUCCESS)

    while (1); //Keep bootloader running for now
}//end of main(...)
