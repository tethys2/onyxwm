// events.h
#ifndef EVENTS_H
#define EVENTS_H

#include <xcb/xcb.h>

/**
 * @brief Calls the corresponding handler in event_handlers
 *
 * @param ev The generic event to be handled
 */
void handleEvent(xcb_generic_event_t *ev);

#endif
