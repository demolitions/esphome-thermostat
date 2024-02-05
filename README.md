# esphome-thermostat

Smart thermostats aren't really smart nowadays, most of them rely heavily on automation systems to manage cutoff temps and heating schedule, making the devices themselves very simple digital thermostats.
I wanted to replace my old programmable thermostat with a smart one, but I wanted some things from it that I didn't find in any of the commercially available ones:

1. No reliance on external cloud services, especially since I already had a Home Assistant installation.
2. No reliance on network connection, I wanted my thermostat to retain full functionality even without wifi.
3. Fully programmable weekly scheduling.
4. Winter (full weekly scheduling), Manual (simple temperature management) and Summer (disabled) modes.
5. Comfort in programming said scheduling, none of that "press this single button to advance time, half hour at a time" that most of the old programmable thermostats had.
6. Full control and schedule management from Home Assistant or from the device web interface.
7. No batteries to change or recharge, fully powered from mains or low voltage PSU.

This repo is my take on a truly "smart" thermostat, completely (almost) independent from the network, fully programmable with a weekly schedule via touchscreen interface, there is no limit on the quantity of temperature configuration points during any day, but the small 320x240 screen has only space for a handful per day, and there is no scrolling, so...
The configuration points can be set for any hour, any minute and any temperature, and the thermostat will engage or disengage the heater relay according to the current configuration.

## Roadmap

The only things that are still missing are:
1. Full network independence, at the moment the thermostat relies on Home Assistant only for the initial date & time sync when it first powers up and when the RTC battery fails, there is no touchscreen menu to setup that.
2. Schedule management from Home Assistant or directly from the device web interface, currently only the instantaneous control is available from the network, which is more than enough for manual operation mode, but to change the daily schedule the only way is from touchscreen interface.

The second point especially is on hold until I'll be done with the complete refactoring, since the current version of ESPHome does not produce a working build (more on that in the firmware section).

## Hardware

### Macro hardware 
Things that could be either self-assembled or bought and used in breakout PCB form
- ESP32-NodeMCU (couldn't fit all of this on an ESP8266, sorry)
- 5v DC Relay
- DHT11 or DHT22 temperature sensor
- ILI9341 screen
- XPT2046 touch digitizer
- Active buzzer
- DS1307 RTC
- SPI SD Card unit

### Miscellaneous hardware
Things that are needed if the macro hardware was not in breakout pcb form
- IRF540N N-Channel MOSFET
- PC817 Optoisolator
- 1N4001 Diode
- Resistors

## Assembly

I found a ready made nice enclosure containing a ILI9341+XPT2046 touchscreen with an SPI SD Card unit, and a preassembled PCB sporting a power regulator, an active buzzer, a decent prototyping area, and ready to socket in an ESP32 in NodeMCU format or ESP8266 in D1 Mini format, all that in a single pretty cheap bundle, thanks AZ-Delivery for your AZ-Touch MOD, it worked wonderfully for this project.

That meant that on the prototyping area I had only to fit the 5v Relay with its control circuit (optoisolator and mosfet), the DHT22 and the DS1307 RTC, and even that has been a bit of a hassle: the DS1307 board comes with a battery holder that I had to desolder and move around the back of the PCB since the relay needed most of the prototyping area volume, I had to fit the RTC under the display and the battery holder was bringing the RTC board a bit too near the SD Card reader.

Another issue has been that mounting the DHT22 inside of the enclosing (there was enough space and the enclosing comes with nice air ventilation grilles on all 4 corners) where the ESP32 and the display were working, heating the air around them and that pretty soon started masking the room temperature reading, so I had to move the temperature sensor outside of the enclosing, temporarily suspending it from 3-4cm of 4x0.22 shielded cable, more than enough to put it in clear air, but it's not very nice to see, so I'll try to see if I can put it around the back of the unit in future revisions.

## Firmware

I initially wrote the firmware in Arduino IDE, then I migrated it to PlatformIO since Arduino IDE isn't that comfortable to work in for project bigger than a sketch.

Then I started using ESPHome for my DIY Shutter Drivers (more on them another time, on another repo), so I started transitioning all the thermostat code to ESPHome.

BIG. MISTAKE.

The current codebase is frozen, since both the ILI9341 and the XPT2046 components inside ESPHome changed drastically (partially dropping APIs and not providing alternatives) in late 2022 and I haven't been able to get a working build since then, either I lose long tap detection or my code doesn't build, and I'm not satisfied with either outcome, so a new refactor is in the works to port everything back to PlatformIO, while trying to maintain OTA updating like the one ESPHome provides, which is not that easy.
That, and the fact that my work and life keeps me working on other things, and the currently deployed thermostat is at full functionality, means that work on this project is reeeeeeeaaaally sloooooooow...

