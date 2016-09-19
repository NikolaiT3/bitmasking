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
#include <time.h>

// Macro definitions to ensure portablity between both sun.cs and linux.cs

#if defined(sun)
    #define CLOCKNAME CLOCK_HIGHRES
        
#else
    #define CLOCKNAME CLOCK_PROCESS_CPUTIME_ID
#endif

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

//The code segment which implements the decision logic
inline void control_action ( int inputType )
{
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

inline void read_inputs_from_ip_if ( int inputType )
{
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

inline void write_output_to_op_if ( int inputType )
{
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
 
int main ( int argc, char *argv[] )
{
	unsigned int cpu_mhz;
	unsigned long long int begin_time, end_time;
	struct timespec timeDiff,timeres;
    struct timespec time1, time2, calibrationTime;
	
	int inputType = 1;
	printf( "Type 0 for console i/o or 1 for file i/o: " );
	scanf( "%i", &inputType );
	
	if ( inputType == 0 )	// console input/output
	{
		for ( ; ; )
		{
			clock_gettime(CLOCKNAME, &time1);
			clock_gettime(CLOCKNAME, &time2);
			calibrationTime = diff(time1,time2); //calibration for overhead of the function calls
			clock_getres(CLOCKNAME, &timeres);  // get the clock resolution data
		
			read_inputs_from_ip_if( 0 );
			
			clock_gettime(CLOCKNAME, &time1); // get current time
			control_action( 0 );
			clock_gettime(CLOCKNAME, &time2);   // get current time
			
			write_output_to_op_if( 0 );
			
			timeDiff = diff(time1,time2); // compute the time difference

			printf("Timer Resolution = %u nanoseconds \n ",timeres.tv_nsec);
			printf("Calibrartion time = %u seconds and %u nanoseconds \n ", calibrationTime.tv_sec, calibrationTime.tv_nsec);
			printf("The measured code took %u seconds and ", timeDiff.tv_sec - calibrationTime.tv_sec);
			printf(" %u nano seconds to run \n", timeDiff.tv_nsec - calibrationTime.tv_nsec);
		}
	}
	else if ( inputType == 1 )	// file input/output
	{
		input = fopen( "lab1_prob5_input.txt", "r" );
		output = fopen( "lab1_prob5(prob3code)_output.txt" , "w" );
		
		fprintf( output, "Variable sequence is: | DOS | DSBF | ER | DC | KIC | DCL | BP | CM |\n\n" );
		int i = 0;
		for ( ; i < 8; ++i )
		{
			fprintf( output , "Test %i:\n" , i + 1 );
			
			clock_gettime(CLOCKNAME, &time1);
			clock_gettime(CLOCKNAME, &time2);
			calibrationTime = diff(time1,time2); //calibration for overhead of the function calls
			clock_getres(CLOCKNAME, &timeres);  // get the clock resolution data
		
			read_inputs_from_ip_if( 1 );
			
			clock_gettime(CLOCKNAME, &time1); // get current time
			control_action( 1 );
			clock_gettime(CLOCKNAME, &time2);   // get current time
			
			write_output_to_op_if( 1 );
			
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
	}
	
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