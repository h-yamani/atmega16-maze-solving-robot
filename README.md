# ATmega16 Labyrinth Robot

<p align="center">
  <img src="assets/labyrinth-robot-header-checkerboard.png"
       alt="Labyrinth Robot navigating a physical checkerboard maze"
       width="100%">
</p>

<p align="center">
  <strong>A custom embedded robot that senses, maps, and autonomously explores a bounded labyrinth.</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/MCU-ATmega16-2f6f6f" alt="ATmega16">
  <img src="https://img.shields.io/badge/Language-Embedded%20C-2563eb" alt="Embedded C">
  <img src="https://img.shields.io/badge/Toolchain-CodeVisionAVR-f59e0b" alt="CodeVisionAVR">
  <img src="https://img.shields.io/badge/Domain-Mobile%20Robotics-7c3aed" alt="Mobile Robotics">
  <img src="https://img.shields.io/badge/Status-Archival%20Project-64748b" alt="Archival project">
</p>

---

## Overview

This repository preserves and documents a bachelor-level embedded robotics project built around the **ATmega16 microcontroller**. The robot combines directional infrared obstacle sensing, floor/path sensing, stepper-motor actuation, grid-based state tracking, and a 16-character LCD to explore a bounded maze and reach a configured goal cell.

The firmware maintains an internal estimate of the robot's grid position and heading. At each cell, it evaluates neighbouring directions, rejects blocked or out-of-bound moves, records what it has discovered, prioritises unexplored paths, and revisits known paths when required. The resulting behaviour is an online maze-exploration strategy; it is not presented as a guaranteed shortest-path algorithm.

## Project Highlights

| Capability | Implementation |
|---|---|
| Environment sensing | Front, rear, left, and right obstacle sensors |
| Path alignment | Two floor sensors with corrective steering |
| Localisation | Grid coordinates `(x, y)` and compass heading |
| Maze memory | Unknown, open, visited, and blocked directions per cell |
| Decision policy | Unexplored moves are prioritised before revisits |
| Motion | Stepper sequences for forward, reverse, left, and right movement |
| Goal handling | Stops when the configured goal coordinate is reached |
| User feedback | LCD sensor readings, position, completion, and dead-end status |

## Robot Hardware

The robot uses a custom two-level PCB chassis containing the controller, sensor interfaces, motor-drive electronics, wiring, and mechanical support. The labelled view below identifies the main visible components.

<p align="center">
  <img src="assets/labyrinth-robot-hardware-labelled.png"
       alt="Labelled hardware elements of the ATmega16 labyrinth robot"
       width="100%">
</p>

> The floor sensors, rear-facing sensor, LCD, and opposing wheel are connected to the platform but are not fully visible from this camera angle.

## System Architecture

Sensor readings are sampled by the ATmega16 ADC and passed to the navigation logic. The controller updates its internal map and heading, selects a valid movement, drives the stepper motors, and observes the resulting sensor feedback. Navigation and sensor status are reported through the LCD.

<p align="center">
  <img src="assets/system-architecture-canva.png"
       alt="Closed-loop ATmega16 labyrinth robot system architecture"
       width="100%">
</p>

## Navigation Process

The navigation controller follows a repeated **sense–localise–map–select–move–correct** cycle.

<p align="center">
  <img src="assets/maze-navigation-architecture.png"
       alt="Sense, map, plan, move, and correct navigation process"
       width="100%">
</p>

At every grid cell, the controller:

1. Reads the front, rear, left, and right obstacle sensors.
2. Reads the two floor sensors used for path alignment.
3. Converts relative movement directions into candidate grid coordinates.
4. Rejects candidates that cross a detected wall or maze boundary.
5. Records open, visited, and blocked directions in the internal cell map.
6. Prioritises unexplored directions before previously visited paths.
7. Updates the robot's grid position and compass heading.
8. Executes the corresponding stepper-motor sequence.
9. Corrects alignment while moving using the floor-sensor pair.
10. Stops when the configured goal coordinate is reached.

## Internal Maze Representation

The firmware stores maze knowledge in:

```c
char location[10][10][5];
```

For each grid cell, four entries represent movement directions and the fifth stores the heading used as the cell's orientation reference. When the robot later enters the same cell from a different direction, `set()` rotates the stored directional state into the current frame.

