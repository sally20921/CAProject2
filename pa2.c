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

tinyfp int2tinyfp(int x) {
    int X;
    tinyfp y;

    unsigned char exp = 0;
    unsigned char rsign = 0;
    unsigned char man = 0;
    union{
        float xx;
        unsigned int y;
    }u;

    if (x == 0) { //x == 0
        return  0x0;
    }
    else if (x>0) { //x>0
        X = x;

    }
    else {//x<0
        rsign = 1;
        X= -x;

    }

     {
           if ((X>=1) && (X<2)) {
               exp = 7;
               u.xx = X/1.0f;
           }
           else if((X>=2) && (X<4)) {
               exp = 8;
               u.xx = X/2.0f;
           }
           else if((X>=4) && (X<8)) {
               exp = 9;
               u.xx = X/4.0f;
           }
           else if((X>=8) && (X<16)) {
               exp = 10;
               u.xx = X/8.0f;
           }
           else if((X>=16) && (X<32)) {
               exp = 11;
               u.xx = X/16.0f;
           }
           else if((X>=32) && (X<64)) {
               exp = 12;
               u.xx = X/32.0f;
           }
           else if((X>=64) && (X<128)) {
               exp = 13;
               u.xx = X/64.0f;
           }
           else if((X>=128) && (X<256)) {
               exp = 14;
               u.xx = X/128.0f;
           }
         else {
               exp = 15;
           }
         u.y = (u.y & 0x7fffff);
         if ((u.y&0x80000) == 0x80000){ //4th bit is 1
            if (((u.y&0x7ffff))!=0x0) { //there is 1 in 5th bit or more
                u.y = ((u.y >> 20)&0x7)+1;
            }
             else {//round to even
                if ((u.y&0x100000) == 0x100000)  {
                    u.y = ((u.y >> 20)&0x7)+1;
                }
                else {
                    u.y = ((u.y >> 20)&0x7);
                }
            }
         }
         else {//4th bit is 0
             u.y = ((u.y >> 20)&0x7);
         }
         //the mantissa is over 3 digits
         if ((u.y & 0x8)==0x8) {
             u.y = 0x0;
             exp+=1;
         }
         if (exp > 14) {
             if (rsign == 0) {
                 return 0x78;
             }
             else {
                 return 0xf8;
             }
         }


        return ((rsign<<7)|(exp<<3)|(u.y&0x7));
    }

}


