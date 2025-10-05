I made a controller for my home AV equipment with a couple of buttons to toggle between off, watching TV, watching projector or listening to music.

It ran on an Arduino and used IR and RF to control the devices.
It also controlled som relays to only have power on the devices that was actually in use.
It used an external real-time-clock to now what time of day it is combined with a motion sensor to automatically turn the power on/off to the internet router at certen times during the day and when someone was home.

This repository is a backup of all my Arduino code (both experiments and the actual AVController) and datasheets from from when I made the controller.
Not all experiemtns and datasheets are relevant to the controller.

Notes:
The main file is AVController.ino.
My board is a Arduino Nano with a ATmega328P with an old bootloader.
To set the time, enable debugging (define debugRtc) and send a send a character using the serial monitor during startup. Use "No Line Endings" in the serial monitor.
