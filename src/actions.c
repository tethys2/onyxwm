/**
 * @file actions.c
 * @brief Wrapper functions for keybinding
 */

#include "xcb/xcb.h"

#include "wm.h"
#include "actions.h"

extern xcb_connection_t *dpy;
extern xcb_drawable_t foc_win;

void quit(const arg_t *arg){
	// stop compiler from complaining about not using argument
	(void)arg;
	//exit without error
	exitWM(0);
}

void spawnAct(const arg_t *arg){
	// typecast void pointer and send to spawn
	spawn((char **)arg->v);
}

void killWindow(const arg_t *arg){
	// unused
	(void)arg;
	// kill focused window
	xcb_kill_client(dpy, foc_win);
}
