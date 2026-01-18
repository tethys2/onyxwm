// types.h
#ifndef TYPES_H
#define TYPES_H

#include <xcb/xcb.h>
// types that reused

// argument for functions in the keybind table
typedef union {
	int i;
	unsigned int ui;
	float f;
	const void *v;
} arg_t;

// keybinding in the keybinding table
typedef struct {
	uint16_t mod; // moddifer key eg. MOD_MASK_SUPER
	xcb_keysym_t keysym; // key pressed
	void (*func)(const arg_t *); // keybinding function
	const arg_t arg; // argument for function
} keybinding_t;

#endif
