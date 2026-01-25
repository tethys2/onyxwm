/**
 * @file cmdline.c
 * @see cmdline.h
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief prints version info
 */
void printVersion(){
	// VERSION defined by build system
	printf("OnyxWM %s\n",VERSION);
};

/**
 * @brief prints a help message to stdout
 */
void printHelp(){
	printVersion();
	printf("\nUSAGE: onyxwm [options]\n");
	printf("\nOPTIONS:\n");
	printf("--help                   print this message\n");
	printf("--version                print version information\n");
};


// see cmdline.h
int parseArguments(int argc, char* argv[]){
	//so we can print all the unknown args before returing -1
	bool unknown_arg_found;
	// skip the first arg since it is the name of the executable
	for(int c = 1; c < argc; ++c){
		// if there is no diffrence between the strings
		if(!strcmp(argv[c],"--help")){
			printHelp();
			return 1;
		} else if (!strcmp(argv[c],"--version")) {
			printVersion();
			return 1;
		} else {
			printf("Unkown argument: %s\n",argv[c]);
			unknown_arg_found = true;
		}
	}
	if(unknown_arg_found){
		return -1;
	}
	return 0;
};
