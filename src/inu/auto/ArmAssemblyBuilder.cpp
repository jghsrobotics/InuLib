#include "ArmAssemblyBuilder.h"
#include "ArmAssembly.h"
#include "inu/motor/background/PIDMotor.h"
#include "pros/adi.hpp"
#include "inu/InuException.hpp"
#include "pros/motors.h"
#include <cstdint>
#include <memory>

using namespace inu;

ArmAssemblyBuilder::ArmAssemblyBuilder() {
	Reset();
}

void ArmAssemblyBuilder::Reset() {
	clawMotor.reset();
	armMotor.reset();
}

void ArmAssemblyBuilder::SetArmMotor(inu::port port, const PIDProfile& profile) {
	if(port < 0 || port > 20) 
		throw InuException("ArmAssemblyBuilder.h: Arm port must be between 1-20.");

	armMotor.reset(new inu::PIDMotor(port, profile));
	armMotor->SetBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
}

void ArmAssemblyBuilder::SetClawMotor(inu::port port, bool reversed) {
	clawMotor.reset(new inu::ADIMotor(port));
	clawMotor->SetReversed(reversed);
}

void ArmAssemblyBuilder::SetArmMaximumVelocity(std::uint32_t velocity) {
	armMotor->SetMaximumVelocity(static_cast<int>(velocity));
}

std::shared_ptr<inu::ADIMotor> ArmAssemblyBuilder::GetClawMotor() const {
	return clawMotor;
}

std::shared_ptr<inu::PIDMotor> ArmAssemblyBuilder::GetArmMotor() const {
	return armMotor;
}

std::shared_ptr<ArmAssembly> ArmAssemblyBuilder::Build() {
	if(clawMotor == nullptr || armMotor == nullptr)
		throw InuException("ArmAssemblyBuilder.h: Arm or claw motor wasn't set.");

	return std::shared_ptr<ArmAssembly>(new ArmAssembly(*this));
}
