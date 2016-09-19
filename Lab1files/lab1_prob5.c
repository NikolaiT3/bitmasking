//***************************************************
// Name			: Nicholas Warner
// Date			: 20 September 2014
// Subject		: CSCE 312-501
// Assignment	: Lab 1
// Updated		: 23 September 2014
// Description	: Implementation of software control
//				: structure for automobile hardware
//***************************************************

//run this file as: gcc FileName.c -o ExecutableName -lrt 

#include <stdio.h>
#include <time.h>

// Macro definitions to ensure portablity between both sun.cs and linux.cs

#if defined(sun)
    #define CLOCKNAME CLOCK_HIGHRES
        
#else
    #define CLOCKNAME CLOCK_PROCESS_CPUTIME_ID
#endif

// Sensor and Actuator Variables
unsigned int sensor_inputs = 0;			// in order | DOS | DSBF | ER | DC | KIC | DCL | BP | CM |
										//		bit:	0		1	2	3		4	5		6	7
unsigned int actuator_outputs = 0;		// in order | BELL | DLA | BA |
										//		bit:	0		1	2
// File variables
FILE *input;
FILE *output;

// for debugging, returns the value of the bit (0 or 1) in n at the place specified
int getBit ( int n, int place )
{
	int mask = 1<<place;	// sets a 1 at the place you want to get the value from
	if ( ( mask & n ) > 0 )	// will only be true if there is a 1 in the correct place
		return 1;
	else
		return 0;
}

//The code segment which implements the decision logic
inline void control_action ( void )
{
	actuator_outputs = 0;
	int mask = 0xc0; // 192, 11000000
	
	// Brake Actuator
	if ( ( sensor_inputs & mask ) == 192 )
	{
		actuator_outputs = actuator_outputs | 1<<2;
	}
	else
	{
		actuator_outputs = actuator_outputs | 0<<2;
	}
	
	mask = 0x31; // 49, 00110001
	
	// Door Lock Actuator
	if ( ( sensor_inputs & mask ) == 49 )
	{
		actuator_outputs = actuator_outputs | 1<<1;
	}
	else if ( ( sensor_inputs & mask ) == 33 )
	{
		actuator_outputs = actuator_outputs | 1<<1;
	}
	else if ( ( sensor_inputs & mask ) == 32 )
	{
		actuator_outputs = actuator_outputs | 1<<1;
	}
	else
	{
		actuator_outputs = actuator_outputs | 0<<1;
	}
	
	mask = 0xe; // 14, 00001110
	
	// BELL
	if ( ( sensor_inputs & mask ) == 6 )
	{
		actuator_outputs = actuator_outputs | 1<<0;
	}
	else if ( ( sensor_inputs & mask ) == 4 )
	{
		actuator_outputs = actuator_outputs | 1<<0;
	}
	else
	{
		actuator_outputs = actuator_outputs | 0<<0;
	}
}

inline void read_inputs_from_ip_if ( void )
{
	sensor_inputs = 0;
	int i = 0;
	int var = 0;
	for ( ; i < 8 ; ++i )
	{
		fscanf( input , "%i" , &var );
		fprintf( output, " %i," , var );
		if ( var == 1 )
			sensor_inputs = sensor_inputs | 1<<i;
		else
			sensor_inputs = sensor_inputs | 0<<i;
		fprintf( output, " %i |", getBit( sensor_inputs, i ) );
	}
	fprintf( output, "\nSensor Decimal representation: %d \n", sensor_inputs );
}

inline void write_output_to_op_if ( void )
{
	fprintf( output, "Actuator Decimal Representation: %d\n" , actuator_outputs );
	fprintf( output, "The state of the BELL is: %u\n", getBit( actuator_outputs, 0 ) );
	fprintf( output, "The state of the DLA is:  %u\n", getBit( actuator_outputs, 1 ) );
	fprintf( output, "The state of the BA is:   %u\n", getBit( actuator_outputs, 2 ) );
}

