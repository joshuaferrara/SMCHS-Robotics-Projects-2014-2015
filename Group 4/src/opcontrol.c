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

void
DriveSystemArcadeDrive( short forward, short turn )
{
    long drive_l_motor;
    long drive_r_motor;

    // Set drive
    drive_l_motor = forward + turn;
    drive_r_motor = forward - turn;

    // normalize drive so max is 127 if any drive is over 127
    int max = abs(drive_l_motor);
    if (abs(drive_r_motor)  > max)
        max = abs(drive_r_motor);
    if (max>127) {
        drive_l_motor = 127 * drive_l_motor / max;
        drive_r_motor = 127 * drive_r_motor  / max;
    }

	motorSet( 9, drive_l_motor);
	motorSet( 10, drive_l_motor);
	motorSet( 1, drive_r_motor);
	motorSet( 2, drive_r_motor);
}

void
ArcadeDriveTask(void *arg)
{
    short   forward, turn;

    (void)arg;

    while(1)
        {
        // Get controller
        if(abs( joystickGetAnalog( 1, 4 ) ) > 10 )
            forward = joystickGetAnalog( 1, 4 );
        else
            forward = 0;

        if( abs( joystickGetAnalog( 1, 3 ) ) > 10 )
            turn = -joystickGetAnalog( 1, 3 );
        else
            turn = 0;

        DriveSystemArcadeDrive( forward / 2, turn / 2 );

        taskDelay(25);
        }
}

void ManipulatorTask(void *arg) {
	while (1) {
		if (joystickGetDigital(1, 6, JOY_UP)) {
			motorSet(5, 127);
		} else if (joystickGetDigital(1, 6, JOY_DOWN)) {
			motorSet(5, -127);
		} else {
			motorSet(5, 0);
		}
	}
}

void operatorControl() {
	taskCreate( ArcadeDriveTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
	taskCreate( ManipulatorTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
}
