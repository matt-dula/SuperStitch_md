#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "common.h"

// use the standard namespace
using namespace std;
using namespace exploringBB;


// function protoypes
void change_pin_state(GPIO::VALUE, GPIO*);
GPIO_INPUT_STATE_TYPE check_gpio_input_state(GPIO*);

// turn the led on or off based on the ledState parameter
void change_pin_state(GPIO::VALUE command, GPIO* pin){
	static bool first_time = true;
	if(first_time){
    	pin->setDirection(GPIO::OUTPUT);
    	first_time = false;
	}
	
	pin->setValue(command);
}


                 
GPIO_INPUT_STATE_TYPE check_gpio_input_state(GPIO* input_pin){
	// create static variables for current and previous pin input levels
	// Note on static variables: static variables retain their values even after
	// the function exits because they are stored in the static section of memory
	// rather than the stack.  They are initialized to their initial values the
	// first time the function is called, but then they are not re-initialized
	// each time the function is called
	static GPIO::VALUE current_gpio_input_level[NUM_BBB_PINS];
	static GPIO::VALUE prev_gpio_input_level[NUM_BBB_PINS];
	
	// the following few lines of code are intended to be executed one time to
	// determine the initial state of the pin.  This will prevent incorrectly
	// detecting an edge the first time the code is run
	static bool first_time = true;
	if(first_time)
	{
		current_gpio_input_level[input_pin->getNumber()] = input_pin->getValue();
		prev_gpio_input_level[input_pin->getNumber()] = current_gpio_input_level[input_pin->getNumber()];
		first_time = false;
	}
	
	// create and initialize gpio_input_state
	// the initialization value is arbitrary because it will be
	// updated by the logic of the function
	GPIO_INPUT_STATE_TYPE gpio_input_state = GPIO_INPUT_STATE_LOW;
	
	// save the input level that you read last time you called the function
	// into prev_gpio_input_level
	prev_gpio_input_level[input_pin->getNumber()] = current_gpio_input_level[input_pin->getNumber()];
	
	// read a new value from the input pin
	current_gpio_input_level[input_pin->getNumber()] = input_pin->getValue();
	
	// use the current and previous levels to set gpio_input_state
	// use the UML Activity Diagram from the lab manual to guide your logic
	if(prev_gpio_input_level[input_pin->getNumber()] == GPIO::LOW && current_gpio_input_level[input_pin->getNumber()] == GPIO::HIGH)
	{
		gpio_input_state = GPIO_INPUT_STATE_RISING_EDGE;
	}
	else if(prev_gpio_input_level[input_pin->getNumber()] == GPIO::HIGH && current_gpio_input_level[input_pin->getNumber()] == GPIO::HIGH)
	{
		gpio_input_state = GPIO_INPUT_STATE_HIGH;
	}
	else if(prev_gpio_input_level[input_pin->getNumber()] ==  GPIO::HIGH && current_gpio_input_level[input_pin->getNumber()] ==  GPIO::LOW)
	{
		gpio_input_state = GPIO_INPUT_STATE_FALLING_EDGE;
	}
	else
	{
		gpio_input_state = GPIO_INPUT_STATE_LOW;
	}
	
	return gpio_input_state;
}

#endif /* UTILITIES_H_ */