#ifndef _ROBOT_DRIVER_H_
#define _ROBOT_DRIVER_H_

class MotorDefs;

namespace pros{
    class ADIGyro;
}

class RobotDriver {
    private:
        MotorDefs *mtrDefs;
        pros::ADIGyro *gyro;
    public:
        RobotDriver(MotorDefs *motorDefs, pros::ADIGyro *gy);
        ~RobotDriver();
        void driveRobot(double degrees, std::int32_t velocity);
        void driveWithCoast(int time, int power);
        void turnRobot(int unscaledDegs, bool left);
        void smoothDrive(int degrees, int power, int direction);
    private:
        void waitForDriveCompletion();
        void waitForTurnCompletion();
        void verifyTurns(MotorDefs *mtrDefs, int degrees, int direction);
};


#endif //_ROBOT_DRIVER_H_