| Stored value | Meaning |
|---:|---|
| `NULL` | Direction has not yet been evaluated |
| `'0'` | Open and unexplored direction |
| `'1'` | Previously visited direction |
| `'3'` | Blocked, invalid, or deliberately closed direction |

This compact representation allows the controller to preserve local maze knowledge while operating within the limited memory available on the ATmega16.

## Motion and Path Correction

The left and right motors are driven using four-phase step sequences. Forward motion advances both motors in the same direction, turning reverses one phase sequence relative to the other, and reverse motion steps both motors backwards.

During forward travel, `check_sensor()` compares the two floor sensors. If their classifications differ, `correct_path()` briefly adjusts the drive sequence until both sensors agree, helping the robot remain aligned with the intended path.

## Configuration

Important firmware values are defined near the beginning of `src/labyrinth_robot.c`:

| Setting | Current value | Purpose |
|---|---:|---|
| Start cell | `(3, 1)` | Initial internal robot position |
| Goal cell | `(2, 5)` | Target grid coordinate |
| Valid maze cells | rows `1–3`, columns `1–5` | Enforced by `promissing()` |
| Initial heading | North | Initial orientation reference |
| Motor step delay | `4 ms` | Delay between stepper phases |
| Floor threshold | `150` | Floor-sensor classification threshold |
| Forward movement | `218` phase iterations | Approximate one-cell travel distance |
| Turn movement | `70` phase iterations | Approximate 90-degree rotation |

Directional obstacle-sensor thresholds are calibrated separately in the main loop. All thresholds and motion constants are hardware-dependent and should be recalibrated before operating a rebuilt platform.

## Repository Structure

```text
atmega16-maze-solving-robot/
├── assets/
│   ├── labyrinth-robot-header-checkerboard.png
│   ├── labyrinth-robot-hardware-labelled.png
│   ├── maze-navigation-architecture.png
│   ├── system-architecture-canva.png
│   └── robot.png
├── docs/
│   └── TECHNICAL_NOTES.md
├── legacy/
│   └── Labyrinth.CPP
├── src/
│   └── labyrinth_robot.c
└── README.md
```

- `src/labyrinth_robot.c` — cleaned and documented firmware for review and maintenance.
- `legacy/Labyrinth.CPP` — untouched historical source from the original project.
- `docs/TECHNICAL_NOTES.md` — hardware assumptions, state representation, calibration notes, and known limitations.
- `assets/` — repository header, hardware annotation, and navigation diagrams.

## Development Environment

The firmware was written for **CodeVisionAVR** and uses toolchain-specific headers and syntax:

- `<mega16.h>` for ATmega16 register definitions
- `<delay.h>` for blocking timing delays
- `<lcd.h>` and the CodeVisionAVR LCD port directive
- Compiler-provided `bit` variables

The project is therefore not expected to compile directly with a standard desktop C/C++ compiler. Porting it to `avr-gcc` would require replacing the CodeVisionAVR-specific headers, LCD configuration, and `bit` declarations.

### Historical build workflow

1. Open `src/labyrinth_robot.c` in CodeVisionAVR.
2. Select the ATmega16 target and configure the original system clock.
3. Confirm the LCD port mapping and ADC reference configuration.
4. Build the firmware and program the microcontroller.
5. Recalibrate sensor thresholds and movement constants on the physical robot.

## Known Limitations

- Maze dimensions and goal coordinates are fixed at compile time.
- Sensor thresholds and motor iteration counts are hardware-specific constants.
- Motor control uses blocking loops and fixed delays rather than timer-driven motion control.
- The exploration policy does not guarantee a globally shortest route.
- The internal map has a fixed compile-time capacity.
- The firmware depends on CodeVisionAVR-specific libraries and syntax.
- The current code assumes the original sensor polarity, motor wiring, and maze geometry.

## Historical Note

This is an archival robotics project. The original firmware is preserved unchanged in `legacy/`, while the maintained copy and supporting documentation make the implementation easier to inspect and understand. The repository is intended to document the design and learning outcomes of the original system rather than claim production-ready robotics software.

## Author

**Hoda Yamani**  
Machine learning, reinforcement learning, robotics, and intelligent systems

[GitHub](https://github.com/h-yamani)
