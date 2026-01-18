#include "types.h"

#define MOD XCB_MOD_MASK_1

// List of applications to autostart
// List arguments and then end with a null
// increase the number if arguments go over 4
static char *autostart_list[][4] = {
	{"st", NULL},
	{"picom", NULL},
	{"xwallpaper","--zoom", "/home/void/.config/wallpaper", NULL},
	{"xbanish", NULL},
	{NULL} // end list
};

// keybinding table, intended to modified
// modifier key, key, function, argument
static const keybinding_t keys[] = {
	//{MOD,        XK_Escape,        exitWM, {.i = 0}}, 
};
