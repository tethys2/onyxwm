#include <xcb/xcb.h>      // XCB core header
#include <stdio.h>        // printf, fprintf
#include <stdlib.h>       // exit, free

#include "wm.h"
#include "events.h"

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
		fprintf(stderr, "Failed to connect to X server\n");
		return 1;
	} 
	printf("hi");

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
		fprintf(stderr, "Another window manager is already running\n");
		free(err);
		xcb_disconnect(dpy);
		return 1;
	}
	// Flush requests to the X server to ensure they are sent
	xcb_flush(dpy);
	printf("onxyWM is running");



	spawn((char *[]){"st", NULL});
	spawn((char *[]){"xwallpaper","--zoom", "/home/void/.config/wallpaper", NULL});

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

