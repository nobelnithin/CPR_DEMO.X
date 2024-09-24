 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include <util/delay.h>

#define F_SENSOR_PIN 7  // PA7 corresponds to pin 7 (physical pin 8)
#define LED_PIN 6       // PA6 for LED
#define BUZZER_PIN 2  // PA1 for Buzzer
#define BUZZER_HIGH 1

// Function to initialize the ADC
void adc_init(void) {
    ADC0.CTRLA = 0x00;
    ADC0.CTRLC = 0x50;
    ADC0.MUXPOS = ADC_MUXPOS_AIN7_gc;
    ADC0.CTRLA = 0x01;  // Enable ADC
}

// Function to read ADC value from the force sensor
uint16_t adc_read(void) {
    ADC0.COMMAND = ADC_STCONV_bm;  // Start conversion
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)) {}  // Wait for conversion to finish
    return ADC0.RES;  // Return the ADC result
}

int main(void) {
    uint16_t adc_value;
    float force;
    uint16_t led_timer = 0;  // Timer for LED blinking
    bool led_state = false;  // LED state (on/off)

    // Initialize peripherals
    PORTA_set_pin_dir(F_SENSOR_PIN, PORT_DIR_IN);  // PA7 as input for force sensor
    PORTA_set_pin_dir(LED_PIN, PORT_DIR_OUT);      // PA6 as output for LED
    PORTA_set_pin_dir(BUZZER_PIN, PORT_DIR_OUT);   // PA1 as output for Buzzer
    PORTA_set_pin_dir(BUZZER_HIGH, PORT_DIR_OUT); 
    adc_init();  // Initialize ADC
    
    while (1) {
        // Read the analog value from the force sensor every 10 ms
        adc_value = adc_read();
        force = (float)adc_value;

        // Control the LED and buzzer based on the force
        if (force > 1019.0) {  // Example threshold
            PORTA_set_pin_level(BUZZER_HIGH, true);
            PORTA_set_pin_level(BUZZER_PIN, true);  // Turn on Buzzer
        } else {
            PORTA_set_pin_level(BUZZER_PIN, false);  // Turn off Buzzer
        }

        // Handle LED blinking every 500 milliseconds
        if (led_timer >= 400) {
            led_state = !led_state;  // Toggle LED state
            PORTA_set_pin_level(LED_PIN, led_state);  // Update LED state
            led_timer = 0;  // Reset the timer after 500 ms
        }

        // Increment the timer
        _delay_ms(10);  // Keep the 10 ms delay for ADC reading and timer increment
        led_timer += 10;  // Increment the LED timer by 10 ms (same as ADC read delay)
    }

    return 0;
}
