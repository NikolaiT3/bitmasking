//***************************************************
// Name			: Nicholas Warner
// Date			: 20 September 2014
// Subject		: CSCE 312-501
// Assignment	: Lab 1
// Updated		: 23 September 2014
// Description	: Implementation of software control
//				: structure for automobile hardware
//***************************************************

#include <stdio.h> //This is useful to do i/o to test the code 

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

void read_inputs_from_ip_if ( void )
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

// would link to actuators in actual use - writes values to text file lab1_prob4_output.txt
void write_output_to_op_if ( void )
{
	fprintf( output, "Actuator Decimal Representation: %d\n" , actuator_outputs );
	fprintf( output, "The state of the BELL is: %u\n", getBit( actuator_outputs, 0 ) );
	fprintf( output, "The state of the DLA is:  %u\n", getBit( actuator_outputs, 1 ) );
	fprintf( output, "The state of the BA is:   %u\n", getBit( actuator_outputs, 2 ) );
}

//The code segment which implements the decision logic
void control_action ( void )
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

int main ( int argc, char *argv[] )
{
	input = fopen( "lab1_prob4_input.txt", "r" );
	output = fopen( "lab1_prob4_output.txt" , "w" );
	
	fprintf( output, "Variable sequence is: | DOS | DSBF | ER | DC | KIC | DCL | BP | CM |\n\n" );
	
	int i = 0;
	for ( ; i < 8; ++i )	// to evaluate the 8 different test cases
	{
		fprintf( output , "Test %i:\n" , i + 1 );
		fprintf( output , "| Integer from file, Bit from sensor_inputs |\n" );
		read_inputs_from_ip_if();
		control_action();
		write_output_to_op_if();
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