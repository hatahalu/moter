#define ROBOMAS_BAUDRATE        1*1000*1000
#define ROBOMAS_OUTPUT_MAX      1*100*100
#define ROBOMAS_OUTPUT_MIN      -1*100*100
#define ROBOMAS_ADDRESS_1       0X200
#define ROBOMAS_ADDRESS_2       0X1FF
#define ROBOMAS_OUTPUT_MAX      1*100*100
#define ROBOMAS_OUTPUT_MIN      -1*100*100
#include "mbed.h"
#include "CAN.h"

// モーターIDが1~4まで対応（5~8は

CAN can(D10, D2); // CAN RX, CAN TX
DigitalOut led(LED1);

void set_motor_speed(int motor_no, int16_t speed)
{
    char motor_output[16];
    // モーター番号が有効範囲かチェック
    if (1 <= motor_no && motor_no <= 8)
    {
        // 速度が有効範囲かチェック
        if (ROBOMAS_OUTPUT_MIN <= speed && speed <= ROBOMAS_OUTPUT_MAX)
        {
            // モーター出力データを設定
            motor_output[motor_no * 2 - 2] = (speed & 0XFF00) >> 8;
            motor_output[motor_no * 2 - 1] = (speed & 0X00FF);
        }
    }
    CANMessage msg1(ROBOMAS_ADDRESS_1, motor_output, 8);
    CANMessage msg2(ROBOMAS_ADDRESS_2, &motor_output[8], 8);
    
    can.write(msg1);
    can.write(msg2);
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
        set_motor_speed(6, 1000);
        ThisThread::sleep_for(1ms);
        if (can.read(msg)) {
            parse_feedback(msg,6);
        }
        led = !led; // 動作確認
    }
}


#include "mbed.h"
#include "CAN.h"

// 定義
#define ROBOMAS_BAUDRATE        1 * 1000 * 1000
#define ROBOMAS_OUTPUT_MAX      1 * 100 * 100
#define ROBOMAS_OUTPUT_MIN      -1 * 100 * 100
#define ROBOMAS_ADDRESS_1       0x200
#define ROBOMAS_ADDRESS_2       0x1FF

// CAN設定
CAN can(D10, D2); // CAN RX, CAN TX
DigitalOut led(LED1);

// バッファ
char send_buffer[16] = {0};  // 送信用バッファ
CANMessage receive_buffer[10]; // 受信用バッファ (最大10件)

// モーター速度設定
void set_motor_speed(int motor_no, int16_t speed) {
    // モーター番号と速度の有効性チェック
    if (motor_no < 1 || motor_no > 8 || speed < ROBOMAS_OUTPUT_MIN || speed > ROBOMAS_OUTPUT_MAX) {
        printf("モーター番号または速度が無効です\n");
        return;
    }

    // バッファにデータを設定
    send_buffer[motor_no * 2 - 2] = (speed & 0xFF00) >> 8; // 上位バイト
    send_buffer[motor_no * 2 - 1] = (speed & 0x00FF);      // 下位バイト

    // メッセージを送信
    CANMessage msg1(ROBOMAS_ADDRESS_1, send_buffer, 8);
    CANMessage msg2(ROBOMAS_ADDRESS_2, &send_buffer[8], 8);
    can.write(msg1);
    can.write(msg2);
}

// フィードバック解析
void parse_feedback(const CANMessage &msg, int motor_rev_id) {
    int motor_feedback_id = 0x200 + motor_rev_id;

    if (msg.id == motor_feedback_id) {
        int16_t rotor_angle = (msg.data[0] << 8) | msg.data[1];
        int16_t rotational_speed = (msg.data[2] << 8) | msg.data[3];
        int16_t torque_current = (msg.data[4] << 8) | msg.data[5];
        int8_t temperature = msg.data[6];

        printf("Feedback ID: %d 角度: %d, 回転速度: %d, トルク電流: %d, 温度: %d\n",
               motor_rev_id, rotor_angle, rotational_speed, torque_current, temperature);
    }
}

// メイン関数
int main() {
    printf("スタート\n");

    can.frequency(ROBOMAS_BAUDRATE); // CAN速度を1 Mbpsに設定

    while (true) {
        // モーター6の速度設定
        set_motor_speed(6, 1000);

        // 受信処理
        CANMessage msg;
        if (can.read(msg)) {
            parse_feedback(msg, 6);
        }

        // LEDのトグル（動作確認用）
        led = !led;
    }
}


