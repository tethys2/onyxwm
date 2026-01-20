#ifndef WM_H
#define WM_H

void spawn(char **program);
void exitWM(int ret);
void autostart(void);
xcb_keysym_t getKeysym(xcb_keycode_t keycode);
xcb_keycode_t *getKeycode(xcb_keysym_t keysym);

#endif

