/**
 * @file BLE_UART.h
 * @brief Header file for the BLE_UART driver.
 *
 * This file provides the function declarations and constants required for
 * the BLE_UART driver. It facilitates communication with the Adafruit
 * Bluefruit LE UART Friend module, handling initialization, data transmission,
 * data reception, and gyroscope data processing.
 *
 * @author Nainika Saha
 */

#ifndef INC_BLE_UART_H_
#define INC_BLE_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "Clock.h"

// Constants
#define BLE_UART_BUFFER_SIZE 128 ///< Buffer size for storing BLE UART data

// Function Declarations

/**
 * @brief Initializes the BLE UART module.
 *
 * Configures the UART pins and the EUSCI_A3 module for communication.
 * Sets up the MOD pin for controlling the BLE module.
 */
void BLE_UART_Init();

/**
 * @brief Receives a single character from the BLE UART module.
 *
 * Waits until the RX buffer is ready and retrieves the received character.
 *
 * @return The character received from the UART RX buffer.
 */
uint8_t BLE_UART_InChar();

/**
 * @brief Sends a single character over the BLE UART module.
 *
 * Waits until the TX buffer is ready and transmits the character.
 *
 * @param data The character to transmit.
 */
void BLE_UART_OutChar(uint8_t data);

/**
 * @brief Receives a BLE packet as a string from the UART interface.
 *
 * Reads characters into a buffer until a full BLE packet is received or the
 * buffer size is reached.
 *
 * @param buffer_pointer Pointer to the buffer where received data will be stored.
 * @param buffer_size The maximum size of the buffer.
 * @return The length of the received packet.
 */
int BLE_UART_InString(char *buffer_pointer, uint16_t buffer_size);

/**
 * @brief Sends a string over the BLE UART module.
 *
 * Transmits characters from the provided string until a null terminator is reached.
 *
 * @param pt Pointer to the string to be transmitted.
 */
void BLE_UART_OutString(char *pt);

/**
 * @brief Resets the BLE module.
 *
 * Switches the BLE module to CMD mode, sends a reset command, and switches back to DATA mode.
 */
void BLE_UART_Reset();

/**
 * @brief Handles received BLE data packets.
 *
 * Validates the packet structure, checks the CRC, and processes gyroscope data.
 *
 * @param buffer Pointer to the received data buffer.
 * @param len The length of the received data.
 */
void BLE_UART_HandleRxData(uint8_t *buffer, uint8_t len);

/**
 * @brief Validates the checksum of a BLE packet.
 *
 * Calculates the checksum by summing all bytes except the last byte
 * and comparing it to the inverted sum of the last byte.
 *
 * @param buffer Pointer to the data buffer.
 * @return true if the checksum is valid, false otherwise.
 */
bool checkCRC(uint8_t *buffer);

/**
 * @brief Prints gyroscope (x, y, z) data to the UART interface.
 *
 * Extracts and formats the gyroscope data from the buffer and sends it over UART.
 *
 * @param buffer Pointer to the data buffer containing gyroscope values.
 */
void printSensorDataXYZ(uint8_t *buffer);

/**
 * @brief Processes gyroscope data and controls motor movements.
 *
 * Determines motor actions (forward, backward, left, right, stop) based on the
 * gyroscope data values.
 *
 * @param x Gyroscope X-axis value.
 * @param y Gyroscope Y-axis value.
 * @param z Gyroscope Z-axis value.
 */
void Process_Gyro_Data(float x, float y, float z);

#endif /* INC_BLE_UART_H_ */
