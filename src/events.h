// events.h
#ifndef EVENTS_H
#define EVENTS_H

#include <xcb/xcb.h>

void handleEvent(xcb_generic_event_t *ev);
xcb_keycode_t *getKeycode(xcb_keysym_t keysym);

#endif
