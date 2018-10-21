//--------------------------------------------------------------
// 
//  4190.308 Computer Architecture (Fall 2018)
//
//  Project #2: TinyFP (8-bit floating point) Representation
//
//  October 16, 2018
//
//  Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//--------------------------------------------------------------

#ifndef _PA_2_H_
#define _PA_2_H_

typedef unsigned char tinyfp;


// cast int to tinyfp
tinyfp int2tinyfp(int x);

// cast tinyfp to int
int tinyfp2int(tinyfp x);

// cast float to tinyfp
tinyfp float2tinyfp(float x);

// cast tinyfp to float
float tinyfp2float(tinyfp x);



#endif // _PA_2_H
