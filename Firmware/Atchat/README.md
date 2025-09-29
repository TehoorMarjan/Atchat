# Atchat Firmware (ATtiny85)

This firmware is part of the Atchat project, a system designed to protect your
cat's food from neighborhood intruders. The main project uses object recognition
with a webcam to detect cats and distinguish your own cat from others. If a
rogue cat is detected approaching the food bowl, a water spray is activated to
deter the trespasser.
![3D CAD model of a cat deterrent water sprayer assembly featuring a blue lever labeled "Atchat !", a cylindrical reservoir, a servo motor mounted on a base plate, and mechanical linkages connecting the servo to the lever.](../../CAD/SprayerV2/SprayerV2.png)

## Overview

This firmware runs on an ATtiny85 microcontroller. It is responsible for
controlling a servo motor that operates the water spray mechanism. The firmware
communicates with the host (e.g., a Raspberry Pi or PC running the object
recognition software) via simple AT commands.

## Features

- **AT Command Interface:**
  - `AT` — Basic connectivity test, returns "OK".
  - `AT+VER?` — Returns the firmware version. Use this to verify communication
    and firmware compatibility.
  - `AT+S=<angle>` — Sets servo to specified angle (0-180°). Use for direct
    servo control or spray activation.
- **Servo Control:**
  - **0° = Standby**: Spray handle is released, no spraying.
  - **90°-120° = Active**: Spray handle is pulled down, triggering water spray.
    - 90° is sufficient for activation
    - 100° provides better pressure and tolerance for loose wires
    - 120° may cause excessive wear on the mechanism

## Usage

1. **Connect the ATtiny85** to your main controller via USB (DigiCDC appears as
   serial port).
2. **Send AT commands** from your host software:
   - `AT+S=0` — Set servo to standby (no spray)
   - `AT+S=100` — Activate spray (recommended angle)
   - `AT+VER?` — Check firmware version
3. **Integrate with object recognition**: The host software should send
   `AT+S=100` when a rogue cat is detected, and `AT+S=0` to stop spraying.

## Implementation Details

- **Minimal AT Parser**: Custom lightweight implementation to fit ATtiny85's 8KB
  flash memory
- **DigiCDC Communication**: USB CDC (Serial) interface for reliable host
  communication
- **Servo_ATTinyCore**: Optimized servo library for ATtiny85 microcontroller
- **Memory Optimized**: Designed to work within ATtiny85's constraints (8KB
  flash, 512B RAM)

## Dependencies

- Servo_ATTinyCore (included in lib/)
- DigiCDC (part of Digispark framework)

## Known Issues

**Important note about compilation:** If you encounter an error like
`usbdrvasm.asm:21: Error: unknown opcode 'end'`, this is because the
`usbdrvasm.asm` file in the DigiCDC library is designed for the IAR compiler,
not for avr-gcc. To fix this issue, I've not found better than to delete the
problematic file.

## License

MIT License. See [LICENSE](../LICENSE) for details.

---

_Protect your cat, keep the food safe, and deter unwanted feline visitors!_
