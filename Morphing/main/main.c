/*
MIT License

Copyright (c) 2024 Society of Robotics and Automation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "sra_board.h"
#include "tuning_websocket_server.h"

#define TAG "MORPHING"
#define DRONE_MODE 1
#define QUAD_MODE 0
#define SET_SERVO_90 83
#define MIN_PWM (40.0f) 
#define MAX_PWM (80.0f)

motor_handle_t motor1;
motor_handle_t motor2;
bool MORPH_UNLOCK = 1, MORPH_ACTIVATE, MORPHED, DEMORPHED, FLIGHT_READY;
servo_config servo_a = {
	.servo_pin = SERVO_A,
	.min_pulse_width = CONFIG_SERVO_A_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_A_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_A_MAX_DEGREE,
};

servo_config servo_b = {
	.servo_pin = SERVO_B,
	.min_pulse_width = CONFIG_SERVO_B_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_B_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_B_MAX_DEGREE,
};

servo_config servo_c = {
	.servo_pin = SERVO_C,
	.min_pulse_width = CONFIG_SERVO_C_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_C_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_C_MAX_DEGREE,
};

servo_config servo_d = {
	.servo_pin = SERVO_D,
	.min_pulse_width = CONFIG_SERVO_D_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_D_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_D_MAX_DEGREE,
};

static void mcpwm_servo_control(void *arg)
{
	ESP_ERROR_CHECK(enable_motor_driver(&motor1, MOTOR_A_0));
	ESP_ERROR_CHECK(enable_motor_driver(&motor2, MOTOR_A_1));
	ESP_ERROR_CHECK(enable_servo());
	
	while (1)
	{
		if (MORPH_UNLOCK == QUAD_MODE){
			if (DEMORPHED == false){
				ESP_LOGI(TAG, "Demorphing...\n");
				for (int i = 0; i <= SET_SERVO_90; i+=2){
					ESP_ERROR_CHECK(set_angle_servo(&servo_a, i));
					ESP_ERROR_CHECK(set_angle_servo(&servo_b, SET_SERVO_90 - i));
					ESP_ERROR_CHECK(set_angle_servo(&servo_c, i));
					ESP_ERROR_CHECK(set_angle_servo(&servo_d, SET_SERVO_90 - i));
					vTaskDelay(10);
				}
				DEMORPHED = true;
				ESP_LOGI(TAG, "QUAD MODE ACTIVATED\n");
				ESP_LOGI(TAG, "Activating wheels...\n");
			}
			else {
				if(read_comms().val_changed){
            		float speed = read_comms().speed;
            		ESP_LOGI(TAG, "speed: %f", speed);                                          //print to console
            		float motor_pwm = bound((speed), MIN_PWM, MAX_PWM);
            		if(read_comms().front){
						ESP_LOGI(TAG, "forward");
						ESP_ERROR_CHECK(set_motor_speed(motor1, MOTOR_FORWARD, motor_pwm));
						ESP_ERROR_CHECK(set_motor_speed(motor2, MOTOR_FORWARD, motor_pwm));
					} else if(read_comms().back){
						ESP_LOGI(TAG, "backward");
						ESP_ERROR_CHECK(set_motor_speed(motor1, MOTOR_BACKWARD, motor_pwm));
						ESP_ERROR_CHECK(set_motor_speed(motor2, MOTOR_BACKWARD, motor_pwm));
					} else if(read_comms().right_turn) {
						ESP_LOGI(TAG, "right turn");
						ESP_ERROR_CHECK(set_motor_speed(motor1, MOTOR_FORWARD, motor_pwm));
						ESP_ERROR_CHECK(set_motor_speed(motor2, MOTOR_BACKWARD, motor_pwm));
					}
					else if(read_comms().left_turn) {
						ESP_LOGI(TAG, "left turn");
						ESP_ERROR_CHECK(set_motor_speed(motor1, MOTOR_BACKWARD, motor_pwm));
						ESP_ERROR_CHECK(set_motor_speed(motor2, MOTOR_FORWARD, motor_pwm));
					} else {
						ESP_LOGI(TAG, "stop");
						ESP_ERROR_CHECK(set_motor_speed(motor1, MOTOR_STOP, 0));
						ESP_ERROR_CHECK(set_motor_speed(motor2, MOTOR_STOP, 0));
					}
					reset_val_changed_coms();
				}
				vTaskDelay(10 / portTICK_PERIOD_MS);
				
				if (MORPH_ACTIVATE == true){
					MORPH_UNLOCK = DRONE_MODE;
					MORPHED = false;
					vTaskDelay(50);
				}
			}
		}
		else {                                          //DRONE_MODE
			if (MORPHED == false){
				ESP_LOGI(TAG, "Morphing...\n");
				for (int i = 0; i<= SET_SERVO_90; i+=2){
					set_angle_servo(&servo_a, SET_SERVO_90 - i);
					set_angle_servo(&servo_b, i);
					set_angle_servo(&servo_c, SET_SERVO_90 - i);
					set_angle_servo(&servo_d, i);
					vTaskDelay(10);
				}
				MORPHED = true;
				ESP_LOGI(TAG, "DRONE MODE ACTIVATED\n");
			}
			else {
				ESP_LOGI(TAG, "Activating Propellers...\n");
				FLIGHT_READY = true;
				if (MORPH_ACTIVATE == true){
					MORPH_UNLOCK = QUAD_MODE;
					DEMORPHED = false;
					vTaskDelay(50);
				}
			}

		}
		// set_motor_speed(motor1, MOTOR_FORWARD, 100);
		// set_motor_speed(motor2, MOTOR_FORWARD, 100);
		// set_angle_servo(&servo_a, 0);
		// set_angle_servo(&servo_b, 83);
		// set_angle_servo(&servo_c, 83);
		// set_angle_servo(&servo_d, 0);
		// vTaskDelay(100);
#ifdef CONFIG_ENABLE_OLED
		// Diplaying Servo A, Servo B, Servo C, Servo D values on OLED
		display_servo_values(read_servo(&servo_a), read_servo(&servo_b), read_servo(&servo_c), read_servo(&servo_d));
#endif
		vTaskDelay(100);
	}
}

void app_main()
{
	start_websocket_server();
	ESP_LOGD(TAG, "Testing servo motors\n");
	xTaskCreate(mcpwm_servo_control, "mcpwm_example_servo_control", 4096, NULL, 5, NULL);
}
