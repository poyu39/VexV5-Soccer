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

#define MAPPING_MB 0.3
#define MAX_MOTOR_X 900
#define MIN_MOTOR_X 150
#define MAX_BALL_X 270
#define MIN_BALL_X 45
#define MAX_BALL_Y 210
#define MIN_BALL_Y 10
#define MAX_SCREEN_X 480
#define MAX_SCREEN_Y 272

// define your global instances of motors and other devices here

bool resetMotor() {
    Motor.setVelocity(50, percent);
    while (!Bumper.pressing()) {
        Motor.spin(forward);
    }
    Motor.stop();
    Motor.spinFor(reverse, 360, degrees);
    Motor.stop();
    // Motor.resetPosition();
    Motor.setPosition(765, degrees);
    return true;
}

float mapping(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void display_field(float ball_x, float ball_y, float motor_x) {
    Brain.Screen.clearScreen();
    ball_x = mapping(ball_x, MIN_BALL_X, MAX_BALL_X, 0, MAX_SCREEN_X);
    ball_y = mapping(ball_y, MIN_BALL_Y, MAX_BALL_Y, 0, MAX_SCREEN_Y);
    motor_x = mapping(motor_x, MIN_MOTOR_X, MAX_MOTOR_X, 0, MAX_SCREEN_X);
    Brain.Screen.drawCircle(MAX_SCREEN_X - ball_x, MAX_SCREEN_Y - ball_y, 15);
    Brain.Screen.drawRectangle(MAX_SCREEN_X - motor_x, 230, 30, 10);
    Brain.Screen.printAt(300, 10, "ball_x: %f", ball_x);
    Brain.Screen.printAt(300, 30, "ball_y: %f", ball_y);
    Brain.Screen.render();
    wait(10, msec);
}

int main() {
    float ball_x, ball_y, motor_x;
    float error, kp = 1, power;
    float map_value = 0.3;
    resetMotor();

    // while (true) {   
    //     Vision.takeSnapshot(Vision__BLUE_BALL);
    //     ball_x = Vision.largestObject.centerX;
    //     ball_y = Vision.largestObject.centerY;
    //     motor_x = Motor.position(degrees);
    //     display_field(ball_x, ball_y, motor_x);
    // }
    

    // follow ball
    while (true) {
        Vision.takeSnapshot(Vision__BLUE_BALL);
        if (Vision.objectCount > 0) {
            ball_x = Vision.largestObject.centerX;
            ball_y = Vision.largestObject.centerY;
            motor_x = Motor.position(degrees);
            display_field(ball_x, ball_y, motor_x);
            if (motor_x < MIN_MOTOR_X) {
                motor_x = MIN_MOTOR_X;
            } else if (motor_x > MAX_MOTOR_X) {
                motor_x = MAX_MOTOR_X;
            }
            if (ball_x < MIN_BALL_X) {
                ball_x = MIN_BALL_X;
            } else if (ball_x > MAX_BALL_X) {
                ball_x = MAX_BALL_X;
            }
            error = ball_x - motor_x * MAPPING_MB;
            power = error * kp;
            if (power > 100) {
                power = 100;
            } else if (power < -100) {
                power = -100;
            }
            Motor.setVelocity(power, percent);
            Motor.spin(forward);
        } else {
            Motor.stop();
        }
    }
}
