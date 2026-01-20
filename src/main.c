#include <xcb/xcb.h>      // XCB core header
#include <stdlib.h>       // free

#include "wm.h"
#include "events.h"
#include "debug.h"

//block needed for config
#include <X11/keysym.h>
#include "types.h"
#include "actions.h"
#include "config/keybindings.h"


//connection to xcb
xcb_connection_t *dpy;
//xcb screen
static xcb_screen_t *scre;

int main(void) {
	int ret = 0;
	int screen_number; // Will be set to the screen number we're using
	// Connect to the default display
	dpy = xcb_connect(NULL, &screen_number);
	// Check if the connection has an error
	if (xcb_connection_has_error(dpy)) {
		log_err("Failed to connect to X server\n");
		return 1;
	} 
    
	//get setup info
	const xcb_setup_t *setup = xcb_get_setup(dpy);
	// Iterator over available screens
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
	// Advance the iterator to our desired screen
	for (int i = 0; i < screen_number; i++)
		xcb_screen_next(&iter);
	// Get a pointer to the screen
	scre = iter.data;
	// Get the root window of that screen
	xcb_window_t root = scre->root;



	// We must ask to receive SubstructureRedirect events on the root window.
	// Only one client can do this at a time. If this fails, another WM is running.
	uint32_t event_mask = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
			XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
			XCB_EVENT_MASK_KEY_PRESS;
	// Set the event mask on the root window (checked call so we can verify success)
	xcb_void_cookie_t cookie = xcb_change_window_attributes_checked(
		dpy, root, XCB_CW_EVENT_MASK, &event_mask
	);
	// Check if the request failed (i.e., another WM is running)
	xcb_generic_error_t *err = xcb_request_check(dpy, cookie);
	if (err) {
		log_err("Another window manager is already running\n");
		free(err);
		xcb_disconnect(dpy);
		return 1;
	}
	// grab keybindings
	int keys_length = sizeof(keys) / sizeof(keys[0]);
	for (int i =  0; i < keys_length; i++){
		xcb_keycode_t *keycode = getKeycode(keys[i].keysym);
		if(keycode != NULL){
			xcb_grab_key(dpy, 1, scre->root, keys[i].mod, *keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
		}
	}


	// Flush requests to the X server to ensure they are sent
	xcb_flush(dpy);
	log_msg("onxyWM is running\n");
	
	// start autostart apps
	autostart();

	// Wait for incoming events forever, returns whether it exited normally or from an error
	xcb_generic_event_t *ev;
	// blocks until xcb gives us a new event
	while ((ev = xcb_wait_for_event(dpy))) {
		handleEvent(ev);
		// event needs to freed since xcb gives us ownership
		free(ev);
		// break if xcb connection has an error
		if((ret = xcb_connection_has_error(dpy))){
			break;
		}
	}
	//exit after event loop is complete
	exitWM(ret);
}

