#include <xcb/xcb.h>      // XCB core header
#include <xcb/xcb_keysyms.h> // XCB helper for key symbol handling
#include <stdio.h>        // printf, fprintf
#include <stdlib.h>       // exit, free
#include <unistd.h>       // fork, execlp
#include <sys/types.h>    // pid_t

#include "debug.h"

// Already declared in main, needed to handle xcb stuff
extern xcb_connection_t *dpy;
// Already declared in main, current screen
extern xcb_screen_t *scre;
// from events.c, needed to alter window attributes (eg. for focus)
extern uint32_t geometry_buf[];

#include "config/autostart.h"

/**
 * @brief Spawn a program by forking and then becoming the program
 *
 * @param program A list of arguments starting with the program name/location and ending in NULL
 */
void spawn(char **program) {
	if (fork() == 0) { //fork and continue if the program is the child
		// Child process
		setsid(); // Start a new session (detach from controlling terminal)
		execvp((char*)program[0], (char**)program); // Replace process image with program
		_exit(0); //Exit once the process is over
	}
	// Parent process does nothing; child runs the program
}
/**
 * @brief Clean up the program and exit
 *
 * @param ret What to exit with: 0 for success, 1 for failure
 */
void exitWM(int ret){
	//disconnect
	if(dpy){
		xcb_disconnect(dpy);
		dpy = NULL;
	}
	if(!ret){
		logMessage("Exiting onyxWM successfully\n", 1);
	}
	else{
		logError("Exiting onyxWM with error\n", 1);
	}
	//exit program with code from argument
	exit(ret);
}

/**
 * @brief Spawn all programs in autostart_list from config/autostart.h
 */
void autostart() {
	// iterate over the autostart list in config until NULL
	for(int i = 0; autostart_list[i][0] != NULL; i++){
		// spawn the program in the current index
		spawn(autostart_list[i]);
	}
}

/**
 * @brief Raise a window by setting its XCB_CONFIG_WINDOW_STACK_MODE to XCB_STACK_MODE_ABOVE
 *
 * @param win The window to be raised
 */
void raiseWin(xcb_drawable_t win) {
	geometry_buf[0] = XCB_STACK_MODE_ABOVE;
	xcb_configure_window(dpy, win, XCB_CONFIG_WINDOW_STACK_MODE, geometry_buf);
}

/**
 * @brief Give a window input focus if not root or undefined
 *
 * @param win The window to give focus
 */
void focusInput(xcb_drawable_t win) {
	if ((win != 0) && (win != scre->root)){
		xcb_set_input_focus(dpy, XCB_INPUT_FOCUS_POINTER_ROOT, win, XCB_CURRENT_TIME);
	}
}

/**
 * @brief Convert an xcb_keycode_t into an xcb_keysym_t by creating and destroying a key symbols table
 *
 * @param keycode The keycode to be converted
 * @return The keysym
 */
xcb_keysym_t getKeysym(xcb_keycode_t keycode){
	// Creates mapping table for keysyms
	xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(dpy);
	// convert keysym if mapping table exists
	xcb_keysym_t keysym = (keysyms ? xcb_key_symbols_get_keysym(keysyms, keycode, 0) : 0);
	// mapping table needs to be freed since we allocated it
	xcb_key_symbols_free(keysyms);
	return keysym;
}

/**
 * @brief Convert an xcb_keysym_t into an xcb_keycode_t by creating and destroying a key symbols table
 *
 * @param keysym The keysymbol to be converted
 * @return The keycode
 */
xcb_keycode_t *getKeycode(xcb_keysym_t keysym){
	// Creates mapping table for keysyms
	xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(dpy);
	// convert keycode if mapping table exists
	xcb_keycode_t *keycode = (keysyms ? xcb_key_symbols_get_keycode(keysyms, keysym) : NULL);
	// mapping table needs to be freed since we allocated it
	xcb_key_symbols_free(keysyms);
	return keycode;
}
