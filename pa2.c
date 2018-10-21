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

#include <stdio.h>
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

tinyfp int2tinyfp(int x) {
    tinyfp y;
    if (x > 240) {
        //+infinity
        printf("+infinite%d\n",x);
        y = 0x78;
        return y;
    }
    else if (x < -240) {
        //-infinity
        printf("-infinite%d\n",x);
        y = 0xf8;
        return y;
    }
    else if (x == 0) {
        y = 0x0;
        return y;
    }
    else {
        printf("normal%d\n",x);
        float xx;
        unsigned char exp = 0;
        unsigned char rsign = 0;
        unsigned char man = 0;
        if (x > 0) {
            rsign = 0;
        }
        else {
            rsign = 1;
        }

           if ((x>=1) && (x<2)) {
               xx = x/16.0f;
               exp = 7;
           }
           else if((x>=2) && (x<4)) {
               xx = x/32.0f;
               exp = 8;
           }
           else if((x>=4) && (x<8)) {
               xx = x/64.0f;
               exp = 9;
           }
           else if((x>=8) && (x<16)) {
               xx = x/128.0f;
               exp = 10;
           }
           else if((x>=16) && (x<32)) {
               xx = x/256.0f;
               exp = 11;
           }
           else if((x>=32) && (x<64)) {
               xx = x/512.0f;
               exp = 12;
           }
           else if((x>=64) && (x<128)) {
               xx = x/1024.0f;
               exp = 13;
           }
           else if((x>=128) && (x<256)) {
               xx = x/2048.0f;
               exp = 14;
           }
        xx = 8*xx-8;
        if ((xx>=0) && (xx <= 0.5)) {
            man = 0;
        }
        else if ((xx>0.5) && (xx < 1.5)) {
            man = 1;
        }
        else if ((xx>=1.5) && (xx <= 2.5)) {
            man = 2;
        }
        else if ((xx>2.5) && (xx < 3.5)) {
            man = 3;
        }
        else if ((xx>=3.5) && (xx <= 4.5)) {
            man = 4;
        }
        else if ((xx>4.5) && (xx < 5.5)) {
            man = 5;
        }
        else if ((xx>=5.5) && (xx <= 6.5)) {
            man = 6;
        }
        else if ((xx>6.5) && (xx < 7.5)) { //little strange
            man = 7;
        }
        else {
            exp+=1;
            man = 0;
        }
        y = ((rsign<<7)|(exp<<3)|(man));
        return y;




    }

}


int tinyfp2int(tinyfp x) {
    unsigned int exp = ((x>>3) & 0xf);
    unsigned int sign = ((x>>7) & 0x1);
    unsigned int mantissa = (x & ((1<<3)-1));
    int y;
    if (exp == 0xf) {
        printf("TMin(infinite, NaN)\n");
        y = -2147483648;
        return y;
    }
    else if (exp == 0x0) {
        printf("0 it includes denormalized\n");
        y = 0;
        return y;
    }
    else {
        printf("normal\n");
        int rexp = exp-7;
        float a= 1.0f;
        if (rexp >= 0) {
            for (int i = 0; i < rexp; i++) {
                a*=2.0f;
            }
        }
        else {
            int rrexp = -rexp;
            for (int i = 0; i < rrexp; i++) {
                a*=0.5f;
            }
        }
        float m = (float) mantissa;
        float rman = ( m / 8.0f) + 1.0;
        if (sign == 0) {
            int re = (int) (rman*a);
            return re;
        }
        else {
            int re = (int) (-rman*a);
            return re;
        }
    }


    return 2;
}


tinyfp float2tinyfp(float x) {
    printf("%f",x);
    tinyfp y;
    if ((x == 0x7f800000) || (x > 240)) {
        //+infinity
        printf("+infinite\n");
        y = 0x78;
        return y;
    }
    else if ((x == 0xff800000) || (x < -240) ) {
        //-infinity
        printf("-infinite\n");
        y = 0xf8;
        return y;
    }



    union{
        float f;
        unsigned int i;
    }u;

    u.f = x;

    unsigned int isign = ((u.i >> 31) & 0x1);
    unsigned int imantissa = (u.i & ((1<<23)-1));
    unsigned int iexp = ((u.i >> 23) & 0xff);
    /*printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(isign >> 16));
    printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(isign >> 8));
    printf(BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(isign));
    printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(imantissa >> 16));
    printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(imantissa >> 8));
    printf(BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(imantissa));
    printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(iexp >> 16));
    printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(iexp >> 8));
    printf(BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(iexp));*/

    unsigned char tsign, tman, texp;
    tsign = (unsigned char) isign;
    texp = (unsigned char) iexp;


    if (isign == 0) {
        if (iexp == 0) {
            if (imantissa == 0) {//+0
                printf("+0\n");
                y = 0;
                return y;
            }
            else {
                //denormalized form
                printf("+denormal\n");


            }

        }
        else if (iexp == 0xff) {//+NaN
            printf("+NaN\n");
            y = 0x79;
            return y;
        }
        else { //normal

        }
    }
    else {
        if (iexp == 0) {
            if (imantissa == 0) {//-0
                printf("-0\n");
                y = 0x80;
                return y;
            }
            else {
                //denormalized form
                printf("-denormal\n");

            }

        }
        else if (iexp == 0xff) {//-NaN
            printf("-NaN\n");
            y = 0xf9;
            return y;

        }
        else {
            printf("-normal\n");

        }
    }
    float k = imantissa/1048576.0;
    if ((k>=0) && (k <= 0.5)) {
        tman = 0;
    }
    else if ((k>0.5) && (k < 1.5)) {
        tman = 1;
    }
    else if ((k>=1.5) && (k <= 2.5)) {
        tman = 2;
    }
    else if ((k>2.5) && (k < 3.5)) {
        tman = 3;
    }
    else if ((k>=3.5) && (k <= 4.5)) {
        tman = 4;
    }
    else if ((k>4.5) && (k< 5.5)) {
        tman = 5;
    }
    else if ((k>=5.5) && (k <= 6.5)) {
        tman = 6;
    }
    else if ((k>6.5) && (k < 7.5)) { //little strange
        tman = 7;
    }
    else {
        texp+=1;
        tman=0;
    }
    y = ((tsign<<7)|(texp<<3)|(tman));
    return y;
}

