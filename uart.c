#include "uart.h"

// Function to initialize UART
UART_ERROR uart_init(UINT32 base) {
    // Disable UART
    UINT32 cr = UART_REG_CTRL(base);
    cr &= ~(UART_REG_CTRL_TXE_MASK | UART_REG_CTRL_RXE_MASK); // Clear TXE and RXE
    UART_REG_CTRL(base) = cr;

    // Set baud rate (example: 115200 baud)
    UART_REG_IBRD(base) = 1;  // Integer part of baud rate
    UART_REG_FBRD(base) = 40; // Fractional part of baud rate

    // Configure 8 data bits, no parity, 1 stop bit
    UART_REG_LCRH(base) = (3 << 5); // 8 bits, no parity, 1 stop bit

    // Enable UART
    cr |= (UART_REG_CTRL_TXE_MASK | UART_REG_CTRL_RXE_MASK); // Set TXE and RXE
    UART_REG_CTRL(base) = cr;

    return UART_SUCCESS;
}

// Function to send a character to the UART
UART_ERROR uart_send_char(UINT32 base, char c) {
    while (UART_REG_FLAG(base) & UART_REG_FLAG_TXFF_MASK);  // Wait until TXFF is 0
    UART_REG_DATA(base) = c;  // Write the character
    return UART_SUCCESS;
}

// Function to send a string to the UART
UART_ERROR uart_send_string(UINT32 base, const char *str) {
    while (*str) {
        UART_ERROR rtn = uart_send_char(base, *str++);
        if (rtn != UART_SUCCESS) {
            return rtn;
        }
    }
    return UART_SUCCESS;
}

// Function to check for errors in received data
UART_ERROR uart_check_errors(UINT32 base) {
    UINT32 fr = UART_REG_FLAG(base);

    if (fr & UART_REG_FLAG_PE_MASK) {  // Check PE (Parity Error)
        uart_handle_error(UART_PARITY_ERROR);
        return UART_PARITY_ERROR;
    }

    if (fr & UART_REG_FLAG_OE_MASK) {  // Check OE (Overrun Error)
        uart_handle_error(UART_OVERFLOW_ERROR);
        return UART_OVERFLOW_ERROR;
    }

    return UART_SUCCESS;
}

// Function to perform startup tests on the UART
UART_ERROR uart_startup_tests(UINT32 base) {
    UINT32 lcrh = UART_REG_LCRH(base);
    UINT32 cr = UART_REG_CTRL(base);

    // Check if UART is configured correctly
    if ((lcrh & (3 << 5)) != (3 << 5)) { // Check for 8 data bits
        uart_handle_error(UART_DATA_BITS_ERROR);
        return UART_DATA_BITS_ERROR;
    }

    if (!(cr & UART_REG_CTRL_TXE_MASK)) { // Check Transmit Enable
        uart_handle_error(UART_TRANSMIT_ENABLE_ERROR);
        return UART_TRANSMIT_ENABLE_ERROR;
    }

    if (!(cr & UART_REG_CTRL_RXE_MASK)) { // Check Receive Enable
        uart_handle_error(UART_RECEIVE_ENABLE_ERROR);
        return UART_RECEIVE_ENABLE_ERROR;
    }

    return UART_SUCCESS;
}

// Function to perform integration testing
UART_ERROR uart_integration_tests(UINT32 base) {
    // Access integration test registers
    volatile UINT32 *itcp = (volatile UINT32 *)UART_REG_ITCP(base);
    volatile UINT32 *itip = (volatile UINT32 *)UART_REG_ITIP(base);
    volatile UINT32 *itop = (volatile UINT32 *)UART_REG_ITOP(base);

    // Enable integration testing
    *itcp = 1; // Enable integration test mode

    // Provide test input
    *itip = 0x55; // Example test input value

    // Perform the integration test and check output
    if ((*itop & 0xFF) != 0xAA) { // Example expected output
        uart_handle_error(UART_INTEGRATION_TEST_FAILED);
        return UART_INTEGRATION_TEST_FAILED;
    }

    // Disable integration testing after tests
    *itcp = 0; // Reset ITCP to disable testing

    return UART_SUCCESS;
}

// Function to handle UART errors
void uart_handle_error(UART_ERROR error_code) {
    // Implement error handling logic, e.g., logging or printing the error code
    // For simplicity, just entering an infinite loop here
    while (1) {
        // Error handling logic (e.g., logging) can be implemented here
    }
}
