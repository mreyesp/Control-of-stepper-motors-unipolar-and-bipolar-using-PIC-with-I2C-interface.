//************************************************************************************ 
// Library stepper motors unipolar and bipolar using 
// I2C to parallel converter (chip PCF8574A) and power driver (chip L293D).
// Schematics and simulations made in Proteus.
//
// By Moises Reyes. 
//
// NOTA: Stepper_Motor1_lib.h file has inside routines, allowing a clean view of main project.
//************************************************************************************


// Clockwise rotation sequence for unipolar stepper motor.
// Secuencia de giro sentido horario para motor unipolar    
int Secuencia_MU[] = {0b0001,0b0010,0b0100,0b1000};
// Clockwise rotation sequence for bipolar stepper motor.
// Secuencia de giro sentido horario para motor bipolar
int Secuencia_MB[] = {0b0101,0b0110,0b1010,0b1001};
// The both previously sequences (clockwise rotation unipolar and bipolar) are stored in this array.
// Arreglo que guarda la unificacion de ambas secuencias para los motores unipolar y bipolar.
int secuencia_motores[] = {0,0,0,0};    // Se inicializa con 4 bytes con 0 para asignar largo y uso de memoria.
// Array to keep the sequence for unipolar stepper motor.
// Arreglo que guarda la secuencia para activar solo motor unipolar
int secuencia_solomov_MU[] = {0,0,0,0};
// Array to keep the sequence for bipolar stepper motor.
// Arreglo que guarda la secuencia para activar solo motor bipolar
int secuencia_solomov_MB[] = {0,0,0,0};
	
// Address of I2C PCF chip used.
// Direccion I2C del chip PCF a utilizar	
	int PCF_Address = 0x7E; 	
							
// Variable used in delay function. Avoid the failure caused for the speed of logical changes, because the motors 
// can not react to fast (can not follow the speed of logical changes). In this case the value is high to allow a clean simulation on Proteus.
// Variable usada en la funcion delay para evitar que los cambios logicos sean mas rapidos que la reaccion de los motores
// pudiendo generar fallas. En este caso es un valor muy alto para poder generar la correcta simulacion en Proteus.
int espera = 500;   
                                               
int8 i = 0;

// The creation of separated and unified sequences arrays make possible execute calculations before the program execution. After those calculations
// the microcontroller will have fixed variables and will run faster, using just more storage memory to keep the arrays.
// It is a compromise between execution speed and storage memory.
// La creacion de los arreglos de secuencias para cada caso permite realizar las operaciones matematicas rutinarias
// antes del comienzo de la ejecucion del programa, de esta manera el microcontrolador dispondra de variables
// fijas, sacrificando memoria al almacenar estos arreglos pero teniendo menos carga de procesamiento durante
// la ejecucion del programa.
creacion_arreglos_secuencias(){
// Operation to combine two nibbles to create a Byte.
// Sequence for unipolar stepper motor -> higher 4 bits.
// Sequence for bipolar stepper motor -> lower 4 bits.
// En el ciclo for se realiza operacion binaria para formar un byte desde dos nibbles guardando en un arreglo la unificacion
// de ambas secuencias de motores.
// el nibble que posiciona al motor unipolar ocupa los 4 bits mas altos
// el nibble que posiciona al motor bipolar ocupa los 4 bits mas bajos
    for(i=0;i<4;i++){
        secuencia_motores[i] = (Secuencia_MU[i]<<4)|Secuencia_MB[i];
    }
// Masking the lower nibble. Just the unipolar sequence is saved.
// La operacion AND con el valor 0xF0 solo habilita los 4 bits mas altos
// dejando en valor 0 los 4 bits mas bajos (hace una operacion de enmascarar).
// lo que permite que solo la secuencia del motor unipolar sea guardada con sus
// respectivos bits en la posicion correcta dentro del byte
    for(i=0;i<4;i++){
        secuencia_solomov_MU[i] = (secuencia_motores[i] & 0xF0);
    }

// Masking the higher nibble. Just the bipolar sequence is saved.
// La operacion AND con el valor 0x0F solo habilita los 4 bits mas bajos
// dejando en valor 0 los 4 bits mas altos (hace una operacion de enmascarar).
// lo que permite que solo la secuencia del motor bipolar sea guardada con sus
// respectivos bits en la posicion correcta dentro del byte
    for(i=0;i<4;i++){
        secuencia_solomov_MB[i] = (secuencia_motores[i] & 0x0F);
    }
return(0);
}

ambos_motores_move(){
    i2c_start();                // Start the I2C comunication. // Comienza la comunicacion I2C.
    i2c_write(PCF_Address);     // Send the I2C address of the used chip PCF8574A. // Se envia la direccion I2C para el chip PCF8574A usado.
    for(i=0;i<4;i++){
        i2c_write(secuencia_motores[i]);   // Positioning instruction for both motors. // Se envia la instruccion de posicionamiento para ambos motores.
        delay_ms(espera);
    }
    i2c_stop(); // End of I2C communication. // Termina la comunicacion I2C. 
return(0);
}
	
solomove_MB(){
    i2c_start();                // Start the I2C comunication. // Comienza la comunicacion I2C. 
    i2c_write(PCF_Address);     // Send the I2C address of the used chip PCF8574A. // Se envia la direccion I2C para el chip PCF8574A usado.
    for(i=0;i<4;i++){
        i2c_write(secuencia_solomov_MB[i]);     // Positioning instruction for bipolar motor, the unipolar motor stays still.
												// Se envia la instruccion de posicionamiento para el motor bipolar, el motor unipolar se mantiene quieto.                                                                                      
        delay_ms(espera);                    
    }
    i2c_stop();     // End of I2C communication. // Termina la comunicacion I2C. 
return(0);
}

solomove_MU(){
    i2c_start();                // Start the I2C comunication. // Comienza la comunicacion I2C. 
    i2c_write(PCF_Address);     // Send the I2C address of the used chip PCF8574A. // Se envia la direccion I2C para el chip PCF8574A usado.
    for(i=0;i<4;i++){
        i2c_write(secuencia_solomov_MU[i]);     // Positioning instruction for uniipolar motor, the bipolar motor stays still.
												// Se envia la instruccion de posicionamiento para el motor unipolar, el motor bipolar se mantiene quieto.                                                                                                      
        delay_ms(espera);                    
    }
    i2c_stop();     // End of I2C communication. // Termina la comunicacion I2C. 
return(0);
}