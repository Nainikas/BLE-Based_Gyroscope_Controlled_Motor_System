/**
 * @file BLE_UART.c
 * @brief Source code for the BLE_UART driver.
 *
 * This file provides the implementation for the BLE_UART driver, which handles communication
 * with the Adafruit Bluefruit LE UART Friend Bluetooth Low Energy (BLE) module.
 * It includes functions for initializing the UART interface, sending and receiving BLE data,
 * and processing gyroscope packets.
 *
 * @author Nainika
 */

#include "../inc/BLE_UART.h"
#include "../inc/GyroParser.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Initializes the BLE UART module.
 *
 * Configures UART pins, sets up the EUSCI_A3 UART module for communication,
 * and prepares the MOD pin for controlling the BLE module.
 */
void BLE_UART_Init() {
    // Configure pins P9.6 (RX) and P9.7 (TX) for UART communication
    P9->SEL0 |= 0xC0;
    P9->SEL1 &= ~0xC0;

    // Configure P1.6 as GPIO output for MOD pin
    P1->SEL0 &= ~0x40;
    P1->SEL1 &= ~0x40;
    P1->DIR |= 0x40;

    // Hold the EUSCI_A3 module in reset state
    EUSCI_A3->CTLW0 |= 0x01;

    // Configure UART parameters: SMCLK, 8-bit data, no parity, LSB first
    EUSCI_A3->CTLW0 = 0x0080;
    EUSCI_A3->BRW = 1250;  // Baud rate = 9600
    EUSCI_A3->MCTLW = 0;   // No modulation

    // Enable RX and TX interrupts
    EUSCI_A3->IE |= 0x03;

    // Release the EUSCI_A3 module from reset state
    EUSCI_A3->CTLW0 &= ~0x01;
}

/**
 * @brief Receives a single character from the BLE UART module.
 *
 * Waits until the RX buffer is ready and retrieves the received character.
 *
 * @return The character received from the UART RX buffer.
 */
uint8_t BLE_UART_InChar() {
    while (!(EUSCI_A3->IFG & 0x01)); // Wait for RX buffer to be ready
    return EUSCI_A3->RXBUF;          // Return the received character
}

/**
 * @brief Transmits a single character over the BLE UART module.
 *
 * Waits until the TX buffer is ready and sends the character.
 *
 * @param data The character to transmit.
 */
void BLE_UART_OutChar(uint8_t data) {
    while (!(EUSCI_A3->IFG & 0x02)); // Wait for TX buffer to be ready
    EUSCI_A3->TXBUF = data;          // Transmit the character
}

/**
 * @brief Receives a BLE packet from the UART interface.
 *
 * Reads characters into a buffer until a full BLE packet is received
 * (starting with `!G` and ending with a checksum).
 *
 * @param buffer_pointer Pointer to the buffer where received data will be stored.
 * @param buffer_size The maximum size of the buffer.
 * @return The length of the received packet.
 */
int BLE_UART_InString(char *buffer_pointer, uint16_t buffer_size) {
    int length = 0;
    uint8_t state = 0;

    while (length < buffer_size) {
        uint8_t character = BLE_UART_InChar();

        if (state == 0 && character == 0x21) { // Look for '!'
            *buffer_pointer++ = character;
            length = 1;
            state = 1;
        } else if (state == 1 && character == 0x47) { // Look for 'G'
            *buffer_pointer++ = character;
            length++;
            state = 2;
        } else if (state == 2) { // Collect the remaining bytes
            *buffer_pointer++ = character;
            length++;

            if (length == 15) { // Full packet (including checksum) received
                break;
            }
        } else { // Reset on desynchronization
            state = 0;
            length = 0;
            buffer_pointer = buffer_pointer - length;
        }
    }

    return length; // Return the length of the received packet
}

/**
 * @brief Sends a string over the BLE UART module.
 *
 * Transmits characters from the provided string until a null terminator is reached.
 *
 * @param pt Pointer to the string to be transmitted.
 */
void BLE_UART_OutString(char *pt) {
    while (*pt) {
        BLE_UART_OutChar(*pt++);
    }
}

/**
 * @brief Handles received BLE data packets.
 *
 * Validates the packet structure, checks the CRC, and forwards the packet
 * to the gyroscope parser for processing.
 *
 * @param buffer Pointer to the received data buffer.
 * @param len The length of the received data.
 */
void BLE_UART_HandleRxData(uint8_t *buffer, uint8_t len) {
    if (len != 15 || buffer[0] != 0x21 || buffer[1] != 0x47) { // Validate frame size and prefix
        BLE_UART_OutString("Error: Invalid data received\r\n");
        return;
    }

    if (!checkCRC(buffer)) { // Validate checksum
        BLE_UART_OutString("Error: CRC check failed\r\n");
        return;
    }

    // Pass the valid packet to GyroParser for processing
    ParseBLEPacket(buffer);
}

/**
 * @brief Validates the checksum of a BLE packet.
 *
 * Calculates the checksum by summing all bytes except the last byte
 * and comparing it to the inverted sum of the last byte.
 *
 * @param buffer Pointer to the data buffer.
 * @return true if the checksum is valid, false otherwise.
 */
bool checkCRC(uint8_t *buffer) {
    uint8_t sum = 0;

    // Calculate the checksum (sum of all bytes except the last byte)
    for (int i = 0; i < 14; i++) {
        sum += buffer[i];
    }

    sum = ~sum; // Invert the sum

    return (sum == buffer[14]); // Compare with the received checksum
}

/**
 * @brief Resets the BLE module.
 *
 * Switches the BLE module to CMD mode, sends a reset command, and switches back to DATA mode.
 */
void BLE_UART_Reset() {
    P1->OUT |= 0x40;  // Switch to CMD mode
    Clock_Delay1ms(1000);

    BLE_UART_OutString("ATZ\r\n");  // Send reset command
    Clock_Delay1ms(3000);

    P1->OUT &= ~0x40;  // Switch back to DATA mode
}
