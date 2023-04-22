/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       bruce_2nbsnuo                                             */
/*    Created:      2023/4/22 下午1:27:31                                       */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
vision::signature Vision__BLUE_BALL = vision::signature (1, -2777, -2145, -2461, 11787, 15261, 13524, 3, 0);
vision Vision = vision (PORT19, 100, Vision__BLUE_BALL);
motor Motor = motor (PORT20, ratio18_1, false);
vex::bumper Bumper = vex::bumper(Brain.ThreeWirePort.H);


// define your global instances of motors and other devices here

bool resetMotor() {
    while (!Bumper.pressing()) {
        Motor.spin(forward);
    }
    Motor.stop();
    Motor.spinFor(reverse, 360, degrees);
    Motor.stop();
    Motor.resetPosition();
    Motor.setPosition(565, degrees);
    return true;
}

int main() {
    float ball_x, motor_x;
    float error, kp = 1, power;
    resetMotor();
    while(true) {
        Vision.takeSnapshot(Vision__BLUE_BALL);
        if (Vision.objectCount > 0) {
            ball_x = Vision.largestObject.centerX;
            Brain.Screen.printAt(10, 10, "x: %f", ball_x);
            Brain.Screen.printAt(10, 30, "motor: %f", motor_x);
            wait(10, msec);
            Brain.Screen.clearScreen();
            motor_x = Motor.position(degrees);
            if (motor_x < 150) {
                motor_x = 150;
            } else if (motor_x > 900) {
                motor_x = 900;
            }

            if (ball_x < 45) {
                ball_x = 45;
            } else if (ball_x > 270) {
                ball_x = 270;
            }
            error = ball_x - motor_x * 0.3;
            power = error * kp;
            Motor.setVelocity(power, percent);
            Motor.spin(forward);
        } else {
            Motor.stop();
        }

    }
}
