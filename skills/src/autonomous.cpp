#include "okapi/api.hpp"
#include "autonselection.h"
#include "motordefs.hpp"

using namespace okapi;

const float TURN_SCALE_FACTOR = 4.85;

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void skillsAuton(MotorDefs *mtrDefs, bool redAlliance);

void autonomous() {
	MotorDefs mtrDefs;
	skillsAuton(&mtrDefs, true);
}

void driveRobot(MotorDefs *mtrDefs, int power, int travelTime){
	mtrDefs->left_mtr_f->move(power);
	mtrDefs->left_mtr_b->move(power);
	mtrDefs->right_mtr_f->move(power);
	mtrDefs->right_mtr_b->move(power);
	mtrDefs->right_mtr_m->move(power);
	mtrDefs->left_mtr_m->move(power);
	pros::Task::delay(travelTime);
	//hard break;
	mtrDefs->left_mtr_f->move(-15);
	mtrDefs->left_mtr_b->move(-15);
	mtrDefs->right_mtr_f->move(-15);
	mtrDefs->right_mtr_b->move(-15);
	mtrDefs->right_mtr_m->move(-15);
	mtrDefs->left_mtr_m->move(-15);
	//stop motors (set to 0)
	pros::Task::delay(50);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_b->move(0);
	mtrDefs->right_mtr_m->move(0);
	mtrDefs->left_mtr_m->move(0);
}

void driveWithCoast(MotorDefs *mtrDefs, int power, int duration){
	mtrDefs->left_mtr_f->move(power);
	mtrDefs->left_mtr_b->move(power);
	mtrDefs->right_mtr_f->move(power);
	mtrDefs->right_mtr_b->move(power);
	mtrDefs->right_mtr_m->move(power);
	mtrDefs->left_mtr_m->move(power);
	pros::Task::delay(duration);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_b->move(0);
	mtrDefs->right_mtr_m->move(0);
	mtrDefs->left_mtr_m->move(0);
}


void turnDegrees(MotorDefs *mtrDefs, int degrees, bool left){
	if(left){
		mtrDefs->left_mtr_f->move(-65);
		mtrDefs->left_mtr_b->move(-65);
		mtrDefs->left_mtr_m->move(-65);
		mtrDefs->right_mtr_f->move(65);
		mtrDefs->right_mtr_b->move(65);
		mtrDefs->right_mtr_m->move(65);
		pros::Task::delay(TURN_SCALE_FACTOR * degrees);
		//hard break (opposite power for 50ms)
		mtrDefs->left_mtr_f->move(15);
		mtrDefs->left_mtr_b->move(15);
		mtrDefs->left_mtr_m->move(15);
		mtrDefs->right_mtr_f->move(-15);
		mtrDefs->right_mtr_b->move(-15);
		mtrDefs->right_mtr_m->move(-15);
		pros::Task::delay(50);
		//setting to 0 (stop moving motors)
		mtrDefs->left_mtr_b->move(0);
		mtrDefs->left_mtr_f->move(0);
		mtrDefs->left_mtr_m->move(0);
		mtrDefs->right_mtr_f->move(0);
		mtrDefs->right_mtr_b->move(0);
		mtrDefs->right_mtr_m->move(0);
	} else {
		mtrDefs->left_mtr_f->move(65);
		mtrDefs->left_mtr_b->move(65);
		mtrDefs->left_mtr_m->move(65);
		mtrDefs->right_mtr_f->move(-65);
		mtrDefs->right_mtr_b->move(-65);
		mtrDefs->right_mtr_m->move(-65);
		pros::Task::delay(TURN_SCALE_FACTOR * degrees);
		//hard break (opposite power for 50ms)
		mtrDefs->left_mtr_f->move(-15);
		mtrDefs->left_mtr_b->move(-15);
		mtrDefs->left_mtr_m->move(-15);
		mtrDefs->right_mtr_f->move(15);
		mtrDefs->right_mtr_b->move(15);
		mtrDefs->right_mtr_m->move(15);
		pros::Task::delay(50);
		//setting to 0 (stop moving motors)
		mtrDefs->left_mtr_b->move(0);
		mtrDefs->left_mtr_f->move(0);
		mtrDefs->left_mtr_m->move(0);
		mtrDefs->right_mtr_f->move(0);
		mtrDefs->right_mtr_b->move(0);
		mtrDefs->right_mtr_m->move(0);
	}
	
}

void pickupAnotherBallAndComeBack(MotorDefs *mtrDefs){
	pros::Task::delay(200);
	driveWithCoast(mtrDefs, 50, 200);
	driveRobot(mtrDefs, 80, 1100);
	mtrDefs->intake_mtr->move(127);
	pros::Task::delay(300);
	// move back with ball and preload ball towards fence
	driveRobot(mtrDefs, -100, 850);
	driveRobot(mtrDefs, -50, 900);
}

