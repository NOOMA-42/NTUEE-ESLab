/*
 * Copyright (c) 2014-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"

// Adjust pin name to your board specification.
// You can use LED1/LED2/LED3/LED4 if any is connected to PWM capable pin,
// or use any PWM capable pin, and see generated signal on logical analyzer.

PwmOut PwmPin (PWM_OUT);
  
int main() {
    PwmPin.period(0.05f);  // Set 
    
    while(1){ //infinite loop
                 
        PwmPin.write(0.75f);       // Set the duty cycle to 75%
        PwmPin.write(0.933f);       // Set the duty cycle to 93%
        PwmPin.write(1.0f);       // Set the duty cycle to 100%
        PwmPin.write(0.933f);       // Set the duty cycle to 93%
        PwmPin.write(0.75f);       // Set the duty cycle to 75%
        PwmPin.write(0.50f);       
        PwmPin.write(0.25f);       
        PwmPin.write(0.066f);       
        PwmPin.write(0.0f);       
        PwmPin.write(0.066f);       
        PwmPin.write(0.25f);       
        PwmPin.write(0.50f);       

    }      
} 