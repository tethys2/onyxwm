/**
 * @file debug.c
 * @brief Provides functions for debuging including logging to a file
 */

#include <stdarg.h> //variadic funcs
#include <stdio.h> //printf() fprintf() fopen()
#include <time.h> //time stamps
#include <fcntl.h> // open() openat()
#include <sys/stat.h> //mkdir()
#include <stdlib.h> //getenv()
#include <string.h> //strcat() strcpy()
#include <stdbool.h> //bool type

#include "debug.h"

static FILE *LOG_FILE = NULL; // Log file handle

/**
 * @brief Opens a log file called onyxwm.log
 *
 * Will not create ./local/share, but will create ./local/share/onyxwm
 * Uses LOG_FILE to store log file location
 *
 * @return Whether the file successfully opened
 *
 */
bool openLog(){
	if(LOG_FILE != (FILE*)NULL){
		// if not null then the handle has already been opened
		return true;
	}
	// directory fd to the home directory
	int home_dir = open(getenv("HOME"), O_DIRECTORY | O_RDONLY);
	// if .local doesn't exist, it shouldn't be created
	// if .local/share doesn't exits, it shouldn't be created
	// this checks for both
	if(openat(home_dir, ".local/share", O_RDONLY | O_DIRECTORY) < 0){
		fprintf(stderr, "~/.local/share does not exist or can't be read, can't open log file\n");
		return false;
	}
	// if the sub-directory for onyxwm doesn't exist, create it
	if(openat(home_dir, ".local/share/onyxwm", O_RDONLY| O_DIRECTORY) < 0){
		printf("~/.local/share/onyxwm does not exist, creating\n");
		if(mkdirat(home_dir, ".local/share/onyxwm", 0777) < 0){
			fprintf(stderr,"Couldn't create ~/.local/share/onyxwm\n");
			return false;
		}
	}
	// using fopen, and there is no fopenat, so the log dir must be concated
	char log_path[99] = "";
	strcpy(log_path, getenv("HOME"));
	strcat(log_path, "/.local/share/onyxwm/onyxwm.log");
	// if the file doesn't exist, this will create it, assuming the directory exists
	LOG_FILE = fopen(log_path, "w");
	// if fopen fails LOG_FILE will be null
	return LOG_FILE != NULL;
};

/**
 * @brief Log a message to the log file
 *
 * Adds a timestamp, message type, then message with subsitutions
 *
 * @param type Type of log message (eg. ERR or MSG)
 * @param msg Message to be logged
 * @param args Arguments (subsitutions) for message
 */
void flog(char *type, const char *msg, va_list args){
	// if open fails, return, logging to files is optional
	if(!openLog()) return;
	char stamp[42];
	// get formated time
	strftime(stamp, sizeof(stamp), "%x - %X", localtime(&(time_t){time(NULL)}));
	// print timestamp
	fprintf(LOG_FILE,"[%s]", stamp);
	// print type of message, e.g. MSG or ERR
	fprintf(LOG_FILE, " %s: ", type);
	// print the message
	vfprintf(LOG_FILE, msg, args);
	// make sure the log file updates immediately
	// only updates after program close otherwise
	fflush(LOG_FILE);
}

// see debug.h
void logMessage(const char *msg, ...){
	va_list args;
	va_start(args, msg);
	// print to stdout
	printf(msg, args);
	//log to file with type MSG
	flog("MSG", msg, args);
	va_end(args);
}

// see debug.h
void logError(const char *msg, ...){
	va_list args;
	va_start(args, msg);
	// print to stderr
	fprintf(stderr, msg, args);
	// log to file with type ERR
	flog("ERR", msg, args);
	va_end(args);
}