void alignAndShoot(MotorDefs *mtrDefs, bool redAlliance){
	pros::Task::delay(200);	
	if(redAlliance == true){
		driveRobot(mtrDefs, 50, 200);
	} else {
		driveRobot(mtrDefs, 50, 110);
	}
	pros::Task::delay(200);
	//turn to face flags
	if(redAlliance){
		turnDegrees(mtrDefs, 86, true /* turn left */);
		pros::Task::delay(500);
		driveRobot(mtrDefs, 50, 200);
	} else {
		turnDegrees(mtrDefs, 77, false /* turn right */);
		pros::Task::delay(500);
		driveRobot(mtrDefs, 50, 350);
	}
	pros::Task::delay(300);
	mtrDefs->catapult_mtr->move_relative(1000, 127);
	pros::Task::delay(500);
	pros::ADIDigitalIn bumper('E');
	mtrDefs->catapult_mtr->move(127);
	while(bumper.get_value()){
		pros::Task::delay(50);
	}
	mtrDefs->catapult_mtr->move(0);
	mtrDefs->intake_mtr->move(127);
}

void flipBottomFlagAndBackToTile(MotorDefs *mtrDefs, bool redAlliance){
	if (redAlliance) {
		turnDegrees(mtrDefs, 17, true /* turn left */); //was15
	} else {
		turnDegrees(mtrDefs, 3, false /* turn right */);
	}
	driveRobot(mtrDefs, 110, 910);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -70, 900);
	
	if (redAlliance) {
		turnDegrees(mtrDefs, 83, false /* turn right */);
	} else {
		turnDegrees(mtrDefs, 90, true /* turn left */);
	}
	pros::Task::delay(200);
	driveRobot(mtrDefs, -50, 500);
}

void flipCap(MotorDefs *mtrDefs){
	mtrDefs->intake_mtr->move(0);
	mtrDefs->intake_mtr->move(-100);
	driveRobot(mtrDefs, 45, 1950);
	mtrDefs->intake_mtr->move(0);
	driveRobot(mtrDefs, 45, 100);
	driveRobot(mtrDefs, 50, 200);
}

void alignToMidAndShoot(MotorDefs *mtrDefs, bool redAlliance){
	if(redAlliance){
		turnDegrees(mtrDefs, 60, true);
	} else{
		turnDegrees(mtrDefs, 60, false);
	}
	driveRobot(mtrDefs, -50, 200);
	pros::Task::delay(100);
	mtrDefs->catapult_mtr->move_relative(500, 127);
}

void midLowerFlag(MotorDefs *mtrDefs, bool redAlliance){
	if(redAlliance){
		turnDegrees(mtrDefs, 20, true);
	} else{
		turnDegrees(mtrDefs, 16, false);
	}	
	driveRobot(mtrDefs, 127, 1300);
	driveRobot(mtrDefs, -75, 100);
	driveRobot(mtrDefs, 75, 400);
}

void loadBallFromBack(MotorDefs *mtrDefs, bool redAlliance) {
	// Move forward so that we clear the fence
	driveRobot(mtrDefs, 70, 25);
	pros::Task::delay(200);

	// turn to face the pick up ball under the cap
	if (redAlliance) {
		turnDegrees(mtrDefs, 52, false /* turn right */);
	} else {
		turnDegrees(mtrDefs, 50, true /* turn left */);
	}
	pros::Task::delay(200);
	driveRobot(mtrDefs, -70, 300);

	mtrDefs->intake_mtr->move(127);
	driveRobot(mtrDefs, 80, 1150);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -80, 100);
	pros::Task::delay(200);

	// Stop intake
	mtrDefs->intake_mtr->move(0);
}

void parkOnPlatform(MotorDefs *mtrDefs, bool redAlliance){
	pros::Task::delay(1000);
	driveRobot(mtrDefs, -50, 1500);
	turnDegrees(mtrDefs, 45, true);
	driveRobot(mtrDefs, -50, 935);
	turnDegrees(mtrDefs, 90, false);
	driveRobot(mtrDefs, -50, 1500);
	pros::Task::delay(200);
	driveRobot(mtrDefs, 127, 2150);

}


void skillsAuton(MotorDefs *mtrDefs, bool redAlliance){
	pickupAnotherBallAndComeBack(mtrDefs);
	alignAndShoot(mtrDefs, redAlliance);
	flipBottomFlagAndBackToTile(mtrDefs, redAlliance);
 	flipCap(mtrDefs);
	alignToMidAndShoot(mtrDefs, redAlliance);
	parkOnPlatform(mtrDefs, redAlliance);
	//midLowerFlag(mtrDefs, redAlliance)

}
