#include "PIDMotor.h"
#include "PIDSystem.h"

using namespace pros;

PIDMotor::PIDMotor(unsigned int port) {
	this->port = port;
	PIDSystem::EnrollMotor(this);

	proportion = 0;
	integral = 0;
	derivative = 0;
	pastError = 0;
}

PIDMotor::~PIDMotor() {
	PIDSystem::RemoveMotor(this);
}

void PIDMotor::Set(int target) {
	this->target = target;
}


void PIDMotor::SetPID(PIDProfile pidProfile) {
	this->pidProfile = pidProfile;
}

unsigned int PIDMotor::GetPort() {
	return port;
}

unsigned int PIDMotor::GetTarget() {
	return target;
}

const PIDProfile PIDMotor::GetPID() {
	return pidProfile;
}

void PIDMotor::_UpdatePID() {
	Motor motor(GetPort());
	double encoderValue = motor.get_position();

	proportion = target - encoderValue;
	integral += proportion;
	derivative = proportion - pastError;
	pastError = proportion;

	if(abs((int)proportion) < 20) {
		integral = 0;
	}

	if(proportion > 12000) {
		// Tune this
		integral = 0;
	}

	float p = pidProfile.p;
	float i = pidProfile.i;
	float d = pidProfile.d;

	float motorSpeed = (proportion * p) + (integral * i) + (derivative * d);
	motor.move(motorSpeed);

	pros::lcd::print(0, "Encoder Value: %f", encoderValue);
	pros::lcd::print(1, "Motor speed: %f", motorSpeed);
	pros::lcd::print(2, "Target: %f", (float)target);
	pros::lcd::print(3, "P: %f", p);
	pros::lcd::print(4, "I: %f", i);
	pros::lcd::print(5, "D: %f", d);
}









