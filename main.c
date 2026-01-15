#include <xcb/xcb.h>      // XCB core header
#include <xcb/xcb_keysyms.h> // XCB helper for key symbol handling (optional here)
#include <xcb/xproto.h>   // X protocol types and constants (MapRequest, KeyPress, etc.)
#include <xcb/xcb_event.h> // Event helpers
#include <stdio.h>        // printf, fprintf
#include <stdlib.h>       // exit, free
#include <unistd.h>       // fork, execlp
#include <sys/types.h>    // pid_t

//definitions
//length for the event_handlers array
#define HANDLER_COUNT 128
//define event handler function type
typedef void (*event_handler_t)(xcb_generic_event_t *);

//function declaration
static void spawn(char **program);
static void handleKeyPress(xcb_generic_event_t *ev);
static void handleMapRequest(xcb_generic_event_t *ev);
static int eventHandler(void);
static void exitWM(int ret);

//connection to xcb
static xcb_connection_t *dpy;
//xcb screen
static xcb_screen_t *scre;

//define an array of event handlers; the xcb event code has a corresponding entry for the function
static event_handler_t event_handlers[HANDLER_COUNT] = {
	[XCB_KEY_PRESS]      = handleKeyPress,
	[XCB_MAP_REQUEST]   = handleMapRequest,
};


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
//handle key press event idk
void handleKeyPress(xcb_generic_event_t *ev){
	//todo: fix chatgpt code with something actually good for key handling
	// Emergency exit: Ctrl+Alt+Escape
	xcb_key_press_event_t *kp = (xcb_key_press_event_t *)ev; // Cast to key press event
	if ((kp->state & (XCB_MOD_MASK_CONTROL | XCB_MOD_MASK_1)) && kp->detail == 9) { // Check modifiers and Escape key
	    exitWM(0);
	}
}
//Handle a windows request to be mapped to the screen
void handleMapRequest(xcb_generic_event_t *ev){
	// Cast the generic event to a map request event
	xcb_map_request_event_t *e = (xcb_map_request_event_t *)ev;
	// Map (show) the requested window
	xcb_map_window(dpy, e->window);
	// Flush to make sure the window is displayed
	xcb_flush(dpy);
}
//handle events, duh. It waits for events from xcb, then sends the event to a handler function from event_handlers
static int eventHandler(void){	
	xcb_generic_event_t *ev;
	while ((ev = xcb_wait_for_event(dpy))) {
		//get response type and get rid of extra bit
		uint8_t type = ev->response_type & ~0x80;
		//make sure event type is within bounds for event handler array, prevent evil memory leak demons
		if(event_handlers[type] && type < HANDLER_COUNT) {
			event_handlers[type](ev);
		}

		else{
			fprintf(stderr, "Unknown event type: %u\n", ev->response_type);
		}

		// Free the event memory after handling it
		free(ev);
	}
	return 0;
}
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
	ret = eventHandler();
	//exit after event loop is complete
	exitWM(ret);
}

