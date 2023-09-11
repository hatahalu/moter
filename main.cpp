#include "mbed.h"
using namespace std;

DigitalOut inA(A0);
DigitalOut inB(A1);

PwmOut pwm(PA_3);

int main(){
    int i;
    inA = 0;
    inB = 1;
    pwm = 1;

    while(i<=1000000){
        i++;
        inA =!inA;
        inB =!inB;

        ThisThread::sleep_for(1s);
    }
    return 0;

}
