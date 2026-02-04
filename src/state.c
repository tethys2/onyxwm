/**
 * @file state.c
 * @brief stores program wide variables
 *
 * if deciding which file to put a global is a headache just put it here
 */

#include <xcb/xcb.h>
#include <xcb/xcb_cursor.h>
/** connection to the x server */
xcb_connection_t *dpy;
/** the xcb screen */
xcb_screen_t *scre;
/** current focused window, might not be accurate */
xcb_drawable_t foc_win;
/** current cursor, visual */
xcb_cursor_t cursor;
/** needed to set cursor */
xcb_cursor_context_t *cursor_context;
