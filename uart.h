#ifndef UART_H
#define UART_H

#include <stdint.h>

//Ensure UINT32 is defined only once
#ifndef UINT32
#define UINT32 uint32_t
#endif

//Define a type for return values
typedef UINT32 UART_ERROR;

//Base address for UART errors
#define UART_ERROR_BASE 0x00000000

//Error codes (offset from UART_ERROR_BASE)
#define UART_SUCCESS                   (UART_ERROR_BASE + 0x00)
#define UART_PARITY_ERROR              (UART_ERROR_BASE + 0x01)
#define UART_OVERFLOW_ERROR            (UART_ERROR_BASE + 0x02)
#define UART_RECEIVE_ENABLE_ERROR      (UART_ERROR_BASE + 0x03)
#define UART_TRANSMIT_ENABLE_ERROR     (UART_ERROR_BASE + 0x04)
#define UART_INTEGRATION_TEST_FAILED   (UART_ERROR_BASE + 0x05)
#define UART_DATA_BITS_ERROR           (UART_ERROR_BASE + 0x06)

/* UART Register offsets from "BCM2711 ARM Peripherals" document

Base Addresses
    UART0: 0x7E201000
    UART1: Reserved for mini UART
    UART2: 0x7E201400
    UART3: 0x7E201600
    UART4: 0x7E201800
    UART5: 0x7E201a00

Register Offsets
    0x00: DR - Data Register
    0x18: flag_reg - Flag register
    0x24: IBRD - Integer baud rate divider 
    0x28: FBRD - Fractional baud rate divisor
    0x2C: line_control_reg - Line control register 
    0x30: control_reg - Control register
    0x34: IFLS - Interrupt FIFO level select register 
    0x38: IMSC - Interrupt mask set clear register
    0x3C: RIS - Raw interrupt status register
    0x40: MIS - Masked interrupt status register
    0x44: ICR - Interrupt clear register
    0x48: DMACR - DMA control register
    0x80: ITCR - Test control register 
    0x84: test_input_reg - Integration test input register
    0x88: test_output_reg - Integration test output register
    0x8C: TDR - Test data reg
*/

//UART base addresses for each bus
#define UART0_BASE_ADDR 0x7E201000  // UART0
#define UART1_BASE_ADDR 0x7E215000  // UART1 (reserved for mini UART - not currently supported)
#define UART2_BASE_ADDR 0x7E201400  // UART2
#define UART3_BASE_ADDR 0x7E201600  // UART3
#define UART4_BASE_ADDR 0x7E201800  // UART4
#define UART5_BASE_ADDR 0x7E201A00  // UART5

//Register offsets
#define UART_REG_DATA(offset) (*(volatile UINT32 *)((offset) + 0x00)) //Data Register
#define UART_REG_FLAG(offset) (*(volatile UINT32 *)((offset) + 0x18)) //Flag Register
#define UART_REG_IBRD(offset) (*(volatile UINT32 *)((offset) + 0x24)) //Integer Baud Rate Register
#define UART_REG_FBRD(offset) (*(volatile UINT32 *)((offset) + 0x28)) //Fractional Baud Rate Register
#define UART_REG_LCRH(offset) (*(volatile UINT32 *)((offset) + 0x2C)) //Line Control Register
#define UART_REG_CTRL(offset) (*(volatile UINT32 *)((offset) + 0x30)) //Control Register
#define UART_REG_ITCP(offset) (*(volatile UINT32 *)((offset) + 0x3C)) //Integration Test Control Register
#define UART_REG_ITIP(offset) (*(volatile UINT32 *)((offset) + 0x40)) //Integration Test Input Register
#define UART_REG_ITOP(offset) (*(volatile UINT32 *)((offset) + 0x44)) //Integration Test Output Register

//Bit masks for control register
#define UART_REG_CTRL_TXE_MASK (1 << 0) // Transmit Enable
#define UART_REG_CTRL_RXE_MASK (1 << 9) // Receive Enable

//Bit masks for flag Register
#define UART_REG_FLAG_TXFF_MASK (1 << 5) // Transmit FIFO Full
#define UART_REG_FLAG_RXFE_MASK (1 << 4) // Receive FIFO Empty
#define UART_REG_FLAG_PE_MASK   (1 << 3) // Parity Error
#define UART_REG_FLAG_OE_MASK   (1 << 6) // Overrun Error

//Function prototype declaration
UART_ERROR uart_init(UINT32 base);
UART_ERROR uart_send_char(UINT32 base, char c);
UART_ERROR uart_send_string(UINT32 base, const char *str);
UART_ERROR uart_check_errors(UINT32 base);
UART_ERROR uart_startup_tests(UINT32 base);
UART_ERROR uart_integration_tests(UINT32 base);
void uart_handle_error(UART_ERROR error_code);

#endif //end of #ifndef UART_H
