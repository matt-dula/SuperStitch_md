#ifndef COMMON_H_
#define COMMON_H_

// create enumerated types for button and led states
typedef enum{
	STATE_OFF = 0,
	STATE_ON  = 1
}STATE_TYPE;

// gpio input state enumeration
// "state" indicates the state of the button
// when taking the previous state into account
typedef enum{
	GPIO_INPUT_STATE_LOW = 0,
	GPIO_INPUT_STATE_RISING_EDGE = 1,
	GPIO_INPUT_STATE_HIGH = 2,
	GPIO_INPUT_STATE_FALLING_EDGE = 3
}GPIO_INPUT_STATE_TYPE;

//create a MOTOR_TURN enum to keep track of which state the Motor is in 
typedef enum{
	IDLE = 0,
	READY = 1,
	POSITIVE_X  = 2,
	NEGATIVE_X = 3,
	POSITIVE_Y = 4,
	NEGATIVE_Y  = 5,
	REWIND = 6
}MOTOR_TURN;


#define NUM_BBB_PINS 128
#endif /* COMMON_H_ */