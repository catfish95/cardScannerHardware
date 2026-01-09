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
- Power supply suitable for RPi 5 and RPi Pico
- Network connectivity (Ethernet or Wi-Fi) for server communication

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

## Deployment

The system communicates with the `cardScannerServer` backend for:
- Card identification and database lookup
- User management
- Scanning history and analytics

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
