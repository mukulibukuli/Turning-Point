#include "main.h"
#include "motordefs.hpp"

#include "okapi/api.hpp"
using namespace okapi;

pros::Controller master(pros::E_CONTROLLER_MASTER);
static MotorDefs *mtr_defs = MotorDefs::getMotorDefs();

void driveTask(void* param){
	while(true){
		int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		float scaledTurn = ((turn * 100) * 0.75) / 100;
		float leftMtrVals = (forward + scaledTurn);
		float rightMtrVals = -(scaledTurn - forward);
		if(leftMtrVals > 127){
			leftMtrVals = 127;
		}
		if(leftMtrVals < -127){
			leftMtrVals = -127;
		}
		if(rightMtrVals > 127){
			rightMtrVals = 127;
		}
		if(rightMtrVals < -127){
			rightMtrVals = -127;
		}
		mtr_defs->left_mtr_b->move(leftMtrVals);
		mtr_defs->left_mtr_f->move(leftMtrVals);
		mtr_defs->right_mtr_b->move(rightMtrVals);
		mtr_defs->right_mtr_f->move(rightMtrVals);
		// The below delay is required for tasks to work in PROS.
		pros::Task::delay(10);
	}
}

void catapultShootTask(void* param){
	while(true){
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
                mtr_defs->catapult_mtr->move(127);
            }
            mtr_defs->catapult_mtr->move(0);
        }
        pros::Task::delay(10);
    }
}

void catapultPrepareToLoadTask(void* param){
	pros::ADIDigitalIn bumper('E');
  	while (true) {
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
			mtr_defs->catapult_mtr->move(127);
			while(bumper.get_value()){
				pros::Task::delay(50);
			}
			mtr_defs->catapult_mtr->move(0);
	  	}
		pros::Task::delay(10);
  	}
}

void intakeTask(void* param){
	static bool intakeStarted = false;
    while(true){
        if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
			if(!intakeStarted){
				mtr_defs->intake_mtr->move(127);	
				intakeStarted = true;
			} else if(intakeStarted){
				mtr_defs->intake_mtr->move(0);
				intakeStarted = false;
			}
			pros::Task::delay(300);
		}
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){	
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
				mtr_defs->intake_mtr->move(-127);
			}
			mtr_defs->intake_mtr->move(0);
		}
        pros::Task::delay(10);
    }
}       

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
//task starter function (called by competition)
void opcontrol() {
	pros::Task driveTask(driveTask);
	pros::Task catapultPrepareToLoadTask(catapultPrepareToLoadTask);
	pros::Task catapultShootTask(catapultShootTask);
	pros::Task intakeTask(intakeTask);
}