/** @file AutoXChassis.h
 * @brief Class for moving a x-drive powered chassis.
 *
 * Chassis used to control a x-drive.
*/

#ifndef INU_AUTOXCHASSIS_H
#define INU_AUTOXCHASSIS_H

#include "main.h"
#include "inu/auto/chassis/AutoChassis.h"
#include "inu/wrapper/Motor.h"
#include <memory>

namespace inu {
	class AutoXChassisBuilder;
	class PIDInertialMotor;

	/**
	 * Controls a x-drive powered chassis with a topleft, topright, bottomleft, and
	 * bottomright motor.
	*/ 
	class AutoXChassis : public AutoChassis {
	protected:
		void Copy(const AutoXChassis& chassis);

	public:
		/**
		 * topleft, topright, bottomleft, and bottomright are the motor ports of
		 * their respective motors. It is assumed that the motors are mounted such
		 * that, when powered with a positive speed, the entire chassis turns
		 * clockwise.
		*/ 
		AutoXChassis(const inu::AutoXChassisBuilder& builder);

		AutoXChassis(const AutoXChassis& chassis);

		void operator=(const AutoXChassis& chassis);

		~AutoXChassis();

		/**
		 * Rebuilds this entire chassis using an AutoXChassisBuilder instance.
		 * This is the best way to change the options of the chassis midway
		 * through the program.
		 *
		 * @param builder Any instance of a valid AutoXChassisBuilder class.
		*/ 
		virtual void Rebuild(const AutoXChassisBuilder& builder);

		virtual void Swerve(std::int8_t y, std::int8_t x) override;

		virtual void TurnA(double degrees) override;

		virtual void TurnAbsolute(double degrees) override;

		virtual void Turn(double ticks) override;

		virtual void Forward(double ticks) override;

		virtual void Backward(double ticks) override;

		virtual void StallUntilSettled(double timeout) override;

		virtual bool IsSettled() override;

		virtual void Stop() override;

		virtual void TareDistance() override;

		virtual double GetDistance() override;

		virtual double GetAbsoluteRotation() override;

		/**
		 * Shortcut for StrafeRight(-ticks).
		 *
		 * @param ticks The number of encoder ticks to move each wheel. If encoder
		 * units where changed, use the new unit instead.
		*/ 
		virtual void StrafeLeft(double ticks);

		/**
		 * Travel straight to the right using the integrated encoders. 
		 *
		 * This function has the capability of stalling; If stalling is enabled
		 * then the chassis will timeout and Stop() if the chassis is not able
		 * to face the direction.
		 *
		 * @param ticks The number of ticks the motors must meet within margin
		 * of error.
		*/
		virtual void StrafeRight(double ticks);

		/** 
		 * Given three velocities forward, right, and turn, swerve the entire
		 * chassis. Velocities will be clamped at the maximum velocity of the
		 * motor.
		 *
		 * This function will not stall and will simply set the speed of the
		 * motors.
		 *
		 * @param forward Positive values move the car forward, negative backward.
		 * @param right Positive values move car to the right, negative to the
		 * left.
		 * @param turn Positive values results in clockwise motion, negative
		 * for anticlockwise.
		*/
		virtual void Swerve(std::int8_t forward, std::int8_t right, std::int8_t turn);

		/** 
		 * Exactly like Swerve(), but uses raw voltage from [-127, 127] and
		 * clamps it from [-127, 127]. Not accurate for Auton at all; Mainly
		 * better for driving.
		 *
		 * @param forward Positive values move the car forward, negative backward.
		 * @param right Positive values move car to the right, negative to the
		 * left.
		 * @param turn Positive values results in clockwise motion, negative
		 * for anticlockwise.
		*/
		virtual void RawSwerve(std::int8_t forward, std::int8_t right, std::int8_t turn);


	protected:
		/**
		 * Deallocates the space of any background motors currently running
		 * that are used by the chassis.
		 *
		 * Call this when you need to be extra sure that no background motors
		 * exist.
		*/ 
		void FreeBackgroundMotors();


		/**
		 * Create background motors, assuming they don't exist. If they do exist,
		 * return false.
		 *
		 * @retuns true if the background motors were able to be created, false
		 * if a background motors is already occupying the space.
		*/
		bool CreateBackgroundMotors();

	protected:
		std::shared_ptr<inu::Motor> topleft;
		std::shared_ptr<inu::Motor> topright;
		std::shared_ptr<inu::Motor> bottomleft;
		std::shared_ptr<inu::Motor> bottomright;
std::unique_ptr<inu::PIDInertialMotor> inertialTopleft;
		std::unique_ptr<inu::PIDInertialMotor> inertialTopright;
		std::unique_ptr<inu::PIDInertialMotor> inertialBottomleft;
		std::unique_ptr<inu::PIDInertialMotor> inertialBottomright;
	};
}


#endif