/*timespec diff from
http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
*/
struct timespec diff( struct timespec start , struct timespec end )
 {
    struct timespec temp;
    //the if condition handles time stamp end being smaller than than 
    //time stamp start which could lead to negative time.

     if ((end.tv_nsec-start.tv_nsec)<0) {
          temp.tv_sec = end.tv_sec-start.tv_sec-1;
          temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
      } else {
          temp.tv_sec = end.tv_sec-start.tv_sec;
          temp.tv_nsec = end.tv_nsec-start.tv_nsec;
      }
  return temp;
 }
 
 int main ( int argc , char *argv[] )
{
	unsigned int cpu_mhz;
	unsigned long long int begin_time, end_time;
	struct timespec timeDiff,timeres;
    struct timespec time1, time2, calibrationTime;
	
	input = fopen( "lab1_prob5_input.txt", "r" );
	output = fopen( "lab1_prob5_output.txt" , "w" );
	
	fprintf( output, "Variable sequence is: | DOS | DSBF | ER | DC | KIC | DCL | BP | CM |\n\n" );
	
	int i = 0;
	for ( ; i < 8; ++i )	// to evaluate the 8 different test cases
	{
		fprintf( output , "Test %i:\n" , i + 1 );
		fprintf( output , "| Integer from file, Bit from sensor_inputs |\n" );
		
		clock_gettime(CLOCKNAME, &time1);
		clock_gettime(CLOCKNAME, &time2);
		calibrationTime = diff(time1,time2); //calibration for overhead of the function calls
		clock_getres(CLOCKNAME, &timeres);  // get the clock resolution data
		
		read_inputs_from_ip_if(); // get the sensor inputs
		
		clock_gettime(CLOCKNAME, &time1); // get current time
		control_action();                 // process the sensors
		clock_gettime(CLOCKNAME, &time2);   // get current time

		write_output_to_op_if();    // output the values of the actuators
		
		timeDiff = diff(time1,time2); // compute the time difference

		printf("Timer Resolution = %u nanoseconds \n ",timeres.tv_nsec);
		printf("Calibrartion time = %u seconds and %u nanoseconds \n ", calibrationTime.tv_sec, calibrationTime.tv_nsec);
		printf("The measured code took %u seconds and ", timeDiff.tv_sec - calibrationTime.tv_sec);
		printf(" %u nano seconds to run \n", timeDiff.tv_nsec - calibrationTime.tv_nsec);
		
		fprintf( output , "Timer Resolution = %u nanoseconds \n",timeres.tv_nsec);
		fprintf( output , "Calibrartion time = %u seconds and %u nanoseconds \n", calibrationTime.tv_sec, calibrationTime.tv_nsec);
		fprintf( output , "The measured code took %u seconds and ", timeDiff.tv_sec - calibrationTime.tv_sec);
		fprintf( output , "%u nano seconds to run \n", timeDiff.tv_nsec - calibrationTime.tv_nsec);
		
		fprintf( output , "\n" );
	}
	
	fclose( input );
	fclose( output );
	
	return 0;
}

/*NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNN                                               NNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNN    Nicholas Warner * warnern@tamu.edu         NNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNN                                               NNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNN         NNNNNNNNNN         NNNNNNNNNN         NNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN         NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN         NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNN         NNNNNNNNNN         NNN       DNNNNNNN       NNNNNNNNNND
NNNNNNNNNNNNNNNNDO      NNNNNNNNNNN         NNNM$       NNNN=       NDNNNNNNNNND
NNNNNNNNNNNNNNNNN        NNNNNNNNNN         NNNNO       ZNNN        NNNNNNNNNNND
NNNNNNNNNNNNNNNN          NNNNNNNNN         NNNNO        DD         NNNNNNNNNNND
NNNNNNNNNNNNNNN7    NN    DNNNNNNNN         NNNNO                   NNNNNNNNNNND
NNNNNNNNNNNNNNN    7NN:    NNNNNNNN         NNNNO    N        N     NNNNNNNNNNND
NNNNNNNNNNNNND              NNNNNNN         NNNNO    NN      NN     NNNNNNNNNNND
NNNNNNNNNNNNN:              $NNNNNN         NNNNO    NNN     NN     NNNNNNNNNNND
NNNNNNNNNNNND    NNNNNNN7    NNNNNN         NNNNO    NNN    NNN     NNNNNNNNNNND
NNNNNNNNNDDN     NNNNNNNN     DDNNN         NNND$    NDNN  NNND     DNNNNNNNNNND
NNNNNNNNNN        NNNNNN        NNN         NNN       ZNNNNNNN       NNNNNNNNNND
NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN         NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNDNNNN         NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNNNNNNNNNNNNNNNN                   NNNNNNNNNNNNNNNNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNNNNNNNNNNNNNNNN                   NNNNNNNNNNNNNNNNNNNNNNNNNNNNNND
NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN*/