#ifndef WM_H
#define WM_H
#include <xcb/xcb_keysyms.h>

void spawn(char **program);
void exitWM(int ret);
void autostart(void);
void focusInput(xcb_drawable_t win);
void raiseWin(xcb_drawable_t win);
xcb_keysym_t getKeysym(xcb_keycode_t keycode);
xcb_keycode_t *getKeycode(xcb_keysym_t keysym);

#endif

