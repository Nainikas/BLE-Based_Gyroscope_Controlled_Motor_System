/**
 * @file GyroParser.c
 * @brief Source code for parsing and validating gyroscope BLE packets.
 *
 * This file implements functions to parse gyroscope data from BLE packets,
 * validate the packet structure and checksum (CRC), and extract floating-point
 * values for further processing.
 *
 * @author Nainika Saha
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "inc/GyroParser.h" // Include the header for function declarations

// Function prototypes
void ParseBLEPacket(uint8_t *buffer);
bool ValidateBLEPacket(uint8_t *buffer);
bool ValidateCRC(uint8_t *buffer);
float ExtractFloat(uint8_t *buffer, uint8_t offset);

/**
 * @brief Parses a BLE packet to process gyroscope data.
 *
 * Validates the BLE packet structure and checksum. If valid, extracts the
 * gyroscope X, Y, and Z values as floating-point numbers and prints them.
 *
 * @param buffer Pointer to the BLE packet buffer.
 */
void ParseBLEPacket(uint8_t *buffer) {
    // Validate the BLE packet
    if (!ValidateBLEPacket(buffer)) {
        printf("Invalid BLE Packet\r\n");
        return;
    }

    // Extract gyroscope values from the BLE packet
    float x = ExtractFloat(buffer, 2);  // Extract X-axis value
    float y = ExtractFloat(buffer, 6);  // Extract Y-axis value
    float z = ExtractFloat(buffer, 10); // Extract Z-axis value

    // Debugging output: Print the parsed gyroscope values
    printf("Parsed Floats - X: %.2f, Y: %.2f, Z: %.2f\r\n", x, y, z);

    // Integrate motor control logic or pass the values to other modules here
}

/**
 * @brief Validates the structure of a BLE packet.
 *
 * Ensures the packet has the correct prefix (`!G`) and a valid checksum (CRC).
 *
 * @param buffer Pointer to the BLE packet buffer.
 * @return true if the packet structure and CRC are valid, false otherwise.
 */
bool ValidateBLEPacket(uint8_t *buffer) {
    // Check that the packet starts with the correct prefix "!G"
    if (buffer[0] != 0x21 || buffer[1] != 0x47) {
        return false;
    }

    // Validate the packet checksum
    return ValidateCRC(buffer);
}

/**
 * @brief Validates the checksum (CRC) of a BLE packet.
 *
 * Computes the checksum by summing all bytes except the last byte and inverting
 * the sum. Compares the result to the last byte of the packet.
 *
 * @param buffer Pointer to the BLE packet buffer.
 * @return true if the checksum is valid, false otherwise.
 */
bool ValidateCRC(uint8_t *buffer) {
    uint8_t sum = 0;

    // Calculate the checksum by summing the first 14 bytes
    for (int i = 0; i < 14; i++) {
        sum += buffer[i];
    }

    // Invert the sum to compute the checksum
    sum = ~sum;

    // Compare the computed checksum with the last byte of the packet
    return (sum == buffer[14]);
}

/**
 * @brief Extracts a floating-point value from a BLE packet.
 *
 * Reads four bytes starting from the specified offset in the BLE packet and
 * interprets them as a 32-bit floating-point value.
 *
 * @param buffer Pointer to the BLE packet buffer.
 * @param offset Offset in the buffer where the float value starts.
 * @return The extracted floating-point value.
 */
float ExtractFloat(uint8_t *buffer, uint8_t offset) {
    float value;

    // Copy 4 bytes from the buffer starting at the offset into the float variable
    memcpy(&value, &buffer[offset], 4);

    return value; // Return the extracted float value
}
