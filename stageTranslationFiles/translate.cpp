// include iostream for console input/output
#include <iostream>
// include fstream for file input/output
#include <fstream>
// include string to use c-style string functions for argument parsing
#include <string.h>
// include signal to be able to catch and handle signals
#include <signal.h>
// include unistd to have access to linux functions such as usleep
#include <unistd.h>
// include Exploring BeagleBone GPIO library
#include "gpio/GPIO.h"
// include utiltiies.h
#include "utilities.h"
//include ctime library 
#include <time.h>
// include stringstream library
#include <sstream>
// include c standard io for sprintf function
#include <cstdio>

#define BILLION 1000000000.0

static GPIO optoY(48);
static GPIO pulY(49);
static GPIO dirY(115);
static GPIO enaY(112);

static GPIO optoX(66);
static GPIO pulX(69);
static GPIO dirX(45);
static GPIO enaX(47);

// implement signal handler
void sig_handler(int signo)
{
    if (signo == SIGINT)
        cout << "received SIGINT" << endl;
    
    // turn motors off 

	
	optoX.setValue(GPIO::LOW);
	pulX.setValue(GPIO::LOW);
	dirX.setValue(GPIO::LOW);
	enaX.setValue(GPIO::LOW);
	
	optoY.setValue(GPIO::LOW);
	pulY.setValue(GPIO::LOW);
	dirY.setValue(GPIO::LOW);
	enaY.setValue(GPIO::LOW);
	
	
    
    // sleep for a second to allow actions to take effect
    sleep(1);
    
    // exit the program
    exit(0);
}





