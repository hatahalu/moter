#include "mbed.h"

DigitalOut inA(A4); //dirのピン
PwmOut pwm(PA_10);   //pwmのピン
int main(){
    pwm.period_us(50);
    inA = 0; //dir 範囲 0 or 1
    pwm = 1;// pwm 範囲 0 から 1 , 0で止まる
}

// #include "mbed.h"
// using namespace std;

// DigitalOut inA(A0);
// DigitalOut inB(A1);
// PwmOut pwm(PA_3);
// int main(){
//     pwm.period_us(50);
//     inA = 0;
//     inB = 1;
//     pwm = 1;
// }


// #include "mbed.h"
// using namespace std;

// DigitalOut inA(A0);
// DigitalOut inB(A1);

// PwmOut pwm(PA_3);

// int main(){
//     pwm.period_us(50);

//     int i;
//     inA = 0;
//     inB = 1;
//     pwm = 1;

//     while(i<=1000000){
//         i++;
//         inA =!inA;
//         inB =inB;
//         ThisThread::sleep_for(1s);
//     }
//     return 0;

// }
