# cardScannerHardware

Hardware documentation and design files for the TCG Card Scanner project.

## Overview

The cardScannerHardware repository contains all the hardware designs, electronics schematics, 3D-printable parts, and Arduino firmware for the TCG Card Scanner automation system.

## Project Structure

### Scanner Hardware (`scanner/TCGScanner/`)

The main scanner hardware project includes:

#### Arduino Code
- **TCG_Card_Scanner_00_English/** - English version of the scanner firmware
- **TCG_Card_Scanner_00_Deutsch/** - German version of the scanner firmware

Key Arduino components:
- Motor control (ENA, IN1, IN2 pins)
- TM1637 7-segment display for card counting
- Adafruit NeoPixel LED ring (8 LEDs) for status indication
- Light sensor (LDR) for card detection
- Buzzer feedback
- Push buttons for manual trigger and reset
- Microphone input for audio feedback

#### Design Files
- **TGC Scanner.DXF** - Technical drawing in DXF format
- **TGC Scanner.PDF** - Portable document version of technical drawings
- **Electronics/** - Electronic schematics and layouts

#### Parts List
- **Part List - TCG Scanner_00.xlsx** - Spreadsheet of required components
- **Part List - TCG Scanner_00.pdf** - PDF version of parts list

#### 3D Printable Components (`STL Files/`)
Complete set of 3D-printable parts including:
- **Structural:** Side panels, side guides, cross connections, light stand
- **Drive System:** Gears, shafts, belt tensioner, rollers, roller holders
- **Card Handling:** Card magazine, card support, retention rollers, outlet guide
- **Electronics Housing:** Gearbox cover, LED holder, light holders, microphone holder
- **Mobile Support:** Smartphone holders (height adjustable), smartphone holder joint
- **Storage:** Side guide storage stacks
- **Hardware Support:** Cable protection, clamping pieces, sleeves

## Hardware Features

- **Automated Card Feeding** - Motor-driven card transport system
- **Card Detection** - Light sensor (LDR) based detection
- **Card Counting** - 7-segment display for real-time count
- **Status Indication** - 8-color NeoPixel LED ring
- **Audio Feedback** - Buzzer for user feedback
- **Mobile Integration** - Adjustable smartphone holders for imaging/processing
- **Configurable Operation** - Multiple startup sequences available

## Building the Scanner

1. Review the part list (Excel or PDF)
2. Obtain all electronic components
3. Print all required STL files (complete list in STL Files directory)
4. Assemble mechanical structure using the technical drawings
5. Wire electronics according to the schematics in Electronics folder
6. Upload Arduino firmware (English or German version) to microcontroller

## Firmware

The Arduino code includes:
- Motor control and PWM management
- Real-time card detection and counting
- LED ring status animations with color fading
- Display brightness dimming
- Button debouncing and timeout handling
- Multiple startup sequences for visual feedback

## Files and Formats

- **DXF/PDF** - For technical drawings and schematics
- **STL** - 3D printable files (printer-ready)
- **XLSX/PDF** - Parts and bill of materials
- **INO** - Arduino firmware code
