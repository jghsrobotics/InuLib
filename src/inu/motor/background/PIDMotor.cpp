#include "inu/motor/background/PIDMotor.h"
#include "inu/motor/background/BackgroundMotorSystem.h"
#include <algorithm>

using namespace inu;

PIDMotor::PIDMotor(unsigned int port, const PIDProfile& profile) :
	BackgroundMotor(port), motor(port), pid(-127, 127, profile) {

	this->port = port;
	BackgroundMotorSystem::Instance()->EnrollMotor(this);
}

PIDMotor::~PIDMotor() {
	BackgroundMotorSystem::Instance()->RemoveMotor(this);
}

void PIDMotor::Set(int target) {
	pid.SetTarget(target);
}

void PIDMotor::SetPID(const PIDProfile& pidProfile) {
	pid.SetPID(pidProfile);
}

int PIDMotor::GetTarget() const {
	return pid.GetTarget();
}

bool PIDMotor::AtTarget(int error) const {
	return pid.WithinError(motor.GetPosition(), error);
}

void PIDMotor::SetMaximumVelocity(int velocity) {
	pid.SetRange(-velocity, velocity);
}

double PIDMotor::GetPosition() const {
	return motor.GetPosition();
}

void PIDMotor::_Update() {
	if(pid.TargetSet()) {
		double motorSpeed = pid.Update(motor.GetPosition());
		motor.Move(motorSpeed);
	}
	else {
		motor.Move(0);
	}
}

