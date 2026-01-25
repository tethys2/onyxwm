/**
 * @file events.c
 * @brief Handles events
 */

#include <xcb/xproto.h>   // X protocol types and constants (MapRequest, KeyPress, etc.)
#include <xcb/xcb_event.h> // Event helpers
#include <stdio.h>        // printf, fprintf

#include "events.h"
#include "debug.h"
#include "wm.h"

//block needed for config
#include <X11/keysym.h>
#include "types.h"
#include "actions.h"
#include "config/keybindings.h"

//definitions
//length for the event_handlers array
#define HANDLER_COUNT 128
//define event handler function type
typedef void (*event_handler_t)(xcb_generic_event_t *);

//function declaration
static void handleKeyPress(xcb_generic_event_t *ev);
static void handleMapRequest(xcb_generic_event_t *ev);
static void handleMapNotify(xcb_generic_event_t *ev);

// declared in main.c, needed to do xcb stuff
extern xcb_connection_t *dpy;
// needed to handle window movement, focus
uint32_t geometry_buf[5];
//define an array of event handlers; the xcb event code has a corresponding entry for the function
static event_handler_t event_handlers[HANDLER_COUNT] = {
	[XCB_KEY_PRESS]      = handleKeyPress,
	[XCB_MAP_REQUEST]    = handleMapRequest,
	[XCB_MAP_NOTIFY]     = handleMapNotify,
};

void handleEvent(xcb_generic_event_t *ev){
	//get response type and get rid of extra bit
	uint8_t type = ev->response_type & ~0x80;
	//check that the handler function exists and make sure event type is within bounds for event handler array, prevent evil memory leak demons
	if(event_handlers[type] && type < HANDLER_COUNT) {
		event_handlers[type](ev);
	}

	else{
		logError("Unknown event type: %u\n", 1, ev->response_type);
	}
}

/**
 * @brief Calls corresponding keybind in keys
 *
 * @param ev A keypress event
 */
static void handleKeyPress(xcb_generic_event_t *ev){
	//typcast event
	xcb_key_press_event_t *e = (xcb_key_press_event_t *)ev;
	// convert event keycode to keysym
	xcb_keysym_t keysym = getKeysym(e->detail);
	//get length of keybinding table
	int keys_length = sizeof(keys) / sizeof(keys[0]);
	for(int i = 0; i < keys_length; i++){
		if((keys[i].keysym == keysym) && (keys[i].mod == e->state)){
			keys[i].func(&(keys[i].arg));
		}
	}
}

/**
 * @brief Maps a window to the screen
 *
 * @param ev A map request event
 */
static void handleMapRequest(xcb_generic_event_t *ev){
	// Cast the generic event to a map request event
	xcb_map_request_event_t *e = (xcb_map_request_event_t *)ev;
	// Map (show) the requested window
	xcb_map_window(dpy, e->window);
	// Flush to make sure the window is displayed
	xcb_flush(dpy);
}
/**
 * @brief Give newly mapped windows focus
 *
 * @param ev A map notify event
 */
static void handleMapNotify(xcb_generic_event_t *ev){
	// cast the generic event to a map notify event
	xcb_map_notify_event_t *e = (xcb_map_notify_event_t *)ev;
	// focus the window which notified
	raiseWin(e-> window);
	focusInput(e-> window);
	xcb_flush(dpy);
}