int tinyfp2int(tinyfp x) {
    unsigned int exp = ((x>>3) & 0xf);
    unsigned int sign = ((x>>7) & 0x1);
    unsigned int mantissa = (x & ((1<<3)-1));
    int y;
    if (exp == 0xf) {
        //printf("TMin(infinite, NaN)\n");
        y = -2147483648;
        return y;
    }
    else if (exp == 0x0) {
        //printf("0 it includes denormalized\n");
        return 0;
    }
    else {
        //printf("normal\n");
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

}


tinyfp float2tinyfp(float x) {

    tinyfp y;
    union{
        float f;
        unsigned int i;
    }u;
    u.f = x;

    unsigned int isign = ((u.i >> 31) & 0x1);
    unsigned int imantissa = (u.i &  ((1<<23)-1));
    int iexp = ((u.i >> 23) & 0xff);

    unsigned char rsign;
    unsigned char rexp;

    union{
        float manf;
        unsigned int mani;
    }man;
    float X;

    if (isign == 0 ) { //whatever it is, it is plus
        if (iexp == 0xff ) {
            if (imantissa == 0x0) { //+infinity
                //printf("+infinity");
                return 0x78;
            }
            else {
                //printf("+NaN");
               return 0x79;
            }
        }
        else if (iexp == 0x0) {
                return 0x0;

        }
        else {
            rsign = 0;
            X = x;
        }

    }
    else { //whatever it is, it is minus
        if (iexp == 0xff ) {
            if (imantissa == 0) { //-infinity
                //printf("-infinity");
                return 0xf8;
            }
            else {
                //printf("-NaN");
                return 0xf9;
            }
        }
        else if (iexp == 0x0) {
                return 0x80;

        }
        else {
            rsign = 1;
            X = -x;
        }
    }

    if ((0.0f<X)&&(X<1.0f/64.0f)) {
        rexp = 0x0;
        man.manf = X*64.0f+1;///////////

    }
        //normalized
    else if ((1.0f/64.0f<=X)&&(X<1.0f/32.0f)) {
                    rexp = 1;
                    man.manf = X*64.0f;
    }
    else if ((1.0f/32.0f<=X)&&(X<1.0f/16.0f)) {
                    rexp = 2;
                    man.manf = X*32.0f;
    }
    else if ((1.0f/16.0f<=X)&&(X<1.0f/8.0f)) {
                    rexp = 3;
                    man.manf = X*16.0f;
    }
    else if ((1.0f/8.0f<=X)&&(X<1.0f/4.0f)) {
                    rexp = 4;
                    man.manf = X*8.0f;

    }
    else if ((1.0f/4.0f<=X)&&(X<1.0f/2.0f)) {
                    rexp = 5;
                    man.manf = X*4.0f;

    }
    else if ((1.0f/2.0f<=X)&&(X<1.0f))  {
                    rexp = 6;
                    man.manf = X*2.0f;

    }
    else if ((X>=1) && (X<2)) {
            rexp = 7;
            man.manf = X/1.0f;
    }
    else if((X>=2) && (X<4)) {
            rexp = 8;
            man.manf= X/2.0f;
    }
    else if((X>=4) && (X<8)) {
            rexp = 9;
            man.manf= X/4.0f;
    }
    else if((X>=8) && (X<16)) {
            rexp = 10;
            man.manf = X/8.0f;
    }
    else if((X>=16) && (X<32)) {
            rexp = 11;
            man.manf = X/16.0f;
    }
    else if((X>=32) && (X<64)) {
            rexp = 12;
            man.manf = X/32.0f;
    }
    else if((X>=64) && (X<128)) {
            rexp = 13;
            man.manf = X/64.0f;
    }
    else if((X>=128) && (X<256)) {
            rexp = 14;
            man.manf = X/128.0f;
    }
    else {
            rexp = 15;
    }
    man.mani = (man.mani & 0x7fffff);
    if ((man.mani&0x80000) == 0x80000){ //4th bit is 1
            if (((man.mani&0x7ffff))!=0x0) { //there is 1 in 5th bit or more
                man.mani = ((man.mani >> 20)&0x7)+1;
            }
            else {//round to even
                if ((man.mani&0x100000) == 0x100000)  {
                    man.mani = ((man.mani >> 20)&0x7)+1;
                }
                else {
                    man.mani = ((man.mani >> 20)&0x7);
                }
            }
    }
    else {//4th bit is 0
            man.mani = ((man.mani >> 20)&0x7);
    }
        //the mantissa is over 3 digits
    if ((man.mani & 0x8)==0x8) {
            man.mani = 0x0;
            rexp+=1;
    }
    if (rexp > 14 ) {
            if (rsign == 0) {
                y = 0x78;
                return y;
            }
            else {
                y = 0xf8;
                return y;
            }
    }
    //unsigned char rmantissa = ((unsigned char) man.mani)&0x7;
    y = ((rsign<<7)|(rexp<<3)|man.mani);
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
    unsigned int fsign, fexp,fmantissa;

    union{
        float f;
        unsigned int i;
    }u;

    if (sign == 0) {
        if (exp == 0x0) {
            if (mantissa == 0x0) {
                //printf("+zero\n");
                return 0.0f;
            }
            else {
                //printf("+denormalized\n");
                return (float)mantissa/512.0f;
                
            }
        }
        else if (exp == 0xf){
            if (mantissa == 0x0) {
                //+infinity
                //printf("+inifite\n");
                u.i = 0x7f800000;
                return u.f;
            }
            else{
                //+NaN
                //printf("+NaN\n");
                fexp = (0xff<<23);
                u.i = ( fexp | mantissa);
                return u.f;

            }
        }
        else {
            //printf("+normal\n");
            fexp = ((exp-7+127)<<23);
            fmantissa = (mantissa << 20);
            u.i = ( fexp | fmantissa);
            return u.f;
        }
    }
    else {
        if (exp == 0x0) {
            if (mantissa == 0x0) {
                //printf("-zero\n");
                return -0.0f;
            }
            else {
                //printf("-denormal\n");
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
                //printf("-infinite\n");
                u.i = 0xff800000;
                return u.f;
            }
            else{
                //-NaN
                //printf("-NaN\n");
                fsign = (1<<31);
                fexp = (0xff<<23);
                u.i = (fsign| fexp | mantissa);
                return u.f;
            }
        }
        else {
            //printf("-normal\n");
            fsign = (1<<31);
            fexp = ((exp-7+127)<<23);
            fmantissa = (mantissa << 20);
            u.i = (fsign | fexp | fmantissa);
            return u.f;
        }
    }

}


