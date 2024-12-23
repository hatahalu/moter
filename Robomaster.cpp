#define MOTOR_ID  2
#define ADJUST_ID  ((MOTOR_ID) >= 5 ? (MOTOR_ID) - 4 : (MOTOR_ID))
#define HIGH_VALUE (((ADJUST_ID) - 1) * 2)
#define LOW_VALUE  (HIGH_VALUE + 1)

#include "mbed.h"
#include "CAN.h"

// モーターIDが1~4まで対応（5~8は

CAN can(D10, D2); // CAN RX, CAN TX
DigitalOut led(LED1);

// 送るモーターID
const int MOTOR_CMD_ID = 0x200 + MOTOR_ID - 1;
// 受け取るモーターID
const int MOTOR_FEEDBACK_ID = 0x200 + MOTOR_ID;

void send_motor_command(int16_t current) {
    char data[8] = {0};
    data[HIGH_VALUE] = current >> 8;  // 上位バイト
    data[LOW_VALUE] = current & 0xFF; // 下位バイト
    CANMessage msg(MOTOR_CMD_ID, data, 8);
    if (can.write(msg)) {
        printf("Motor command sent: %d  ", current);
    } else {
        printf("Error sending motor command.\n");
    }
}

void parse_feedback(const CANMessage &msg) {
    if (msg.id == MOTOR_FEEDBACK_ID) {
        int16_t rotor_angle = (msg.data[0] << 8) | msg.data[1];
        int16_t rotational_speed = (msg.data[2] << 8) | msg.data[3];
        int16_t torque_current = (msg.data[4] << 8) | msg.data[5];
        int8_t temperature = msg.data[6];
        printf("Feedback ID: %d 角度: %d, 回転速度: %d, トルク電流: %d, 温度: %d\n",
               MOTOR_ID, rotor_angle, rotational_speed, torque_current, temperature);
    }
}

int main() {
    printf("スタート\n");

    can.frequency(1000000); // CANの速度は1 Mbps

    // Main loop
    while (true) {
        CANMessage msg;
        send_motor_command(1000); // 送る電流値
        if (can.read(msg)) {
            parse_feedback(msg);
        }
        led = !led; // 動作確認
    }
}





#include "mbed.h"
#include "CAN.h"

// モーターIDが1~4まで対応（5~8は

CAN can(D10, D2); // CAN RX, CAN TX
DigitalOut led(LED1);


void send_motor_command(int16_t current,int motor_sed_id) {
    int adjust_id  = ((motor_sed_id) >= 5 ? (motor_sed_id) - 4 : (motor_sed_id));
    int high_value = (((adjust_id) - 1) * 2);
    int low_value  = (high_value + 1);
    // 送るモーターID
    int motor_cmd_id = 0x200 - motor_sed_id;

    char data[8] = {0};
    // data[high_value] = current >> 8;  // 上位バイト
    // data[low_value] = current & 0xFF; // 下位バイト
    data[high_value] = (current&0X00FF) >> 8;  // 上位バイト
    data[low_value] = current&0X00FF; // 下位バイト
    CANMessage msg(motor_cmd_id, data, 8);
    if (can.write(msg)) {
        printf("Motor command sent: %d  ", current);
    } else {
        printf("Error sending motor command.\n");
    }
}

void parse_feedback(const CANMessage &msg,int motor_rev_id) {
    // 受け取るモーターID
    int motor_feedmack_id = 0x200 + motor_rev_id;

    if (msg.id == motor_feedmack_id) {
        int16_t rotor_angle = (msg.data[0] << 8) | msg.data[1];
        int16_t rotational_speed = (msg.data[2] << 8) | msg.data[3];
        int16_t torque_current = (msg.data[4] << 8) | msg.data[5];
        int8_t temperature = msg.data[6];
        printf("Feedback ID: %d 角度: %d, 回転速度: %d, トルク電流: %d, 温度: %d\n",
               motor_rev_id, rotor_angle, rotational_speed, torque_current, temperature);
    }
}

int main() {
    printf("スタート\n");

    can.frequency(1000000); // CANの速度は1 Mbps

    // Main loop
    while (true) {
        CANMessage msg;
        send_motor_command(1000,6); // 送る電流値
        if (can.read(msg)) {
            parse_feedback(msg,6);
        }
        led = !led; // 動作確認
    }
}
