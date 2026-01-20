// Functions meant for keybinding
#include "wm.h"
#include "actions.h"

// wrapper function for exitWM
void quit(const arg_t *arg){
	// stop compiler from complaining about not using argument
	(void)arg;
	//exit without error
	exitWM(0);
}

//wrapper for spawn
void spawnAct(const arg_t *arg){
	// typecast void pointer and send to spawn
	spawn((char **)arg->v);
}
