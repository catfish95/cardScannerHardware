# TCG Card Scanner Hardware

## Overview

This is the TCG Card Scanner hardware project. The scanner is a specialized device designed to automatically scan and identify trading card game (TCG) cards.

## Architecture

The TCG Card Scanner uses a distributed architecture:

- **Raspberry Pi 5**: The main processing unit ("brains") that handles image analysis, card identification, and server communication
- **Raspberry Pi Pico**: Microcontroller for hardware interfacing and control
- **Touch Screen Display**: User interface connected to the Raspberry Pi 5 for card scanning control and feedback
- **Camera Module**: Image capture hardware for scanning cards

## Key Changes from Original Design

The scanner has been redesigned with the following changes from the original version:

1. **Microcontroller Change**: Replaced Arduino with Raspberry Pi Pico for more integrated hardware control
2. **Processing Unit**: Raspberry Pi 5 serves as the main brains, enabling more sophisticated image processing and analysis
3. **Image Capture**: Removed dependency on smartphones for image capture; now uses dedicated camera hardware
4. **User Interface**: Replaced physical buttons (start button, counter, etc.) with a touch screen interface
5. **Server Communication**: Raspberry Pi 5 handles all communication with the backend server

## Hardware Components

### Main Processor
- Raspberry Pi 5

### Microcontroller
- Raspberry Pi Pico

### Input/Output
- Touch Screen Display (connected to RPi 5)
- Dual Camera Module (connected to RPi 5)

### Power & Connectivity
- 12V Power Supply (1A minimum)
- Network connectivity (Ethernet or Wi-Fi) for server communication

### Sensing & Control
- Light Dependent Resistor (LDR) for card position detection
- Microphone module for app trigger detection
- Piezo buzzer for audio feedback
- 7-segment display for card count visualization
- Control buttons (start/stop and reset)

### Lighting
- Neopixel LED ring for optimal card illumination
- 3D-printed diffuser for soft, even lighting

### Mechanical
- Motor for card feeder mechanism
- Motor driver (L298N H-bridge recommended) for motor control
- Card separation mechanism using friction feeder principle

## Directory Structure

```
scanner/
├── README.md                          # This file
├── TCGScanner/
│   ├── TGC Scanner.DXF                # Hardware CAD design file
│   ├── Electronics/                   # Circuit diagrams and schematics
│   │   └── TCG Card Scanner_02.plt    # PCB layout/design
│   ├── STL Files/                     # 3D printable components for physical housing
│   └── Arduino Code/                  # Reference code (legacy, see Pico implementation)
│       ├── TCG_Card_Scanner_00_English/
│       └── TCG_Card_Scanner_00_Deutsch/
```

## Development

### Raspberry Pi Pico Firmware
The RPi Pico firmware handles:
- Hardware initialization and control
- Communication protocol with Raspberry Pi 5
- Real-time sensor and component management

### Raspberry Pi 5 Software
The RPi 5 software includes:
- Image processing and card recognition
- Touch screen interface management
- Server API communication
- Application logic and card database queries

## Hardware Control System

### Control Logic
The Raspberry Pi Pico manages precise control of the card feeding process:

1. **Card Detection**: LDR (Light Dependent Resistor) detects gaps between cards using ambient light changes
2. **Motor Activation**: Triggered by the scanning app via audio signal (microphone module detects tone)
3. **Motion Control**: Motor drives exactly one card forward until the LDR detects the next gap
4. **Automatic Stop**: Motor stops when card gap is detected, preventing multiple-card feeds
5. **Feedback**: Piezo buzzer and LED ring provide visual and audio feedback

### Lighting System
- **Neopixel LED Ring**: Provides optimized illumination for card scanning
- **Dynamic Adjustment**: LEDs dim slightly after card feed if scan doesn't complete, reducing glare and reflections
- **Soft Illumination**: 3D-printed diffuser spreads light evenly, minimizing harsh shadows
- **Color Strategy**: Red light works best for Magic: The Gathering cards (reduces glare on glossy surfaces)

### User Interface Components
- **7-Segment Display**: Shows count of scanned cards in real-time
- **Start/Stop Button**: Pause scanning to make adjustments
- **Reset Button**: Clear card counter and restart
- **Piezo Buzzer**: Audio alerts for errors and system status

## Deployment

The system communicates with the `cardScannerServer` backend for:
- Card identification and database lookup
- User management
- Scanning history and analytics

## 3D Printing

### Print Settings
✅ Infill: 20%
✅ Perimeter: 2 exterior walls
✅ Top and bottom layers: 3

### Material Recommendations
- **Material**: PLA (Polylactic Acid)
- **Color**: White filament (critical for card detection - the scanner app relies on color differentiation between white plastic and card edges)
- **Build Plate Size**: Minimum 220 x 220 mm

### Important Note
White filament is essential throughout the scanner, particularly in the scanning area. The card detection logic depends on distinguishing between the white mechanical components and the colored card edges.

## Card Separation Mechanism

The scanner uses a friction feeder mechanism to ensure reliable single-card separation:

### Principle
- Based on the friction feeding principle (proven design from similar applications)
- Achieves 100% reliability when properly calibrated
- Prevents multiple cards from being fed simultaneously

### Adjuster Roller Setup
The retainer roller controls how tightly cards are held. Proper calibration is essential:

1. **Load first card**: Place one card into the scanner and slide it under the retainer roller
2. **Set light resistance**: Adjust the screw on the retainer roller until you feel very light resistance on the card
3. **Test with second card**: Place a second card on top. It should experience noticeable resistance from the retainer roller
4. **Verify calibration**: Once adjusted correctly, the mechanism feeds exactly one card reliably

## Documentation

- **DXF Design**: CAD file for the physical enclosure (`TGC Scanner.DXF`)
- **Electronics**: Schematic and PCB layout files in the `Electronics/` directory
- **3D Models**: STL files for 3D-printable components in the `STL Files/` directory

## Next Steps

For implementation details, refer to:
1. RPi Pico firmware implementation (to be added)
2. RPi 5 software stack (to be added)
3. Touch screen interface configuration (to be added)
4. Network and server integration guide (to be added)

## License

See LICENSE file in the parent directory for details.
