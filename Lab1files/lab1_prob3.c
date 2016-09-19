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

// Available Sensors
unsigned int driver_on_seat;			// DOS
unsigned int driver_seat_belt_fastened;	// DSBF
unsigned int engine_running;			// ER
unsigned int doors_closed;				// DC
unsigned int key_in_car;				// KIC
unsigned int door_lock_lever;			// DLC
unsigned int brake_pedal;				// BP
unsigned int car_moving;				// CM

// Available Actuators
unsigned int bell;						// BELL
unsigned int door_lock_actuator;		// DLA
unsigned int brake_actuator;			// BA

// File variables
FILE *input;
FILE *output;


void read_inputs_from_ip_if ( int inputType )
{

	//place your input code here
	//to read the current state of the available sensors
	
	if ( inputType == 0 )
	{
		printf( "Is the driver on the seat?\n" );
		scanf( "%i", &driver_on_seat );
		
		printf( "Is the driver's seat belt fastened?\n" );
		scanf( "%i", &driver_seat_belt_fastened );
		
		printf( "Is the engine running?\n" );
		scanf( "%i", &engine_running );
		
		printf( "Are the doors closed?\n" );
		scanf( "%i", &doors_closed );
		
		printf( "Are the keys in the ignition?\n" );
		scanf( "%i", &key_in_car );
		
		printf( "Is the door lock lever pressed?\n" );
		scanf( "%i", &door_lock_lever );
		
		printf( "Is the brake pressed?\n" );
		scanf( "%i", &brake_pedal );
		
		printf( "Is the car moving?\n" );
		scanf( "%i", &car_moving );
		
		printf( "\n" );
	}
	else if ( inputType == 1 )
	{
		fscanf( input , "%i" , &driver_on_seat );
		fprintf( output , "| %i | " , driver_on_seat );
		
		fscanf( input , "%i" , &driver_seat_belt_fastened );
		fprintf( output , "%i | " , driver_seat_belt_fastened );
		
		fscanf( input , "%i" , &engine_running );
		fprintf( output , "%i | " , engine_running );
		
		fscanf( input , "%i" , &doors_closed );
		fprintf( output , "%i | " , doors_closed );
		
		fscanf( input , "%i" , &key_in_car );
		fprintf( output , "%i | " , key_in_car );
		
		fscanf( input , "%i" , &door_lock_lever );
		fprintf( output , "%i | " , door_lock_lever );
		
		fscanf( input , "%i" , &brake_pedal );
		fprintf( output , "%i | " , brake_pedal );
		
		fscanf( input , "%i" , &car_moving );
		fprintf( output , "%i |\n" , car_moving );
	}
}

void write_output_to_op_if ( int inputType )
{

	//place your output code here
    //to display/print the state of the 3 actuators (DLA/BELL/BA)
	
	if ( inputType == 0 )
	{
		printf( "The state of the DLA is %i\n", door_lock_actuator );
		printf( "The state of the BELL is %i\n", bell );
		printf( "The state of the BA is %i\n", brake_actuator );
		
		printf( "\n\n" );
	}
	else if ( inputType == 1 )
	{
		fprintf( output, "The state of the BELL is %u\n", bell );
		fprintf( output, "The state of the DLA is %u\n", door_lock_actuator );
		fprintf( output, "The state of the BA is %u\n", brake_actuator );
	}
}


//The code segment which implements the decision logic
void control_action ( int inputType )
{
	/*
	The code given here sounds the bell when driver is on seat 
	AND hasn't closed the doors. (Requirement-2)
	Replace this code segment with your own code to do problems 3 and 4.
	*/

	bell = 0;
	door_lock_actuator = 0;
	brake_actuator = 0;
	
	// Requirement 1, 2, 3
	if ( ( engine_running == 1 ) && ( driver_seat_belt_fastened == 0 ) && ( doors_closed == 0 ) )
	{
		bell = 1;
	}
	else if ( ( engine_running == 1 ) && ( doors_closed == 0 ) ) // Requirement 2 - modified, from DOS to ER
	{
		bell = 1;
	}
	else if ( engine_running == 0 )
	{
		bell = 0;
	}
	
	// Requirement 4
	if ( ( driver_on_seat == 0 ) && ( key_in_car == 1 ) )
	{
		door_lock_actuator = 0;
	}
	else if ( door_lock_lever == 1 )
	{
		door_lock_actuator = 1;
	}
	
	// Requirement 5
	if ( brake_pedal && car_moving )
	{
		brake_actuator = 1;
	}
	else
	{
		brake_actuator = 0;
		}
}

/* ---     You should not have to modify anything below this line ---------*/

int main ( int argc, char *argv[] )
{
	
	/*The main control loop which keeps the system alive and responsive for ever, 
	until the system is powered off */
	
	int inputType = 1;
	printf( "Type 0 for console i/o or 1 for file i/o: " );
	scanf( "%i", &inputType );
	
	if ( inputType == 0 )	// console input/output
	{
		for ( ; ; )
		{
			read_inputs_from_ip_if( 0 );
			control_action( 0 );
			write_output_to_op_if( 0 );

		}
	}
	else if ( inputType == 1 )	// file input/output
	{
		input = fopen( "lab1_prob3_input.txt", "r" );
		output = fopen( "lab1_prob3_output.txt" , "w" );
		
		fprintf( output, "Variable sequence is: | DOS | DSBF | ER | DC | KIC | DCL | BP | CM |\n\n" );
		int i = 0;
		for ( ; i < 8; ++i )
		{
			fprintf( output , "Test %i:\n" , i + 1 );
			read_inputs_from_ip_if( 1 );
			control_action( 1 );
			write_output_to_op_if( 1 );
			fprintf( output , "\n" );
		}
		
		fclose( input );
		fclose( output );
	}
	
	return 0;
}
