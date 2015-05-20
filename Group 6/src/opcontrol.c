/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

TaskHandle driveTask;
TaskHandle manipulatorTask;

void DrivingTask(void *arg) {
	int C1LX, C1LY, C1RX;
	while (1) {
		C1LX = joystickGetAnalog(1, 1);
		C1LY = joystickGetAnalog(1, 3);
		C1RX = joystickGetAnalog(1, 4);

		motorSet(9, -C1LY - C1LX - C1RX);
		motorSet(2, C1LY - C1LX - C1RX);
		motorSet(1, C1LY + C1LX - C1RX);
		motorSet(10, -C1LY + C1LX - C1RX);

		delay(20);
	}
}

void ManipulatorTask(void *arg) {
	while (1) {
		if (joystickGetDigital(1, 6, JOY_UP)) {
			motorSet(5, 127);
			motorSet(6, 127);
		} else if (joystickGetDigital(1, 6, JOY_DOWN)) {
			motorSet(5, -127);
			motorSet(6, -127);
		} else {
			motorSet(5, 0);
			motorSet(6, 0);
		}
		if (joystickGetDigital(1, 5, JOY_UP)) {
			motorSet(7, 127);
		} else if (joystickGetDigital(1, 5, JOY_DOWN)) {
			motorSet(7, -127);
		} else {
			motorSet(7, 0);
		}
		if (joystickGetDigital(1, 8, JOY_DOWN) || joystickGetDigital(1, 7, JOY_DOWN)) {
			motorSet(8, 128);
		} else {
			motorSet(8, 0);
		}
		if (joystickGetDigital(1, 7, JOY_DOWN)) {
			motorSet(4, 128);
		} else if (joystickGetDigital(1, 8, JOY_DOWN)) {
			motorSet(4, -128);
		} else {
			motorSet(4, 0);
		}
		delay(20);
	}
}

void operatorControl() {
	driveTask = taskCreate(DrivingTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
	manipulatorTask = taskCreate(ManipulatorTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

	while (1) {
		delay(20);
	}
}
