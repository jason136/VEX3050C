#include "main.h"
#include "init.h"
#include "portdef.h"
#include "auto.h"
#include "chassis.h"
#include "lift.h"
#include "roller.h"

// Character array holdign the vaious labels for the autonomous routines
// labels are the function names being called
const char* titles[] = {"Skills", "autoRedLeft", "AutoBlueLeft", "autoRedRight", "autoBlueRight"};

int selection;													// Select script to run
unsigned int scriptNumber = 0;					// scriptNumber which will be passed

static bool selectionMade = false;			// Sart fresh - no selection made yet

// left LCD display button call back routine
// Button pressed decrements in the selection array
void on_left_button() {
	static bool pressed = false;
  if(!selectionMade){
		pros::lcd::clear_line(4);
		if (scriptNumber != 0) {
			scriptNumber--;
		  pros::lcd::print(2, "Script#: %d\n", scriptNumber);
      pros::lcd::print(3, titles[scriptNumber]);
 	  } else {
		  pros::lcd::print(2, "Script#: %d Can't decrement\n", scriptNumber);
		  pros::lcd::print(3, titles[scriptNumber]);
	  }
	}
}

// center button, make a selection or deselect the choosen option
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		selection = scriptNumber;
		pros::lcd::set_text(4, "<< SELECTED ! >>");
		selectionMade = true;
	} else {
		pros::lcd::set_text(4, "<< DE-SELECTED >>");
		selection = 0;					// Reset selection
    selectionMade = false;
	}
}

// right button, increments the array index to next selection
void on_right_button() {
	static bool pressed = false;
  if(!selectionMade){
		pros::lcd::clear_line(4);
		if (scriptNumber < (NUM_SCRIPTS - 1)) {
			scriptNumber++;
		  pros::lcd::print(2, "Script#: %d\n", scriptNumber);
		  pros::lcd::print(3, titles[scriptNumber]);
		} else {
			pros::lcd::print(2, "Script#: %d Can't increment\n", scriptNumber);
		  pros::lcd::print(3, titles[scriptNumber]);
		}
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// Motor Setup
	// GEARSET_36 -- RED
	// GEARSET_18 -- GREEN (default)
	// GEARSET_6 -- BLUE

	pros::Motor front_right_motor(FRONT_RIGHT_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor front_left_motor(FRONT_LEFT_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor back_right_motor(BACK_RIGHT_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor back_left_motor(BACK_LEFT_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

	pros::Motor tray_motor(TRAY_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

	pros::Motor lift_motor(LIFT_MOTOR, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);

	pros::Motor right_roller_motor(RIGHT_ROLLER_MOTOR, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor left_roller_motor(LEFT_ROLLER_MOTOR, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

	pros::lcd::initialize();
	pros::lcd::print(1, "Build 2021v1 unstable");
	pros::lcd::print(2, "Script#: %d\n", scriptNumber);
	pros::lcd::print(3, titles[scriptNumber]);

	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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

void autonomous() {
	// We are calling a autonomous function based on the selection
  // we made on the LCD
	switch(selection) {
		case 0 :
			pros::lcd::print(4, "Script#: %d\n", selection);
      pros::lcd::print(5, titles[selection]);
      skillRun();
      break;

		case 1 :
			pros::lcd::print(4, "Script#: %d\n", selection);
      pros::lcd::print(5, titles[selection]);
      autoRedLeft();
      break;

    case 2 :
      pros::lcd::print(4, "Script#: %d\n", selection);
      pros::lcd::print(5, titles[selection]);
    	autoBlueLeft();
      break;

    case 3 :
    	pros::lcd::print(4, "Script#: %d\n", selection);
      pros::lcd::print(5, titles[selection]);
      autoRedRight();
      break;

    case 4 :
      pros::lcd::print(4, "Script#: %d\n", selection);
      pros::lcd::print(5, titles[selection]);
      autoBlueRight();
      break;

    default :
      // this should never happen as selection is alwasy inialized as 0
      // does the case of '0' is in essence the defualt.
      break;
  }
}
