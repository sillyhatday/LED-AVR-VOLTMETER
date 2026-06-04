# AVR Voltmeter 0 - 25.5v DC

## Universal ELV Voltmeter

After doing other more complex AVR based projects, I wanted to do something simpiler and useful. For a while now I've wanted a small bench top PSU, which I found with the help of @nataliethenerd. To go along with that, I thought a bench voltmeter would be useful.
This is my tiny bench voltmeter for some simple bench top testing.

It has a resolution of 0.1v and an accuracy of +/-0.1v. I've plenty of multimeters with 0.01v and 0.001v accuracy, but they aren't always needed for some work. Often you just need a vague idea of the voltage on certain things or simply measuring if a bettery is good or bad,
the exact voltage value is not important. For more accurate work, the multimeter will have to come out. That said this could be good for others setting up a work space to get a very cheap bench top multimeter. You don't need expensive tools to do a lot of projects and
tinkering.

This is however not for AC voltages and most certainly not for anything that is over 50v. It will not be a good experience for the user or the instrument.

---

## Hardware

| Part Description | Part Number | Package | Quantity |
| --------------------- | ------------------ | ----------- | ----------- |
| Microcontroller | ATmega8A | DIP-28 | 1 |
| 3x 7 Segment Display | SR420561N | ?? | 1 |
| Resistors | 470R | L6.3xD2.5xP10.16 | 8 |
| Resistors | 1K | L6.3xD2.5xP10.16 | 1 |
| Resistors | 10K | L6.3xD2.5xP10.16 | 7 |
| Capacitors | 20pF | D3.4xW2.1xP2.5 | 2 |
| Capacitors | 100nF | D3.0xW2.0xP2.5 | 3 |
| Capacitors | 1uF | ?? | 2 |
| Capacitors | 10uF | ?? | 1 |
| Buttons | PS-7054DVB-6PN | ?? | 2 |

Needs to be updated

---

## Functional Description

This is an amazing starter project for anyone tinkering with AVRs. It's super simple in terms of components and code. THe input voltage measurement only requires dividing down with a resistor array. The AVR ADC reads that value and displays it on screen.

The ADC mode is set to 8-bit. 10-bit mode could be used, but it would make maths more complex and there would be no real gain in accuracy. The internal AVR voltage reference is not accurate enough and the resistors on the input divider would need to be of good
quality; stable and accurate.

The ADC value directly maps to the voltage on the input and the voltage displayed. Input voltage is divided down by 10 to be in the analog reference range. The ADC value is 100 times larger than that voltage. The ADC value is written directly to the screen
and a decimal place is added to the second digit on screen. The AVR has no clue that the displayed voltage is visually 10 times smaller. There is no maths or floating point numbers being used internally.

There is no need for the NPN transistors on the screen cathodes, they can just as well be controlled by the MCU directly, but I like having all logic in code the same. That being on is one and off is zero.

The project code is honestly a mess. I learned a lot doing this and have since learned more. It's currently half rewritten in a better way using state machines. For now, this works well enough.

## Progress

It’s actively being used and keeping an eye out for bugs.

### Done
- Build a functional voltmeter that can show voltage readings.

### Working On
- PCB revision needed for more resistor options on the input voltage divider.
- Case for it to be mounted. Current setup is temporary until another project progresses from @nataliethenerd

### Next
- Nothing. Possible higher voltage version, higher accuracy version or current meter.

### Known Bugs
- None.

---

## Photos

### Current Build

<img width="450" height="800" alt="bench voltmeter" src="https://github.com/user-attachments/assets/fbe6d835-897a-4407-85e8-e051cbf1133e" />

Used in the desktop voltmeter setup with the small bench top PSU. Case not final. Waiting for Nataliethenerd to finish her benchtop meter project.

<img width="800" height="450" alt="battery bank project meter on show" src="https://github.com/user-attachments/assets/6ce47ff5-cd3a-474c-96f7-18fa377fbc5e" />
<img width="800" height="450" alt="battery bank project meter on show" src="https://github.com/user-attachments/assets/fa627fce-6e6c-4bf5-ab00-0296125c459f" />
<img width="800" height="450" alt="battery bank project voltmeter" src="https://github.com/user-attachments/assets/5218a833-cb18-40e3-9f62-fd864813825b" />

---

## Code

- Tools: Arduino IDE 1.8.19 – MiniCore for ATMEGA8

---
