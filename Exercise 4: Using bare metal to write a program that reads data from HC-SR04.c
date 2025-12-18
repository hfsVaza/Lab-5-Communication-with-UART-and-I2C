
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define TRIG_DDR  DDRD
#define TRIG_PORT PORTD
#define TRIG_PIN  PD7

#define ECHO_DDR  DDRB
#define ECHO_PINR PINB
#define ECHO_PIN  PB0

static void pwm_init_timer2(void) {
	DDRD |= (1 << PD3); // OC2B output

	// Fast PWM 
	TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);
	// Prescaler 64 -> ~976 Hz PWM
	TCCR2B = (1 << CS22);

	OCR2B = 0; // start OFF
}

static void fan_set(uint8_t duty) {
	OCR2B = duty; 
}

static void io_init(void) {
	TRIG_DDR |= (1 << TRIG_PIN);      
	ECHO_DDR &= ~(1 << ECHO_PIN);     
}

static void trig_pulse_10us(void) {
	TRIG_PORT &= ~(1 << TRIG_PIN);
	_delay_us(2);
	TRIG_PORT |= (1 << TRIG_PIN);
	_delay_us(10);
	TRIG_PORT &= ~(1 << TRIG_PIN);
}

static uint32_t echo_pulse_us(uint32_t timeout_us) {
	// wait for echo HIGH
	while (!(ECHO_PINR & (1 << ECHO_PIN))) {
		if (timeout_us-- == 0) return 0;
		_delay_us(1);
	}
	// measure HIGH width
	uint32_t w = 0;
	while (ECHO_PINR & (1 << ECHO_PIN)) {
		if (w++ >= 30000) break; // safety cap
		_delay_us(1);
	}
	return w;
}

static int16_t read_distance_cm(void) {
	trig_pulse_10us();
	uint32_t us = echo_pulse_us(30000);
	if (us == 0) return -1;
	return (int16_t)(us / 58); // cm
}

int main(void) {
	io_init();
	pwm_init_timer2();

	while (1) {
		int16_t cm = read_distance_cm();

		if (cm < 0 || cm > 20) {
			fan_set(0);             // OFF
			} else if (cm <= 4) {
			fan_set(255);           // max ON
			} else {
			// map 4..20 cm -> 255..0
			uint16_t x = (uint16_t)(cm - 4);  // 0..16
			uint8_t duty = (uint8_t)(255 - (255 * x) / 16);
			fan_set(duty);
		}

		// every 2s
		for (uint8_t i = 0; i < 20; i++) _delay_ms(100);
	}
}
