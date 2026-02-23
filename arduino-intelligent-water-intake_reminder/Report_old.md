# Smart Water Bottle Reminder

## Overview
The Smart Water Bottle Reminder project aims to encourage users to stay hydrated throughout the day, especially during long working hours. By attaching a device to a water bottle, it periodically reminds the user to drink water at regular intervals.

## Pin Configuration
Ensure the components are connected to the correct pins on the Arduino Nano:

- **Tilt Sensor:** Connected to pin 2
- **Green LED:** Connected to pin 3
- **Yellow LED:** Connected to pin 4
- **Vibration Motor:** Connected to pin 5
- **Red LED:** Connected to pin 6
- **Piezo Buzzer:** Connected to pin 7

Verify and adjust pin configurations in the Arduino code (`const int` declarations) if necessary.

## Features
- Reminds the user to drink water after specific intervals
- Utilizes LEDs, a piezo buzzer, and a vibration motor for notifications
- Automatically resets after the bottle is picked up
- Detects bottle placement and tilting using sensors

## Components

- **Arduino Nano:** Powers the system and controls the components
- **LEDs:** Provide visual reminders in different colors
- **Piezo Buzzer:** Generates audible alerts to prompt the user
- **Vibration Motor:** Provides tactile feedback for notifications
- **Tilt Sensor:** Detects bottle movement for activation

## Installation
1. Connect the components according to the circuit diagram.
2. Upload the provided Arduino code to the Arduino Nano.
3. Attach the device securely to the water bottle.

## Usage
1. Place the water bottle on the desk.
2. The green LED blinks after 1 hour of inactivity, reminding the user to drink water.
3. If there's no response, the yellow LED blinks continuously with vibration for 20 seconds.
4. If still ignored, the red LED blinks continuously with the buzzer sounding every 5 seconds for 20 seconds.
5. Pick up the bottle to reset the system and stop the reminders.

## Configuration
- Adjust the inactivity threshold in the Arduino code (`inactivityThreshold`) to change the reminder frequency.
- Modify pin configurations in the code if necessary.

## Contributors
- Shah Zaman Haider - Project Developer

## License
This project is licensed under the MIT License - see the LICENSE file for details.
