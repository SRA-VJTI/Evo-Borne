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

#define TAG "MORPHING"
#define DRONE_MODE 1
#define QUAD_MODE 0

motor_handle_t motor1;
motor_handle_t motor2;
bool MORPH_UNLOCK;
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
	enable_motor_driver(&motor1, MOTOR_A_0);
	enable_motor_driver(&motor2, MOTOR_A_1);
	enable_servo();
#ifdef CONFIG_ENABLE_OLED
	// Initialising the OLED
	// ESP_ERROR_CHECK(init_oled());
	// display_logo(MARIO_LOGO);
	vTaskDelay(100);
#endif

	while (1)
	{
		// if (MORPH_UNLOCK == QUAD_MODE){
		// 	for (int i = 83; i >= 0; i--){	
		// 		ESP_LOGI(TAG, "Demorphing...\n");
		// 		set_angle_servo(&servo_a,83-i);
		// 		set_angle_servo(&servo_b,i);
		// 		set_angle_servo(&servo_c,83-i);
		// 		set_angle_servo(&servo_d,i);
		// 		vTaskDelay(10);
		// 		if (i == 0){
		// 			morph_unlock = true;
		// 			ESP_LOGI(TAG, "Quad Mode\n");
		// 		}
		// 	}
		// 	vTaskDelay(100);
		// 	set_motor_speed(motor1, MOTOR_FORWARD, 100);
		// 	set_motor_speed(motor2, MOTOR_FORWARD, 100);
		// }
		// else {
			
		// }
		// set_motor_speed(motor1, MOTOR_FORWARD, 100);
		// set_motor_speed(motor2, MOTOR_FORWARD, 100);
		set_angle_servo(&servo_a, 0);
		set_angle_servo(&servo_b, 83);
		set_angle_servo(&servo_c, 83);
		set_angle_servo(&servo_d, 0);
		vTaskDelay(100);
#ifdef CONFIG_ENABLE_OLED
		// Diplaying Servo A, Servo B, Servo C, Servo D values on OLED
		display_servo_values(read_servo(&servo_a), read_servo(&servo_b), read_servo(&servo_c), read_servo(&servo_d));
#endif
	}
}

void app_main()
{
	ESP_LOGD(TAG, "Testing servo motors\n");
	xTaskCreate(mcpwm_servo_control, "mcpwm_example_servo_control", 4096, NULL, 5, NULL);
}
