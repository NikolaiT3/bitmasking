//*************************
// Nicholas Warner
// 10 September 2014
// CSCE 312-501
// Lab 1, Problem 2
//*************************

#include <stdio.h>
#include <sys/time.h>

int main ( void )
{
	char one;
	unsigned int two;
	long three;
	long long four;
	double five;
	float six;
	struct timeval seven;
	
	FILE* filep;
	if ( (filep = fopen("lab1_prob2_out.txt", "w")) == NULL)
	{
		printf("Error opening the file\n");
		exit(1);
	}

	fprintf( filep, "char data type is %d bytes or %d bits long\n", sizeof(one), sizeof(one)*8 );
	fprintf( filep, "unsigned int data type is %d bytes or %d bits long\n", sizeof(two), sizeof(two)*8 );
	fprintf( filep, "long data type is %d bytes or %d bits long\n", sizeof(three), sizeof(three)*8 );
	fprintf( filep, "long long data type is %d bytes or %d bits long\n", sizeof(four), sizeof(four)*8 );
	fprintf( filep, "double data type is %d bytes or %d bits long\n", sizeof(five), sizeof(five)*8 );
	fprintf( filep, "float data type is %d bytes or %d bits long\n", sizeof(six), sizeof(six)*8 );
	fprintf( filep, "timeval data type is %d bytes or %d bits long\n", sizeof(seven), sizeof(seven)*8 );
	
	return 0;
}