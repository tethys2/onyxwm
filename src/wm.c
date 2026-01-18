#include <xcb/xcb.h>      // XCB core header
#include <stdio.h>        // printf, fprintf
#include <stdlib.h>       // exit, free
#include <unistd.h>       // fork, execlp
#include <sys/types.h>    // pid_t


// Already declared in main, needed to handle xcb stuff
extern xcb_connection_t *dpy;

//block needed for config
#include <X11/keysym.h>
#include <xcb/xcb_keysyms.h>
#include "wm.h"
#include "types.h"
#include "actions.h"
#include "config.h"

// spawn program
// char **program is an array of arguments for the program ending in null
void spawn(char **program) {
	if (fork() == 0) { //fork and continue if the program is the child
		// Child process
		setsid(); // Start a new session (detach from controlling terminal)
		execvp((char*)program[0], (char**)program); // Replace process image with program
		_exit(0); //Exit once the process is over
	}
	// Parent process does nothing; child runs the program
}
// kill the window manager, exiting with the inputed return value
void exitWM(int ret){
	//disconnect
	if(dpy){
		xcb_disconnect(dpy);
		dpy = NULL;
	}
	if(ret){
		printf("Exiting onyxWM successfully");
	}
	else{
		printf("Exiting onyxWM with error");
	}
	//exit program with code from argument
	exit(ret);
}

// starts the apps in autostart_list
void autostart() {
	// iterate over the autostart list in config until NULL
	for(int i = 0; autostart_list[i][0] != NULL; i++){
		// spawn the program in the current index
		spawn(autostart_list[i]);
	}
}


