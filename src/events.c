#include <xcb/xcb_keysyms.h> // XCB helper for key symbol handling (optional here)
#include <xcb/xproto.h>   // X protocol types and constants (MapRequest, KeyPress, etc.)
#include <xcb/xcb_event.h> // Event helpers
#include <stdio.h>        // printf, fprintf

#include "events.h"
#include "wm.h"

//definitions
//length for the event_handlers array
#define HANDLER_COUNT 128
//define event handler function type
typedef void (*event_handler_t)(xcb_generic_event_t *);

//function declaration
static void handleKeyPress(xcb_generic_event_t *ev);
static void handleMapRequest(xcb_generic_event_t *ev);


// declared in main.c, needed to do xcb stuff
extern xcb_connection_t *dpy;
//define an array of event handlers; the xcb event code has a corresponding entry for the function
static event_handler_t event_handlers[HANDLER_COUNT] = {
	[XCB_KEY_PRESS]      = handleKeyPress,
	[XCB_MAP_REQUEST]   = handleMapRequest,
};

// Takes in an event and sends the event to a handler function from event_handlers
void handleEvent(xcb_generic_event_t *ev){
	//get response type and get rid of extra bit
	uint8_t type = ev->response_type & ~0x80;
	//check that the handler function exists and make sure event type is within bounds for event handler array, prevent evil memory leak demons
	if(event_handlers[type] && type < HANDLER_COUNT) {
		event_handlers[type](ev);
	}

	else{
		fprintf(stderr, "Unknown event type: %u\n", ev->response_type);
	}
}

//handle key press event idk
static void handleKeyPress(xcb_generic_event_t *ev){
	//todo: fix chatgpt code with something actually good for key handling
	// Emergency exit: Ctrl+Alt+Escape
	xcb_key_press_event_t *kp = (xcb_key_press_event_t *)ev; // Cast to key press event
	if ((kp->state & (XCB_MOD_MASK_CONTROL | XCB_MOD_MASK_1)) && kp->detail == 9) { // Check modifiers and Escape key
	    exitWM(0);
	}
}

//Handle a windows request to be mapped to the screen
static void handleMapRequest(xcb_generic_event_t *ev){
	// Cast the generic event to a map request event
	xcb_map_request_event_t *e = (xcb_map_request_event_t *)ev;
	// Map (show) the requested window
	xcb_map_window(dpy, e->window);
	// Flush to make sure the window is displayed
	xcb_flush(dpy);
}

