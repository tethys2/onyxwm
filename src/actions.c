// Functions meant for keybinding

#include "wm.h"
#include "actions.h"

// wrapper function for exitWM
void quit(arg_t arg){
	// stop compiler from complaining about not using argument
	(void)arg;
	//exit without error
	exitWM(0);
}
