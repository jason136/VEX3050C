#include "main.h"
#include "portdef.h"
#include "lift.h"

pros::Motor rightLiftMotor(RIGHT_LIFT_MOTOR_PORT);
pros::Motor leftLiftMotor(LEFT_LIFT_MOTOR_PORT);

void liftSetZero() {
  // resets encoders of the lift Motor
  rightLiftMotor.tare_position();
  if(DEBUG_ON){
    std::cout << "Lift ZERO point reset \n";
  }
}

void liftRaiseManual(int speed) {
  // Raises lift manual based using speed provided
  // Don't allow movement past lowest point and past upper point of mechanical limits of lift
  // We can't use encoder for that as it will bypass it - we need likely mechanical switch for safety
  // So for now there is NO SAFETY stop
  if(DEBUG_ON){
    std::cout << "In Lift Raise Manual function -- Speed: " << speed ;
    std::cout << " Current Encoder: " << rightLiftMotor.get_position() << "\n";
  }
  rightLiftMotor.move_velocity(speed);
  leftLiftMotor.move_velocity(speed);
}

void liftRaiseForEncoder(int speed, int encDegrees) {
  // raise the lift to a given position as specified by encDegrees at speed indicated
  // by variable speed.  Lift will protect agaisnt lower and upper bound of lift
  // mechanical cability
  int moveDegrees = rightLiftMotor.get_position() + encDegrees;   // take current position and add
                                                              // requested movement to it

  // allow movement we are within bounderies
  int upperBound = moveDegrees + 5;
  int lowerBound = moveDegrees - 5;
  if(lowerBound < 0 ) {
    lowerBound = 0;
    moveDegrees = 0;  // Can't move past below 0 point!
  }

  if (!((moveDegrees > lowerBound) && (moveDegrees < upperBound))) {
    // we can move
    rightLiftMotor.move_absolute(moveDegrees, speed); // Moves given position
    leftLiftMotor.move_absolute(moveDegrees, speed); // Moves given position

    while (!((rightLiftMotor.get_position() < upperBound) && (rightLiftMotor.get_position() > lowerBound))) {
        // Continue running this loop as long as the motor is not within +-5 units of its goal
        pros::delay(2);
    }
  }
  if(DEBUG_ON) {
    std::cout << "EncDegrees " << encDegrees << " moveDegrees: " << moveDegrees;
    std::cout << " Current Deg: " << rightLiftMotor.get_position() << "\n";
    pros::delay(2);
  }
}

void liftLockMode() {
  // get the current lift lock mode of the break
  std::cout << "Brake Mode: " << rightLiftMotor.get_brake_mode() << "\n";
  pros::delay(10);
}

void liftLock() {
  // lock the lift where it is right now....
  // We need to make sure that the lift is on BREAK_HOLD
  if(rightLiftMotor.get_brake_mode() != 2 ) {
    rightLiftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    leftLiftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  }
  rightLiftMotor.move_velocity(0);
  leftLiftMotor.move_velocity(0);
  pros::delay(2);
}

void liftEncoderValue() {
  // gets the encoder value and prints it to the std::std::cout
  std::cout << "Lift Encoder: -- M1: " << rightLiftMotor.get_position();
  pros::delay(2);
}

void liftRaise(int speed, int level) {
  // move the lift up and down where levle is:
  // 0 -- fully retracted back to 0
  // 1 -- raise to lower pole height
  // 2 -- raise to upper pole height

  int lowTowerUpper = LIFT_LEVEL_LOW + 5;
  int lowTowerLower = LIFT_LEVEL_LOW - 5;

  int highTowerUpper = LIFT_LEVEL_HIGH + 5;
  int highTowerLower = LIFT_LEVEL_HIGH - 5;

  int zeroPointUpper = LIFT_FULL_RETRACT + 5;
  int zeroPointLower = LIFT_FULL_RETRACT;

  switch(level) {
    case 0:
      // move all the way back down
      rightLiftMotor.move_absolute(0, speed); // Moves LIFT_LOW_POLE units forward
      leftLiftMotor.move_absolute(0, speed); // Moves LIFT_LOW_POLE units forward
      while (!((-rightLiftMotor.get_position() < zeroPointUpper) && (-rightLiftMotor.get_position() > zeroPointLower))) {
          // Continue running this loop as long as the motor is not within +-5 units of its goal
          pros::delay(2);
          void liftSetZero();
      }

      break;

    case 1:
      // move to middle pole position
      rightLiftMotor.move_absolute(LIFT_LEVEL_LOW, speed); // Moves LIFT_LOW_POLE units forward
      leftLiftMotor.move_absolute(LIFT_LEVEL_LOW, speed); // Moves LIFT_LOW_POLE units forward
      while (!((-rightLiftMotor.get_position() < lowTowerUpper) && (-rightLiftMotor.get_position() > lowTowerLower))) {
          // Continue running this loop as long as the motor is not within +-5 units of its goal
          pros::delay(2);
      }

      break;

    case 2:
      // move to heigh pole position
      rightLiftMotor.move_absolute(LIFT_LEVEL_HIGH, speed); // Moves LIFT_LOW_POLE units forward
      leftLiftMotor.move_absolute(LIFT_LEVEL_HIGH, speed); // Moves LIFT_LOW_POLE units forward
      while (!((-rightLiftMotor.get_position() < highTowerUpper) && (-rightLiftMotor.get_position() > highTowerLower))) {
          // Continue running this loop as long as the motor is not within +-5 units of its goal
          pros::delay(2);
      }
      break;
  }
  if(DEBUG_ON){
    std::cout << "Lift Encoder: -- M1: " << rightLiftMotor.get_position();
    pros::delay(2);           // Give thread time to catch up.
  }
}

void debugToBrain() {

  pros::lcd::clear_line(5);
  pros::lcd::clear_line(6);
  pros::lcd::print(5, "Motor output: %d\n", rightLiftMotor.get_efficiency());
  pros::lcd::print(6, "Motor output: %d\n", leftLiftMotor.get_efficiency());

}
