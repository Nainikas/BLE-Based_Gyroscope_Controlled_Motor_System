/**
 * @file main.c
 * @brief Main source code for BLE UART with gyroscope (x, y, z) data parsing and motor control.
 *
 * This file demonstrates receiving, parsing, and processing gyroscope data over BLE
 * and controlling motors based on the parsed values. The program uses an MSP432 microcontroller
 * to read BLE data, parse the gyroscope values, and control motor movements accordingly.
 *
 * @author Nainika Saha
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "msp.h"
#include "inc/Clock.h"
#include "inc/CortexM.h"
#include "inc/GPIO.h"
#include "inc/EUSCI_A0_UART.h"
#include "inc/Motor.h"
#include "inc/BLE_UART.h"
#include "inc/GyroParser.h" // Include the parser header for processing BLE packets

#define BLE_UART_BUFFER_SIZE 128 // Define the maximum buffer size for BLE UART data

/**
 * @brief Controls motor movements based on gyroscope data.
 *
 * Determines the motor action based on the gyroscope values `x`, `y`, and `z`.
 * - **Forward/Backward**: Controlled by the `y` axis.
 * - **Left/Right**: Controlled by the `x` axis.
 * - **Stop**: Stops the motor if the `x` and `y` values are within a threshold.
 *
 * @param x Gyroscope X-axis value.
 * @param y Gyroscope Y-axis value.
 * @param z Gyroscope Z-axis value (not used in motor control).
 */
void MotorControlFromGyro(float x, float y, float z);

int main(void) {
    // Disable interrupts during initialization to prevent unwanted behavior
    DisableInterrupts();

    // Initialize peripherals
    Clock_Init48MHz();           // Set the system clock to 48 MHz
    LED2_Init();                 // Initialize the on-board RGB LED
    EUSCI_A0_UART_Init_Printf(); // Initialize UART for debugging via the serial console
    BLE_UART_Init();             // Initialize BLE UART for communication
    Motor_Init();                // Initialize motor control functionality

    // Enable global interrupts
    EnableInterrupts();

    // Reset the BLE module and indicate readiness
    BLE_UART_Reset();
    BLE_UART_OutString("BLE UART Ready\r\n");

    uint8_t BLE_UART_Buffer[BLE_UART_BUFFER_SIZE] = {0}; // Buffer for storing BLE UART data

    while (1) {
        // Read the BLE data into the buffer
        int string_size = BLE_UART_InString((char *)BLE_UART_Buffer, BLE_UART_BUFFER_SIZE);

        if (string_size > 0) {
            // Debug: Print the raw BLE data in hexadecimal format for verification
            EUSCI_A0_UART_OutString("BLE UART Raw Data: ");
            for (int i = 0; i < string_size; i++) {
                char hex[4];
                sprintf(hex, "%02X ", BLE_UART_Buffer[i]);
                EUSCI_A0_UART_OutString(hex);
            }
            EUSCI_A0_UART_OutString("\r\n");

            // Parse the BLE packet for validation and processing
            ParseBLEPacket(BLE_UART_Buffer);

            // Extract gyroscope float values for motor control
            float x, y, z;
            x = ExtractFloat(BLE_UART_Buffer, 2);  // Extract X-axis value from buffer
            y = ExtractFloat(BLE_UART_Buffer, 6);  // Extract Y-axis value from buffer
            z = ExtractFloat(BLE_UART_Buffer, 10); // Extract Z-axis value from buffer

            // Control the motors based on parsed gyroscope data
            MotorControlFromGyro(x, y, z);
        }
    }
}

/**
 * @brief Controls motor movements based on gyroscope data.
 *
 * Uses the parsed gyroscope data to control motor actions:
 * - **Forward/Backward**: Based on the `y` axis.
 * - **Left/Right**: Based on the `x` axis.
 * - **Stop**: If `x` and `y` values are within a certain range, stop the motors.
 *
 * @param x Gyroscope X-axis value.
 * @param y Gyroscope Y-axis value.
 * @param z Gyroscope Z-axis value (not used here).
 */
void MotorControlFromGyro(float x, float y, float z) {
    if (y > 0.2) {
        Motor_Forward(3000, 3000); // Move forward
        EUSCI_A0_UART_OutString("Motor: Moving Forward\r\n");
    } else if (y < -0.2) {
        Motor_Backward(3000, 3000); // Move backward
        EUSCI_A0_UART_OutString("Motor: Moving Backward\r\n");
    }

    if (x > 0.2) {
        Motor_Right(3000, 3000); // Turn right
        EUSCI_A0_UART_OutString("Motor: Turning Right\r\n");
    } else if (x < -0.2) {
        Motor_Left(3000, 3000); // Turn left
        EUSCI_A0_UART_OutString("Motor: Turning Left\r\n");
    }

    // Stop the motors if both `x` and `y` values are within the threshold
    if (fabs(x) <= 0.2 && fabs(y) <= 0.2) {
        Motor_Stop(); // Stop the motors
        EUSCI_A0_UART_OutString("Motor: Stopped\r\n");
    }
}
