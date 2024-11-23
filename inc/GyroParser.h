/**
 * @file GyroParser.h
 * @brief Header file for the Gyroscope BLE Packet Parser.
 *
 * This file provides the function declarations required to parse and validate
 * gyroscope data received over BLE. It includes utilities for extracting
 * floating-point values from BLE packets and performing packet validation.
 *
 * @author Nainika Saha
 */

#ifndef GYRO_PARSER_H
#define GYRO_PARSER_H

#include <stdint.h>
#include <stdbool.h>

// Function Declarations

/**
 * @brief Parses a BLE packet to process gyroscope data.
 *
 * Extracts gyroscope data from the provided BLE packet, validates the data,
 * and handles it appropriately.
 *
 * @param buffer Pointer to the BLE packet buffer.
 */
void ParseBLEPacket(uint8_t *buffer);

/**
 * @brief Validates the structure of a BLE packet.
 *
 * Checks whether the packet has the correct prefix, size, and structure.
 *
 * @param buffer Pointer to the BLE packet buffer.
 * @return true if the packet structure is valid, false otherwise.
 */
bool ValidateBLEPacket(uint8_t *buffer);

/**
 * @brief Validates the CRC (checksum) of a BLE packet.
 *
 * Computes the checksum of the packet and compares it with the provided CRC.
 *
 * @param buffer Pointer to the BLE packet buffer.
 * @return true if the CRC is valid, false otherwise.
 */
bool ValidateCRC(uint8_t *buffer);

/**
 * @brief Extracts a floating-point value from a BLE packet.
 *
 * Interprets four consecutive bytes from the specified offset in the BLE packet
 * as a 32-bit floating-point value.
 *
 * @param buffer Pointer to the BLE packet buffer.
 * @param offset Offset in the buffer where the float starts.
 * @return The extracted float value.
 */
float ExtractFloat(uint8_t *buffer, uint8_t offset);

#endif // GYRO_PARSER_H
