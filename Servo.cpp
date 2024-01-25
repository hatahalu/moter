#include "mbed.h"

PwmOut pwm(PA_3);

int main() {
    int start=500, end=2500;
    while(true){
        for(int p = start; p < end; p += 40) {
            pwm.pulsewidth_us(p);
            wait_us(1000*40);
        }
        for(int p = end; p >=start; p -= 40) {
             pwm.pulsewidth_us(p);
            wait_us(1000*40);
        }
    }
}
