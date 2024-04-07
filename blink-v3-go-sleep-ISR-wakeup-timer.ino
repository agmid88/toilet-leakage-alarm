#include <avr/sleep.h>
#include <avr/interrupt.h>

#define LED_PIN PIN_PA2
#define WAKE_PIN PIN_PA1

volatile bool isAwake = false;

// ISR for pin change interrupt
ISR(PORTA_PORT_vect) {
  PORTA.INTFLAGS = PORT_INT1_bm;
  isAwake = (digitalRead(WAKE_PIN) == HIGH);
}
 
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(WAKE_PIN, INPUT);

  PORTA.PIN1CTRL = PORT_ISC_BOTHEDGES_gc;

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  digitalWrite(LED_PIN, LOW);

  isAwake = (digitalRead(WAKE_PIN) == HIGH);
}

void goToSleep() {
  sleep_disable();        
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Set sleep mode to 'power down'
  sleep_enable();          // Enable sleep mode

  sei();                   // Enable interrupts
  sleep_cpu();             // Put the CPU to sleep

  sleep_disable();         // Disable sleep mode
}

void loop() {
  static int postpone_counter = 0;
  if (isAwake) {
    if(postpone_counter > 100)
    {
      digitalWrite(LED_PIN, HIGH);
    }else{
      postpone_counter++;
    }
  }  else  {
    postpone_counter = 0;
    digitalWrite(LED_PIN, LOW);
    goToSleep();  // Enter sleep mode
  }

  delay(50);
}
