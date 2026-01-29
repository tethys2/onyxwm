/**
 * @file events.c
 * @brief Handles events
 */

#include <xcb/xcb.h>
#include <xcb/xproto.h>   // X protocol types and constants (MapRequest, KeyPress, etc.)
#include <xcb/xcb_event.h> // Event helpers

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
static void handleButtonPress(xcb_generic_event_t *ev);
static void handleButtonRelease(xcb_generic_event_t *ev);
static void handleMotionNotify(xcb_generic_event_t *ev);
static void handleEnterNotify(xcb_generic_event_t *ev);

extern xcb_connection_t *dpy;
extern xcb_screen_t *scre;
extern xcb_drawable_t foc_win;
// needed to keep track of last button pressed for window movement
xcb_button_t last_button_pressed;
//define an array of event handlers; the xcb event code has a corresponding entry for the function
static event_handler_t event_handlers[HANDLER_COUNT] = {
	[XCB_KEY_PRESS]      = handleKeyPress,
	[XCB_MAP_REQUEST]    = handleMapRequest,
	[XCB_MAP_NOTIFY]     = handleMapNotify,
	[XCB_BUTTON_PRESS]   = handleButtonPress,
	[XCB_BUTTON_RELEASE] = handleButtonRelease,
	[XCB_MOTION_NOTIFY]  = handleMotionNotify,
	[XCB_ENTER_NOTIFY]   = handleEnterNotify,
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
	foc_win = e->window;
	raiseWin(e->window);
	focusInput(foc_win);
	xcb_flush(dpy);
}

/**
 * @brief Focuses window and sets it up for movement
 *
 * @param ev A button press event
 */
static void handleButtonPress(xcb_generic_event_t *ev){
	xcb_button_press_event_t *e = (xcb_button_press_event_t *) ev;
	// redundant, but would cause weird behavior if not synced
	foc_win = e->child;
	raiseWin(e->child);
	// pointer needs to be grabbed for window movement
	xcb_grab_pointer(dpy,0, scre->root, XCB_EVENT_MASK_BUTTON_RELEASE
		  | XCB_EVENT_MASK_BUTTON_MOTION | XCB_EVENT_MASK_POINTER_MOTION_HINT,
		  XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC,
		  scre->root, XCB_NONE, XCB_CURRENT_TIME);
	last_button_pressed = e->detail;
	xcb_flush(dpy);
}

/**
 * @brief Ungrabs mouse after release
 *
 * @param ev Unused
 */
static void handleButtonRelease(xcb_generic_event_t *ev){
	(void)ev;
	// force window move to stop working
	last_button_pressed = 0;
	//standard ungrab
	xcb_ungrab_pointer(dpy, XCB_CURRENT_TIME);
	xcb_flush(dpy);
}

/**
 * @brief Move and resize windows
 *
 * @param ev A motion notify event
 */
static void handleMotionNotify(xcb_generic_event_t *ev){
	(void)ev;
	// needed to get pointer coordinates
	// send request for pointer
	xcb_query_pointer_cookie_t cookie = xcb_query_pointer(dpy, scre->root);
	// receive request for pointer
	xcb_query_pointer_reply_t *pointer = xcb_query_pointer_reply(dpy, cookie, 0);
	// left mouse button
	if((last_button_pressed == (xcb_button_t)(1)) && (foc_win != 0)){
		// store x, y coords
		uint32_t geometry_buf[2] = {pointer->root_x, pointer->root_y};
		// update window coordinates
		xcb_configure_window(dpy, foc_win, XCB_CONFIG_WINDOW_X
			      | XCB_CONFIG_WINDOW_Y, geometry_buf);
	}
	xcb_flush(dpy);
}

/**
 * @brief Focuses entered window
 *
 * @param ev An enter notify event
 */
static void handleEnterNotify(xcb_generic_event_t *ev){
	xcb_enter_notify_event_t *e = (xcb_enter_notify_event_t *) ev;
	foc_win = e->event;
	focusInput(foc_win);
	xcb_flush(dpy);
}
