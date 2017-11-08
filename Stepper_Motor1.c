//************************************************************************************ 
// Control of stepper motors unipolar and bipolar using 
// I2C to parallel converter (chip PCF8574A) and power driver (chip L293D).
// Schematics and simulations made in Proteus.
//
// By Moises Reyes. 
//
// NOTA: Stepper_Motor1_lib.h file has inside routines, allowing a clean view of main project.
//************************************************************************************

#include <16F886.h>
#fuses NOWDT,NOPROTECT          // To clarified the meaning of this "Configuration´s BITS"
#fuses NOLVP,INTRC              // go to the datasheet and also on MPLAB following the path
#fuses MCLR,NOBROWNOUT          // MPLAB-> Window-> Pic memory views->
#fuses NOIESO,NOFCMEN,DEBUG     // configuration bits

#use delay(clock=8000000)       // Inform to our compiler the clock frequency to be used  (8Mhz)
#define ocho_MHz 0b01110001     // Define the value of OSCCON register. Setting the internal clock 
                                // as source of system clock, running at 8MHz.
/* Setup I2C */
#use I2C(MASTER, sda=PIN_C4, scl=PIN_C3, FAST=2000000) 

#include <Stepper_Motor1_lib.h>

void main(){ 
    
#byte OSCCON = ocho_MHz;   // The register OSCCON take the value previously defined.
    
// A LED blinks indicating that the program has started.   
output_high(PIN_A0);
delay_ms(1000);
output_low(PIN_A0);

creacion_arreglos_secuencias(); // Create the sequences arrays.
    
 while (TRUE) { 
// The switch connected to pin B2 controls the bipolar motor. // EL switch conectado al pin B2 controla al motor bipolar, cuando esta en nivel alto permite el giro.
// The switch connected to pin B2 controls the unipolar motor. // EL switch conectado al pin B3 controla al motor uniipolar, cuando esta en nivel alto permite el giro.     
// If both switches are closed (high level), then both motors spin. // Si ambos switch estan cerrados, entonces ambos motores giran.

// If both switches are closed (high levels in pins B2 and B3)    
// Ambos switch cerrados permitiendo niveles altos en B2 y B3
    if(input(PIN_B2) & input(PIN_B3)){
		ambos_motores_move();	// Both motors spin. // Se llama a la funcion que mueve ambos motores.	
    }
    else{ 
	// If one switch is closed (high level in pin B2 or B3)
    // Uno de los switch esta cerrado permitiendo un nivele alto en B2 o B3
        if(input(PIN_B2) | input(PIN_B3)){        
    // Evaluation of pin B2.
	// Se evalua el estado del pin B2
            switch(input(PIN_B2)) {        
                case 1:     // Pin B2 = High, that means that Pin B3 = Low. // Pin B2 en nivel alto e implica que Pin B3 esta en nivel bajo.
                    solomove_MB();	// Move just bipolar motor. // Se llama a la funcion que mueve solo el motor bipolar.
                break;

                case 0:     // Pin B2 = Low, that means that Pin B3 = High. // Pin B2 en nivel bajo e implica que Pin B3 esta en nivel alto.
                    solomove_MU();	// Move just unipolar motor. // Se llama a la funcion que mueve solo el motor unipolar.
                break;
            }
        }
    }
  }    
} 