int main(int argc, char *argv[])
{
    // set up signal handler
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		cout << "can't catch SIGINT" << endl;
	else
		cout << "Successfully set up SIGINT handler" << endl;
		
	
	//create unit32_t variables to set max position of the stage 
	//FIXME 
	const uint32_t MAX_X_POSITION = 7000;
	//one revolution with 1000 usleep and 875 count means 8 mmm movement 
	//30.
	//80
	const uint32_t MAX_Y_POSITION = 300;
	//FIXME
	uint32_t NUM_ROWS = 0;
	uint32_t Y_REWIND = 0;
	
	const uint32_t PUL_SLEEP = 2000;
	const uint32_t SIGNAL_SLEEP = 10;
	const uint32_t STATE_SLEEP = 500;
	const uint32_t FILE_SLEEP = 150000;
	
	const int MOD_NUM = 100;
	// declare variables
	uint32_t x_position = 0;
	uint32_t y_position = 0;
	uint32_t num_comp_rows = 0;
	
	//DIR HIGH is NEGATIVE 
	MOTOR_TURN motor_state = READY;
	MOTOR_TURN next_x_motor_state = IDLE;
	
	char camBashCommand [50];
	string imgFileName;
	
	GPIO e_stop(65);
	e_stop.setDirection(GPIO::INPUT);
	
	GPIO e_stop_signal(27);
	e_stop_signal.setDirection(GPIO::OUTPUT);

	optoX.setDirection(GPIO::OUTPUT);
	pulX.setDirection(GPIO::OUTPUT);
	dirX.setDirection(GPIO::OUTPUT);
	enaX.setDirection(GPIO::OUTPUT);
	
	optoY.setDirection(GPIO::OUTPUT);
	pulY.setDirection(GPIO::OUTPUT);
	dirY.setDirection(GPIO::OUTPUT);
	enaY.setDirection(GPIO::OUTPUT);
	
	double difference;
	struct timespec start, end;
	int com = -1;
	int size = -1;
	
	
	
	fstream sizeInFile;
	fstream commandInFile;
	fstream timeInfo;
	fstream positionFile;
	fstream fileNameFile;
	ifstream readpos;
	
	commandInFile.open("./command_file.txt");
	usleep(FILE_SLEEP); 
    commandInFile << com;
    commandInFile.close();
     
    sizeInFile.open("./size_file.txt");
	usleep(FILE_SLEEP); 
    sizeInFile << size;
    sizeInFile.close();
    
    positionFile.open("./position_file.txt");
	usleep(FILE_SLEEP); 
	timeInfo.flush();
	usleep(FILE_SLEEP);
    positionFile << x_position << " " << y_position << endl;
    
    timeInfo.open("./timing.txt");
	usleep(FILE_SLEEP); 
    timeInfo.flush();
    usleep(FILE_SLEEP);
    timeInfo.close();
    
    string posFileName = "position_file.txt";
	
    // evaluate command line arguments
    // if there are not exactly 2 arguments in the argument count
    // print an error and return to exit the program
	if(argc != 2)
	{
		printf("Error: Incorrect number of arguments\n");
		return 0;
	}
	// if there are 3 arguments, evaluate themmae
	else
	{
	    // if the second argument is "on", then init the LED to on
		if(!strcmp(argv[1], "on"))
		{
			//led_state = STATE_ON;
			printf("*** Init LED ON ****\n");
		}
	    // if the second argument is "off", then init the LED to off
		else if(!strcmp(argv[1], "off"))
		{
			//led_state = STATE_OFF;
			printf("*** Init LED OFF ****\n");
		}
		// if the second argument is neither on nor off, print an
		// error and exit the program
		else
		{
		    cout << "Error: Bad initialization state given, terminating" << endl;
		    exit(0);
		}
	}
	
	
	
	usleep(10);
	
	// change the led state to either turn on or off the LED
	// based on the led_state set above

	
	optoY.setValue(GPIO::HIGH);
	optoX.setValue(GPIO::HIGH);
	usleep(5);
	
	
	
	
	// start while(1) loop
	// this loop can be broken by the signal handler receiving a signal
	// this is basic flashlight logic you have seen before
	
	
	
	while(1){
		
		
		switch(motor_state){
			case READY: 
			
				positionFile.flush();
				timeInfo.flush();
				
				
				enaX.setValue(GPIO::LOW);
				enaY.setValue(GPIO::LOW);
				usleep(SIGNAL_SLEEP);
				
				sizeInFile.open("./size_file.txt");
		        usleep(FILE_SLEEP); 
	    	    sizeInFile >> size;
	    	    sizeInFile.close();
	    	    
	    	    if(size != -1){
	    	    	if(size == 1){
	    	    		NUM_ROWS = 10;
						Y_REWIND = MAX_Y_POSITION * NUM_ROWS;
	    	    	}
	    	    	
	    	    	if(size == 2){
	    	    		NUM_ROWS = 20;
						Y_REWIND = MAX_Y_POSITION * NUM_ROWS;
	    	    	}
	    	    }
				
				cout << "IN READY" << endl;
				commandInFile.open("./command_file.txt");
		        usleep(FILE_SLEEP);
	    	    commandInFile >> com;
	    	    commandInFile.close();
	    	   
				
				if (com == 0){
					fileNameFile.open("./file_name.txt");
					usleep(FILE_SLEEP);
					fileNameFile >> imgFileName;
					fileNameFile.close();
					
					sprintf(camBashCommand, "./run_camera.sh %i %s &", size, imgFileName.c_str());
					system(camBashCommand);
					
					motor_state = POSITIVE_X;
					positionFile.flush();
					
					clock_gettime(CLOCK_MONOTONIC, &start);
					timeInfo.open("./timing.txt");
					usleep(FILE_SLEEP); 
				} 
				
				usleep(STATE_SLEEP);
				
			break;
			
			case IDLE:
			
				positionFile.close();
				timeInfo.close();
			
				enaX.setValue(GPIO::LOW);
				enaY.setValue(GPIO::LOW);
				usleep(SIGNAL_SLEEP);
				
				cout << "IN IDLE" << endl;
				commandInFile.open("./command_file.txt");
		        usleep(FILE_SLEEP); 
	    	    commandInFile >> com;
	    	    commandInFile.close();
	    	    
	    	    cout << x_position << endl;
	    	    cout << y_position << endl;
	    	   
				
				if (com == 2){
					motor_state = REWIND;
				}
				
				usleep(STATE_SLEEP);
			break;
			
			case POSITIVE_X:
				cout<< "IN POSITIVE_X" << endl;
				
				
				enaY.setValue(GPIO::LOW);
				usleep(SIGNAL_SLEEP);
				enaX.setValue(GPIO::HIGH);
				usleep(SIGNAL_SLEEP);
			
				dirX.setValue(GPIO::LOW);
				usleep(SIGNAL_SLEEP);
				
				commandInFile.open("./command_file.txt");
		        usleep(FILE_SLEEP); 
	    	    commandInFile >> com;
	    	    commandInFile.close();
	    	    
				clock_gettime(CLOCK_MONOTONIC, &end);		/* mark the end time */
				difference = double(end.tv_sec - start.tv_sec)  + (double(end.tv_nsec - start.tv_nsec) / BILLION);
				
				
				timeInfo << difference << endl; 
				cout << difference << endl; 
				
				while(x_position < MAX_X_POSITION){
					pulX.setValue(GPIO::HIGH);
					usleep(PUL_SLEEP);
					pulX.setValue(GPIO::LOW);
					usleep(PUL_SLEEP);
					x_position++;
					
					if((x_position % MOD_NUM) == 0){
						positionFile << x_position << " " << y_position << endl;
					}
				}
				
				clock_gettime(CLOCK_MONOTONIC, &end);		/* mark the end time */
				difference = double(end.tv_sec - start.tv_sec)  + double((end.tv_nsec - start.tv_nsec) / BILLION);
				
				
				timeInfo << difference << endl; 
				cout << difference << endl; 
				
				commandInFile.open("./command_file.txt");
		        usleep(FILE_SLEEP); 
	    	    commandInFile >> com;
	    	    commandInFile.close();
	    	    
				if((x_position == MAX_X_POSITION) && (num_comp_rows < NUM_ROWS)){
					num_comp_rows++;
					
					motor_state = POSITIVE_Y;
					if(com == 1){
						motor_state = IDLE;
					}
					next_x_motor_state = NEGATIVE_X;
					usleep(SIGNAL_SLEEP);
				}
				
				if(num_comp_rows == NUM_ROWS ){
						motor_state = IDLE;
						
						clock_gettime(CLOCK_MONOTONIC, &end);		/* mark the end time */
						difference = double(end.tv_sec - start.tv_sec)  + double((end.tv_nsec - start.tv_nsec) / BILLION);
				
				
						timeInfo << difference << endl; 
						cout << difference << endl; 
								
						timeInfo.close();
						usleep(FILE_SLEEP);
						system("./time_scp.sh");
				}
				
				enaX.setValue(GPIO::LOW);
				
				usleep(STATE_SLEEP);
				
			break;
			
			case NEGATIVE_X:
			
				cout << "IN NEGATIVE_X" << endl;
				
				enaY.setValue(GPIO::LOW);
				enaX.setValue(GPIO::HIGH);
				usleep(SIGNAL_SLEEP);
				
				commandInFile.open("./command_file.txt");
		        usleep(FILE_SLEEP); 
	    	    commandInFile >> com;
	    	    commandInFile.close();
	    	    
				dirX.setValue(GPIO::HIGH);
				usleep(SIGNAL_SLEEP);
				
				clock_gettime(CLOCK_MONOTONIC, &end);		/* mark the end time */
				difference = double(end.tv_sec - start.tv_sec)  + double((end.tv_nsec - start.tv_nsec) / BILLION);
				
				
				timeInfo << difference << endl; 
				cout << difference << endl; 
				
				
				while(x_position > 0){
					pulX.setValue(GPIO::HIGH);
					usleep(PUL_SLEEP);
					pulX.setValue(GPIO::LOW);
					usleep(PUL_SLEEP);
					
					x_position--;
					
					
					if((x_position % MOD_NUM) == 0){
						positionFile << x_position << " " << y_position << endl;
					}
				}
				
				clock_gettime(CLOCK_MONOTONIC, &end);		/* mark the end time */
				difference = double(end.tv_sec - start.tv_sec)  + double((end.tv_nsec - start.tv_nsec) / BILLION);
				
				
				timeInfo << difference << endl; 
				cout << difference << endl; 
				
				
				if(x_position == 0){
					motor_state = POSITIVE_Y;
					next_x_motor_state = POSITIVE_X;
					if(com == 1){
						motor_state = IDLE;
					}
					usleep(SIGNAL_SLEEP);
				}
				
				enaX.setValue(GPIO::LOW);
				
				usleep(STATE_SLEEP);
				
			break;
			
			case POSITIVE_Y:
			
				cout << "IN POSITIVE Y" << endl;
				
				enaX.setValue(GPIO::LOW);
				enaY.setValue(GPIO::HIGH);
				usleep(SIGNAL_SLEEP);
				
				dirY.setValue(GPIO::LOW);
				usleep(SIGNAL_SLEEP);
				
				commandInFile.open("./command_file.txt");
		        usleep(FILE_SLEEP); 
	    	    commandInFile >> com;
	    	    commandInFile.close();
				
			    clock_gettime(CLOCK_MONOTONIC, &end);		/* mark the end time */
				difference = double(end.tv_sec - start.tv_sec)  + double((end.tv_nsec - start.tv_nsec) / BILLION);
				
				
				timeInfo << difference << endl; 
				cout << difference << endl; 
				
				for(int i = 0; i <  MAX_Y_POSITION; i++){
					
					pulY.setValue(GPIO::HIGH);
				    usleep(PUL_SLEEP);
					pulY.setValue(GPIO::LOW);
					usleep(PUL_SLEEP);
					
					y_position++;
					
					if((y_position % MOD_NUM) == 0){
						positionFile << x_position << " " << y_position << endl;
					}
				}	
				
				commandInFile.open("./command_file.txt");
		        usleep(FILE_SLEEP); 
	    	    commandInFile >> com;
	    	    commandInFile.close();
				
				clock_gettime(CLOCK_MONOTONIC, &end);		/* mark the end time */
				difference = double(end.tv_sec - start.tv_sec)  + double((end.tv_nsec - start.tv_nsec) / BILLION);
				
				
				timeInfo << difference << endl; 
				cout << difference << endl; 
				
				motor_state = next_x_motor_state;
				
				
				if(y_position == Y_REWIND){
					motor_state = IDLE;
					if(com == 1){
						motor_state = IDLE;
					}
					cout << y_position << endl;
				}
				
				enaY.setValue(GPIO::LOW);
				
				usleep(STATE_SLEEP);
			break;
			
			case NEGATIVE_Y:
			    enaX.setValue(GPIO::LOW);
				enaY.setValue(GPIO::HIGH);
				usleep(SIGNAL_SLEEP);
				
				//FIXME 
				dirY.setValue(GPIO::HIGH);
				usleep(SIGNAL_SLEEP);
				
				pulY.setValue(GPIO::HIGH);
			    usleep(PUL_SLEEP);
				pulY.setValue(GPIO::LOW);
				usleep(PUL_SLEEP);
				
				
				y_position--;
				
				usleep(STATE_SLEEP);
				
			break;
			
			case REWIND:
			
				cout << "REWINDING" << endl;
				
				
				
				enaX.setValue(GPIO::LOW);
				enaY.setValue(GPIO::HIGH);
				usleep(SIGNAL_SLEEP);
				
				dirY.setValue(GPIO::HIGH);
				
				for(int i = 0; i < NUM_ROWS; i++){
					for(int j = 0; j <  MAX_Y_POSITION; j++){
					
						pulY.setValue(GPIO::HIGH);
					    usleep(PUL_SLEEP);
						pulY.setValue(GPIO::LOW);
						usleep(PUL_SLEEP);
						if((y_position % MOD_NUM) == 0){
							positionFile << x_position << " " << y_position << endl;
						}
					}
					
					usleep(FILE_SLEEP);
					
				}
				
				
				enaY.setValue(GPIO::LOW);
				enaX.setValue(GPIO::HIGH);
				usleep(SIGNAL_SLEEP);
				
				dirX.setValue(GPIO::HIGH);
				usleep(SIGNAL_SLEEP);
				while(x_position >  0){
					pulX.setValue(GPIO::HIGH);
					usleep(PUL_SLEEP);
					pulX.setValue(GPIO::LOW);
					usleep(PUL_SLEEP);
					x_position--;
				}
				
				
				
				optoX.setValue(GPIO::LOW);
				pulX.setValue(GPIO::LOW);
				dirX.setValue(GPIO::LOW);
				enaX.setValue(GPIO::LOW);
				
				optoY.setValue(GPIO::LOW);
				pulY.setValue(GPIO::LOW);
				dirY.setValue(GPIO::LOW);
				enaY.setValue(GPIO::LOW);
				
				motor_state = READY;
				
				usleep(STATE_SLEEP);
				
			
				
			break;
		}
		
		
	
		// usleep delays by a certain number of microseconds
		// delay is necessary to yield the processor and to
		// filter out noise since we don't have a capacitor
		// in the circuit
		usleep(1000);
	}
	
	
/*
for(int j = 0; j < 3; j++){

	enaX.setValue(GPIO::HIGH);
	usleep(SIGNAL_SLEEP);
	dirX.setValue(GPIO::LOW);
	usleep(SIGNAL_SLEEP);
	
		for(int i = 0; i < MAX_X_POSITION; i++){
				pulX.setValue(GPIO::HIGH);
				usleep(PUL_SLEEP);
				pulX.setValue(GPIO::LOW);
				usleep(PUL_SLEEP);
		}
		
    enaX.setValue(GPIO::LOW);
    usleep(FILE_SLEEP);
    
    enaY.setValue(GPIO::HIGH);
	usleep(SIGNAL_SLEEP);
	dirY.setValue(GPIO::LOW);
	usleep(SIGNAL_SLEEP);
	
		for(int i = 0; i < MAX_Y_POSITION; i++){
				pulY.setValue(GPIO::HIGH);
				usleep(PUL_SLEEP);
				pulY.setValue(GPIO::LOW);
				usleep(PUL_SLEEP);
		}
		
    enaY.setValue(GPIO::LOW);
    usleep(FILE_SLEEP);
    
    enaX.setValue(GPIO::HIGH);
	usleep(SIGNAL_SLEEP);
	dirX.setValue(GPIO::HIGH);
	usleep(SIGNAL_SLEEP);
	
		for(int i = 0; i < MAX_X_POSITION; i++){
				pulX.setValue(GPIO::HIGH);
				usleep(PUL_SLEEP);
				pulX.setValue(GPIO::LOW);
				usleep(PUL_SLEEP);
		}
		
    enaX.setValue(GPIO::LOW);
    usleep(SIGNAL_SLEEP);
    
    enaY.setValue(GPIO::HIGH);
	usleep(SIGNAL_SLEEP);
	dirY.setValue(GPIO::LOW);
	usleep(SIGNAL_SLEEP);
	
		for(int i = 0; i < MAX_Y_POSITION; i++){
				pulY.setValue(GPIO::HIGH);
				usleep(PUL_SLEEP);
				pulY.setValue(GPIO::LOW);
				usleep(PUL_SLEEP);
		}
		
    enaY.setValue(GPIO::LOW);
    usleep(FILE_SLEEP);
}

   */
/*
  	enaX.setValue(GPIO::HIGH);
	usleep(SIGNAL_SLEEP);
	dirX.setValue(GPIO::HIGH);
	usleep(SIGNAL_SLEEP);
	
		for(int i = 0; i < MAX_X_POSITION; i++){
				pulX.setValue(GPIO::HIGH);
				usleep(PUL_SLEEP);
				pulX.setValue(GPIO::LOW);
				usleep(PUL_SLEEP);
		}
		
    enaX.setValue(GPIO::LOW);
    usleep(FILE_SLEEP);
  */

}	