//1. deal with exp 0000 2. deal with exp bias value
float tinyfp2float(tinyfp x) { //disconnect x into sign bit, exp, and mantissa and change it to float

    //check sign bit of tinyfp <(x>>7) != 0>
    //check mantissa part of tinyfp <x & ((1<<3)-1)>
    //check exponent part of tinyfp < (x>>3) & 0x0f>

    //check sign bit for float (i>>31)!=0
    //check mantissa for float i & ((1<<23)-1)
    //check exponent for float (i>>23) & 0xff

    //we do &0xff to take the positive value in sign bit 1

    unsigned int sign = ((x>>7) & 0x1);
    unsigned int mantissa = (x & ((1<<3)-1));
    unsigned int exp = ((x>>3) & 0xf);
    //printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(sign >> 16));
    //printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(sign >> 8));
    //printf(BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(sign));
    //printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(mantissa >> 16));
    //printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(mantissa >> 8));
    //printf(BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(mantissa));
    //printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(exp >> 16));
    //printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(exp >> 8));
    //printf(BYTE_TO_BINARY_PATTERN") => ", BYTE_TO_BINARY(exp));

    unsigned int fsign, fexp,fmantissa;

    union{
        float f;
        unsigned int i;
    }u;

    if (sign == 0) {
        if (exp == 0x0) {
            if (mantissa == 0x0) {
                printf("+zero\n");
                return 0.0f;
            }
            else {
                printf("+denormalized\n");
                return (float)mantissa/512.0f;
                if (mantissa == 1) {
                    return 1.0f/512.0f;
                }
                else if (mantissa == 2){
                    return 2.0f/512.0f;
                }
                else if (mantissa == 3){
                    return 3.0f/512.0f;
                }
                else if (mantissa == 4) {
                    return 3.0f/512.0f;
                }
                else if (mantissa == 5) {
                    return 5.0f/512.0f;
                }
                else if (mantissa == 6){
                    return 6.0f/512.0f;
                }
                else if (mantissa == 7){
                    return 7.0f/512.0f;
                }
                return 2.0;
            }
        }
        else if (exp == 0xf){
            if (mantissa == 0x0) {
                //+infinity
                printf("+inifite\n");
                u.i = 0x7f800000;
                return u.f;
            }
            else{
                //+NaN
                printf("+NaN\n");
                fexp = (0xff<<23);
                u.i = ( fexp | mantissa);
                return u.f;

            }
        }
        else {
            printf("+normal\n");
            fexp = ((exp-7+127)<<23);
            fmantissa = (mantissa << 20);
            u.i = ( fexp | fmantissa);
            return u.f;
        }
    }
    else {
        if (exp == 0x0) {
            if (mantissa == 0x0) {
                printf("-zero\n");
                return -0.0f;
            }
            else {
                printf("-denormal\n");
                return -(float)mantissa/512.0f;
                if (mantissa == 1) {
                    return -1.0f/512.0f;
                }
                else if (mantissa == 2){
                    return -2.0f/512.0f;
                }
                else if (mantissa == 3){
                    return -3.0f/512.0f;
                }
                else if (mantissa == 4) {
                    return -3.0f/512.0f;
                }
                else if (mantissa == 5) {
                    return -5.0f/512.0f;
                }
                else if (mantissa == 6){
                    return -6.0f/512.0f;
                }
                else if (mantissa == 7){
                    return -7.0f/512.0f;
                }
            }
        }
        else if (exp == 0xf){
            if (mantissa == 0x0) {
                printf("-infinite\n");
                u.i = 0xff800000;
                return u.f;
            }
            else{
                //-NaN
                printf("-NaN\n");
                fsign = (1<<31);
                fexp = (0xff<<23);
                u.i = (fsign| fexp | mantissa);
                return u.f;
            }
        }
        else {
            printf("-normal\n");
            fsign = (1<<31);
            fexp = ((exp-7+127)<<23);
            fmantissa = (mantissa << 20);
            u.i = (fsign | fexp | fmantissa);
            return u.f;
        }
    }

}


