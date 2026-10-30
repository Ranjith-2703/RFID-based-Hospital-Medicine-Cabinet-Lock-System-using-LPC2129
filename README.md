# RFID-Based Hospital Medicine Cabinet Lock System

An embedded-systems mini-project developed using the **LPC2129 ARM7 microcontroller**. The system provides RFID-based access control for a hospital medicine cabinet using an EM-18 RFID reader. Authorized RFID cards activate a DC motor through an L293D motor driver to open the cabinet, while unauthorized cards trigger a buzzer alert. A 16×2 LCD displays the system status.

## Table of Contents

- [Overview](#overview)
- [Objectives](#objectives)
- [Hardware Components](#hardware-components)
- [Software and Tools](#software-and-tools)
- [System Block Diagram](#system-block-diagram)
- [Software Architecture](#software-architecture)
- [Working Principle](#working-principle)
- [Peripheral Interfacing](#peripheral-interfacing)
- [Learning Outcomes](#learning-outcomes)
- [Important Implementation Notes](#important-implementation-notes)

## Overview

This project demonstrates RFID-based authentication and automatic cabinet access control using the LPC2129 ARM7 microcontroller.

The EM-18 RFID reader communicates with the LPC2129 through UART0. When an RFID card is scanned, the microcontroller receives its identification number and compares it with predefined authorized RFID IDs.

If the RFID card is authorized, the green LED indicates access, the LCD displays the access status, and the motor opens the cabinet through the L293D driver.

If an unauthorized RFID card is detected, the buzzer is activated and the LCD displays an access-denied message.

The cabinet can be closed using a push button.

## Objectives

- Implement RFID-based authentication using the EM-18 RFID reader.
- Interface UART0 communication with the LPC2129.
- Control a DC motor using the L293D motor driver.
- Display access information using a 16×2 LCD.
- Indicate authorized access using a green LED.
- Generate an audio alert for unauthorized access using a buzzer.
- Implement push-button-based cabinet closing.
- Develop embedded C firmware using Keil µVision.

## Hardware Components

- LPC2129 ARM7 microcontroller
- EM-18 RFID reader
- RFID cards/tags
- L293D motor driver IC
- DC motor
- 16×2 character LCD
- Green LED
- Buzzer
- Push button (switch)
- Regulated power supply
- Connecting wires and supporting components

## Software and Tools

- Embedded C
- Keil µVision IDE
- LPC2129 device header files
- UART serial communication
- ARM7 microcontroller development board
- USB-to-TTL serial adapter (optional for debugging)

## System Block Diagram

<img width="1536" height="909" alt="WhatsApp Image 2026-10-30 at 7 51 01 PM" src="https://github.com/user-attachments/assets/51b72ad1-405a-4ced-9748-8539a0b006df" />

## Software Architecture

### 1. Main Application — `main.c`

Responsible for:

- Configuring LPC2129 GPIO pins.
- Initializing UART0 communication.
- Initializing the LCD.
- Initializing motor and buzzer control.
- Reading RFID data.
- Comparing RFID IDs with authorized IDs.
- Controlling cabinet opening and closing.
- Handling authorized and unauthorized access.

### 2. UART0 Driver

Responsible for serial communication between the EM-18 RFID reader and LPC2129.

Functions:

- `UART0_INIT()` – Initializes UART0.
- `UART0_RX()` – Receives RFID data.

### 3. RFID Authentication Module

Responsible for:

- Receiving RFID identification data.
- Storing the received RFID ID.
- Comparing received IDs with predefined authorized RFID IDs.
- Determining whether access is granted or denied.

Functions:

- `RFID_READ()`
- `RFID_CHECK()`

### 4. LCD Driver

Responsible for displaying system messages and access status.

Functions:

- `LCD_INIT()`
- `LCD_COMMAND()`
- `LCD_DATA()`
- `LCD_STRING()`
- `LCD_LINE2()`
- `LCD_STATUS()`

### 5. Motor Control Module

Responsible for controlling cabinet opening and closing through the L293D motor driver.

Functions:

- `MOTOR_OPEN()`
- `MOTOR_CLOSE()`
- `MOTOR_STOP()`

### 6. Buzzer Control Module

Responsible for generating an alert when an unauthorized RFID card is detected.

Functions:

- `BUZZER_ON()`
- `BUZZER_OFF()`

### 7. Delay Module

- `delay_milliseconds()` implements delays using Timer 0.

## Working Principle

1. The LPC2129 initializes GPIO, UART0, LCD, motor control, and buzzer.
2. The LCD displays the project title and system startup message.
3. The system waits for an RFID card to be scanned.
4. The EM-18 reader reads the RFID card and transmits its identification data through UART0.
5. The LPC2129 receives the RFID ID and compares it with predefined authorized IDs.
6. If the RFID ID matches an authorized ID:
   - The LCD displays `Access Granted`.
   - The green LED is activated.
   - The L293D motor driver controls the DC motor to open the cabinet.
7. After the cabinet opens, the LCD displays `Cabinet Open`.
8. The user presses the push button to initiate closing.
9. The motor rotates in the opposite direction and closes the cabinet.
10. If an unauthorized RFID ID is detected:
    - The LCD displays `Unauthorized ID`.
    - The buzzer is activated.
    - The motor remains stopped.
11. The system returns to RFID scanning mode.

## Peripheral Interfacing

### UART0 Communication

- EM-18 RFID reader TX is connected to LPC2129 P0.1 (RXD0).
- UART0 is configured for serial communication.
- Baud rate: approximately 9600 bps.
- Data format: 8 data bits, no parity, 1 stop bit (8N1).

### GPIO Motor Control

- Motor IN1: P0.18
- Motor IN2: P0.19
- L293D enable input: Must be enabled through the appropriate hardware connection.

### LCD Interface

The LCD operates in 8-bit parallel mode.

- Data pins: P0.8–P0.15
- RS: P0.16
- Enable: P0.17
- RW: GND

### LED Indication

- Green LED: P0.20
- The current firmware uses active-low LED control.

### Buzzer

- Buzzer control: P0.21
- The current firmware uses active-low buzzer control.

### Push Button

- Switch input: P0.22
- Used to initiate the cabinet closing operation.
- The input requires an appropriate pull-up or pull-down configuration.

## Learning Outcomes

- ARM7 LPC2129 microcontroller programming.
- UART0 serial communication.
- RFID reader interfacing.
- RFID-based authentication.
- GPIO programming and control.
- L293D DC motor driver interfacing.
- 16×2 LCD interfacing.
- Buzzer and LED interfacing.
- Timer-based delay generation.
- Embedded C programming using Keil µVision.
- Hardware and software integration.

## Important Implementation Notes

- This project is an educational embedded-systems prototype.
- Verify all LPC2129 pin configurations against the actual hardware connections.
- Ensure the EM-18 RFID reader and LPC2129 UART logic voltage levels are compatible.
- The RFID IDs must be entered correctly in the source code.
- The current source code does not implement RFID checksum validation.
- The second sample RFID ID must be corrected because it contains a non-hexadecimal character (`S`).
- The current motor control uses fixed-duration delays. Limit switches or position sensors are recommended for reliable cabinet positioning.
- The L293D requires a suitable motor supply and an enabled motor-driver input.
- Use a separate suitable motor power supply with a common ground.
- Verify the active-low LED and buzzer connections before hardware testing.
- This prototype should not be treated as a certified medical storage security system without further safety and security validation.

## About

**Project:** RFID-Based Hospital Medicine Cabinet Lock System

**Microcontroller:** LPC2129 ARM7

**Programming Language:** Embedded C

**IDE:** Keil µVision

**RFID Module:** EM-18

**Motor Driver:** L293D

**Display:** 16×2 LCD

**Communication:** UART0

**Project Type:** Embedded Systems Mini Project
