// -------------------------------------------------------------------------------------------------------- //
//                                                                                                          //
//                                      ATmega8A 7 Segment Display                                          //
//                                      Basic Voltmeter Display V1.0                                        //
//                                                                                                          //
// -------------------------------------------------------------------------------------------------------- //

// ATMEGA8 input resistors, 1M & ~111K + trimmer, 25.6V in -> 2.56V out to ADC, scaling 10:1
// ADC: 0 = 0V, ADC: 255 = 2.56V. 2.56 / 255 = 0.01004
// ADC 1 bit every 0.1004V pre voltage divider.       ADC 1 bit every 0.1004V pre divider

// ATMEGA328 input resistors, 1M & ~44K + trimmer, 25.6V in -> 1.1V out to ADC, scaling 23.27:1
// ADC: 0 = 0V, ADC: 255 = 1.10V. 1.10 / 255 = 0.00431
// ADC 1 bit every 0.00431V post voltage divider.     ADC 1 bit every 0.1003V pre divider

// ***************************************** Include/Define *********************************************** //

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// **************************************** Display Constants ********************************************* //

const uint8_t NUM_DISPLAYS =                        3;                         // Number of displays

// ****************************************** Character Data ********************************************** //

const uint8_t ANODES[11] = {             // DP G F E D C B A
                                            0b00111111,                        // 0
                                            0b00000110,                        // 1
                                            0b01011011,                        // 2
                                            0b01001111,                        // 3
                                            0b01100110,                        // 4
                                            0b01101101,                        // 5
                                            0b01111101,                        // 6
                                            0b00000111,                        // 7
                                            0b01111111,                        // 8
                                            0b01101111,                        // 9
                                            0b00000000,                        // 10 OFF
};
const uint8_t CATHODES[3] = {
                                            0b10000000,                        // Display 1
                                            0b01000000,                        // Display 2
                                            0b00100000,                        // Display 3
};
const uint8_t DECIMAL =                     0b10000000;                        // Decimal bit mask
const uint8_t DISPLAY_OFF =                         10;

// **************************************** Sensor Constants ********************************************** //

const uint8_t SAMPLE_NUM =                          16;                        // Number of ADC samples
const uint8_t SAMPLE_SHIFT =                         4;                        // Divide for average (sum / 16)

// **************************************** Display Variables ********************************************* //

uint8_t digits[3] =                          {0, 0, 0};                        // What each digit is to display
volatile uint8_t currentDigit =                      0;                        // Digit lit up now

// **************************************** Global Variables ********************************************** //

volatile uint16_t heartBeat =                        0;                        // Counter to trigger ADC reading

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Functions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ //

void setup(void);
void loop(void);
uint16_t voltageReading(void);
void displayLogic(void);
void multiplex(void);

// ============================================== Setup =================================================== //

void setup() {

  // Voltage reference - mega8 2.56v internal - mega328 1.1v internal
  // Left adj for 8 bit, precaler 128, PC0 analog in
  ADMUX = (1 << REFS0)|(1 << REFS1)|(1 << ADLAR);
  ADCSRA = (1 << ADEN)|(1 << ADSC)|(1 << ADPS0)|(1 << ADPS1)|(1 << ADPS2);
  while (ADCSRA & (1 << ADSC));        // While bit 6 (ADSC) is true, AND bit 6 to check if true - stability read

  // Anode Pin Setup
  // Arduino Pin -> 8, 9, 10, 11, 12 ,13 , PB6, PB7  -  Port Pin: PB7...0
  DDRB = 0b11111111;
  PORTB = 0b00000000;

  // Sensor Pin Setup
  // Arduino Pin -> A0  -  Port Pin: PC0
  DDRC = 0b00000000;
  PORTC = 0b00111110;
  
  // Cathode Pin Setup
  // Arduino Pin -> 7, 6, 5   -   Port Pin: PD7...3
  DDRD = 0b11100000;
  PORTD = 0b00000000;
/*
  // ATmega328 Interrupts & Misc
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 124;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS21);
  TIMSK2 |= (1 << OCIE2A);
  ACSR |= (1 << ACD);
  sei();
*/
  // ATmega8 Interrupts & Misc
  cli();
  TCCR2 = 0;                                                  // Reset timer 2 registers
  TCNT2 = 0;
  TIMSK = 0;
  OCR2 = 124;                                                 // Compare match value
  TCCR2 |= (1 << WGM21)|(1 << CS21);                          // Timer CTC mode enable, Prescaler 8 - 2ms@2MHz OCR2@250
  TIMSK |= (1 << OCIE2);                                      // Enable timer 2 interrupt
  ACSR |= (1 <<  ACD);                                        // Comparator disable
  sei();

  // Sleep settings
  set_sleep_mode(SLEEP_MODE_IDLE);                             // Select sleep mode
  sleep_enable();                                              // Enable ability to sleep
}

// ========================================= Timer2 Interrupts ============================================ //
/*
// ATmega328
ISR(TIMER2_COMPA_vect) {
  heartBeat++;
  multiplex();
}
*/

// ATmega8
ISR(TIMER2_COMP_vect) {
  heartBeat++;                                                 // Increment counter each interrupt
  multiplex();                                                 // Continue updating screen each interrupt
}
// ============================================== Main ==================================================== //

void loop() {

  if (heartBeat >= 1000) {                                     // When counter reaches 1000
    heartBeat = 0;                                             // Reset counter
    displayLogic();                                            // Update display buffer & read ADC
  }
  sleep_cpu();
}
// ============================================ Multiplex ================================================= //

void multiplex() {

  uint8_t num = digits[currentDigit];                          // What number is to be displayed 0 - 9
  uint8_t anodes = ANODES[num];                                // Pull data from array for value in num
  uint8_t cathodes = CATHODES[currentDigit];                   // Pull data from array for which digit to activate

  if (currentDigit == 1) {                                     // Determine if DP is needed
    anodes |= DECIMAL;                                         // Bit mask DP with num
  }

  PORTD = 0;                                                   // Turn off cathodes
  PORTB = anodes;                                              // write anode outputs to data in anodes
  PORTD = cathodes;                                            // write cathode output for current digit high
  currentDigit++;                                              // Increment to next digit

  if (currentDigit >= NUM_DISPLAYS) {
    currentDigit = 0;
  }
}
// =========================================== Display Buffer ============================================= //

void displayLogic() {

  uint16_t reading = 0;

  reading = voltageReading();

  digits[2] = (reading / 100) % 10;                             // Extract hundreds, tens, units
  digits[1] = (reading / 10) % 10;                              // input data is 10x scaled
  digits[0] = reading % 10;                                     // Therefore, it's tens, units and tenths
}
// ========================================== Voltage Reading ============================================= //

uint16_t voltageReading() {

  uint16_t sum = 0;

  for (uint8_t i = 0; i < SAMPLE_NUM; i++) {                    // Take X readings
    ADCSRA = ADCSRA | (1 << ADSC);                              // Enable ADC
    while (ADCSRA & (1 << ADSC));                               // Wait for ADC to finish
    sum += ADCH;                                                // Sum readings from ADC
  }

  sum = (sum >> SAMPLE_SHIFT);                                  // Divide readings for average
  return sum;
}
