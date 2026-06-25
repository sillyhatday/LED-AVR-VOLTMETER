# AVR Voltmeter - 3 Digit - 7 Segment LED Displays - 0 to 25.5v DC

## Introduction

After doing other more complex AVR based projects, I wanted to do something simpiler. For a while now I've wanted a small bench top PSU, which I found with the help of @nataliethenerd. To go along with that, I thought a bench voltmeter would be useful.

It has a resolution of 0.1v and an accuracy of +/-0.1v. Often you just need a vague idea of voltage on certain things or simply measuring if a battery is good or bad, the exact voltage is not important. For more accurate work, a quality multimeter is needed.

I had a thought that this could be good for setting up a work space, to have a handy & cheap bench top multimeter on the cheap. You don't need expensive tools to do a lot of projects and tinkering. A lot of the time, a rough idea of voltage is all that is needed.

>[!WARNING]
>This does not work with any AC voltages. Do not use over 50v. Don't stick it in a mains wall socket.

---

## 🔨 Hardware

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

## 📓 Functional Description

>[!NOTE]
>This is an amazing starter project for anyone wanting to learn AVRs.

It's super simple in terms of components and code. The input voltage measurement only requires dividing down with a resistor array.

The ADC mode is set to 8-bit (Really just left shifted to only read one ADC register). 10-bit mode could be used, but it would make the math more complex for no real gain. The internal AVR voltage reference is not that accurate and the resistor divider network is likely built from +/-10% tolerance parts.

The ADC value read directly maps to the voltage. This means an ADC value of 157 = 15.7v. The ADC value is written directly to the screen and a decimal place is added to the second digit on screen. The AVR has no clue that the displayed voltage is a decimal number. This method means no floating point numbers are used.

I like to use NPN transistors on the display cathodes (common cathode display). This means that a logic 1 in code means on and 0 means off. If the output pins were sinking the cathode current, the logic is inverse. If you don't want to use transistors, bridge the collector and emmitor pads, then reverse the logic in the code.

>[!TIP]
>I have included code for using the newer ATmega328, this is untested. It will also require different input resistors.

## 🥇 Progress

### 🟢 Done
- Build a functional voltmeter that can show voltage readings.

### 🟡 Working On
- Case for it to be mounted. Current setup is temporary until another project progresses from @nataliethenerd

### ⚪ Next
- PCB revision needed for more resistor options on the input voltage divider.

---

## 📸 Photos

Used in the bench top voltmeter (not final enclosure) and my big power bank.

<img width="280" height="500" alt="bench voltmeter" src="https://github.com/user-attachments/assets/fbe6d835-897a-4407-85e8-e051cbf1133e" />
<img width="500" height="280" alt="battery bank project meter on show" src="https://github.com/user-attachments/assets/6ce47ff5-cd3a-474c-96f7-18fa377fbc5e" />
<img width="500" height="280" alt="battery bank project voltmeter" src="https://github.com/user-attachments/assets/5218a833-cb18-40e3-9f62-fd864813825b" />

---

- Tools: Arduino IDE 1.8.19 – MiniCore for ATMEGA8

---

## Links

🛍️ Inside Gadgets Shop: https://shop.insidegadgets.com <br>
📎 Inside Gadgets GitHub: https://github.com/insidegadgets <br>
📷 Inside Gadgets Insta: https://www.instagram.com/inside.gadgets <br>
🖇️ Bytendo Mods GitHub: https://github.com/bytendomods <br>
🐭 Bucket Mouse: https://github.com/Bucket-Mouse <br>
🛒 Natalie Shop: https://nataliethenerd.com/ <br>
🤓 Natalie GitHub: https://github.com/natalie-lang/natalie <br>
💡Danyk Project: https://danyk.cz/avr_adc_en.html
