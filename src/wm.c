#include <xcb/xcb.h>      // XCB core header
#include <xcb/xcb_keysyms.h> // XCB helper for key symbol handling
#include <stdlib.h>       // exit, free
#include <unistd.h>       // fork, execlp
#include <sys/types.h>    // pid_t

// Already declared in main, needed to handle xcb stuff
extern xcb_connection_t *dpy;
// Already declared in main, current screen
extern xcb_screen_t *scre;

#include "debug.h"
#include "config/autostart.h"

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
	if(!ret){
		log_msg("Exiting onyxWM successfully\n");
	}
	else{
		log_msg("Exiting onyxWM with error\n");
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

void focusWin(xcb_drawable_t win) {
	if ((win != 0) && (win != scre->root)){
		xcb_set_input_focus(dpy, XCB_INPUT_FOCUS_POINTER_ROOT, win, XCB_CURRENT_TIME);
	}
}

// converts a xcb_keycode_t to a xcb_keysym_t
xcb_keysym_t getKeysym(xcb_keycode_t keycode){
	// Creates mapping table for keysyms
	xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(dpy);
	// convert keysym if mapping table exists
	xcb_keysym_t keysym = (keysyms ? xcb_key_symbols_get_keysym(keysyms, keycode, 0) : 0);
	// mapping table needs to be freed since we allocated it
	xcb_key_symbols_free(keysyms);
	return keysym;
}
// converts a xcb_keysym_t to xcb_keycode_t 
xcb_keycode_t *getKeycode(xcb_keysym_t keysym){
	// Creates mapping table for keysyms
	xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(dpy);
	// convert keycode if mapping table exists
	xcb_keycode_t *keycode = (keysyms ? xcb_key_symbols_get_keycode(keysyms, keysym) : NULL);
	// mapping table needs to be freed since we allocated it
	xcb_key_symbols_free(keysyms);
	return keycode;
}
