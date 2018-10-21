#include <stdio.h>
//#include <limits.h>
#include "pa2.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#define TINYFP_INF_NAN(t)	((((t) >> 3) & 0x0f) == 0x0f)
#define FLOAT_INF_NAN(f)	((((f) >> 23) & 0xff) == 0xff)
#define CHECK_INF_NAN(f,t)	TINYFP_INF_NAN(t) && FLOAT_INF_NAN(f) && \
							((((t) >> 7) == (f) >> 31) && \
							(((((t) & 0x07) == 0) && (((f) & 0x007fffff) == 0)) || \
							(((t) & 0x07) && ((f) & 0x007fffff))))

#define N 	6

int int_literal[N] = {1, -20, 67, 149, 241, -252};
tinyfp int2tinyfpAnswer[N] = {0b00111000, 0b11011010, 0b01101000, 0b01110001, 0b01110111, 0b11111000}; 

tinyfp tinyfp_literal[N] = {0b10000000, 0b00011110, 0b11101010, 0b01010101, 0b01111000, 0b01111111};
int tinyfp2intAnswer[N] = {0, 0, -80, 13, -2147483648, -2147483648};

float float_literal[N] = {0.001953125, 0.000732421875, -12.345, 1.6875, -0.0/0.0, 314.0};
tinyfp float2tinyfpAnswer[N] = {0b00000001, 0b00000000, 0b11010100, 0b00111110, 0b11111001, 0b01111000};


tinyfp tinyfp_literal2[N] = {0b00000010, 0b00010000, 0b11101010, 0b10000000, 0b01111000, 0b11111100};
float tinyfp2floatAnswer[N] = {0.00390625, 0.03125, -80.0, -0.0, 1.0/0.0, 0.0}; 

int main(void){
	int idx;
	tinyfp tf;
	int i;
	union {
		unsigned int i;
		float f;
	} u;

	printf("\nTest 1: casting from int to tinyfp\n");
	for (idx = 0; idx < N; idx++)
	{
		i = int_literal[idx];
		tf = int2tinyfp(i);
		printf("int("BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(i >> 24));
		printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(i >> 16));
		printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(i >> 8));
		printf(BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(i));
		printf("tinyfp("BYTE_TO_BINARY_PATTERN"), ", BYTE_TO_BINARY(tf));
		printf("%s\n", (tf == int2tinyfpAnswer[idx])? "CORRECT" : "WRONG");
	}

	printf("\nTest 2: casting from tinyfp to int\n");
	for (idx = 0; idx < N; idx++)
	{
		tf = tinyfp_literal[idx];
		i = tinyfp2int(tf);
		printf("tinyfp("BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(tf));
		printf("int("BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(i >> 24));
		printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(i >> 16));
		printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(i >> 8));
		printf(BYTE_TO_BINARY_PATTERN"), ", BYTE_TO_BINARY(i));
		printf("%s\n", (i == tinyfp2intAnswer[idx])? "CORRECT" : "WRONG");
	}

	printf("\nTest 3: casting from float to tinyfp\n");
	for (idx = 0; idx < N; idx++)
	{
		u.f = float_literal[idx];
		tf = float2tinyfp(u.f);
		printf("float("BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(u.i >> 24));
		printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(u.i >> 16));
		printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(u.i >> 8));
		printf(BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(u.i));
		printf("tinyfp("BYTE_TO_BINARY_PATTERN"), ", BYTE_TO_BINARY(tf));
		if (FLOAT_INF_NAN(u.i))
			printf("%s\n", CHECK_INF_NAN(u.i, tf)? "CORRECT" : "WRONG");
		else 
			printf("%s\n", (tf == float2tinyfpAnswer[idx])? "CORRECT" : "WRONG");
	}

	printf ("\nTest 4: casting from tinyfp to float\n");
	for (idx = 0; idx < N; idx++)
	{
		tf = tinyfp_literal2[idx];
		u.f = tinyfp2float(tf);
		printf("tinyfp("BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(tf));
		printf("float("BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(u.i >> 24));
		printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(u.i >> 16));
		printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(u.i >> 8));
		printf(BYTE_TO_BINARY_PATTERN"), ", BYTE_TO_BINARY(u.i));
		if (TINYFP_INF_NAN(tf))
			printf("%s\n", CHECK_INF_NAN(u.i, tf)? "CORRECT" : "WRONG");
		else 
			printf("%s\n", (u.f == tinyfp2floatAnswer[idx])? "CORRECT" : "WRONG");
	}

	return 0;
}
