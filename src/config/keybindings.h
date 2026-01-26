#include "../types.h"

// Main modkey, default is super key
#define MOD XCB_MOD_MASK_4
#define SHIFT XCB_MOD_MASK_SHIFT
#define CTRL XCB_MOD_MASK_CONTROL

static const char *launcher_cmd[] = {"j4-dmenu-desktop", NULL};
static const char *term_cmd[] = {"st", NULL};

// keybinding table, intended to modified
// modifier key, key, function, argument
static const keybinding_t keys[] = {
	{MOD,        XK_r,         spawnAct,        {.v = launcher_cmd}},
	{MOD,        XK_Return,    spawnAct,        {.v = term_cmd}},
	{MOD|SHIFT,  XK_Escape,        quit,        {0}},
	{MOD|SHIFT,  XK_c,         killWindow,      {0}},
};
