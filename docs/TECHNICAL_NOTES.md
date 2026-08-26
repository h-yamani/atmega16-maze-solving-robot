# Technical Notes

## Hardware represented by the firmware

The source indicates the following system configuration:

- ATmega16 eight-bit AVR microcontroller
- Two independently driven stepper motors connected through `PORTB`
- Four analogue obstacle sensors connected to ADC channels 2, 3, 5, and 7
- Two analogue floor/path sensors connected to ADC channels 4 and 6
- 16-character alphanumeric LCD
- Custom two-level PCB and differential-drive chassis

The exact sensor models, motor-driver components, supply voltage, clock frequency, and full pin schematic were not included in the archived repository and should not be inferred without further project records.

## Internal map

The controller stores navigation state in:

```c
char location[10][10][5];
```

For each cell, indices `0–3` represent directions relative to the robot's recorded heading:

| Index | Direction |
|---:|---|
| 0 | Front |
| 1 | Right |
| 2 | Left |
| 3 | Back |
| 4 | Heading when the cell was recorded |

Direction entries use character states:

- `'0'`: available and unexplored
- `'1'`: previously selected/visited
- `'3'`: unavailable, blocked, or excluded
- zero-initialised value: not evaluated yet

The `set()` function rotates stored relative directions when the same cell is approached with a different heading.

## Motion model

The motor arrays define four-phase step sequences. Forward motion advances both motors through their sequences. Turning reverses the phase progression of one motor for a calibrated number of steps before executing a forward cell movement.

These counts are mechanical calibration values rather than physical units:

- `218` iterations for one grid-cell translation
- `70` iterations for a turn
- `4 ms` delay per phase

## Known maintenance considerations

The cleaned source preserves the original control behaviour, but future work should consider:

- replacing magic characters with named enums or constants;
- replacing global state with small navigation, sensor, and motor structures;
- separating ADC, LCD, motor, sensing, and planning code into modules;
- resetting goal-command state after executing a goal-directed move;
- reviewing signed modulo operations used when reversing motor phase indices;
- adding explicit bounds checks before every map access;
- replacing blocking motor loops with timer-driven control where practical;
- recording the original circuit diagram and calibration procedure if available.

Any behavioural refactor should be validated against the original hardware or an accurate simulator.

