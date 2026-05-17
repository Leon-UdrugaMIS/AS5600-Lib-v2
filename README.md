# AS5600-Lib-v2

New version of the AS5600 library for Arduino with added wraparound, calibration,
working-zero, and EEPROM persistence helpers.

## What’s included
- Base AS5600 / AS5600L driver from the foundation library.
- `AS5600Calibrator` for wraparound-aware calibration between 0° and 180°.
- EEPROM-backed storage for calibration and working-zero settings.

## Examples
- `examples/AS5600_calibration/AS5600_calibration.ino` shows the full calibration
  flow described in the README AI document.
