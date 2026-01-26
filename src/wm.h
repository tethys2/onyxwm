#ifndef WM_H
#define WM_H
#include <xcb/xcb_keysyms.h>

/**
 * @brief Spawn a program by forking and then becoming the program
 *
 * @param program A list of arguments starting with the program name/location and ending in NULL
 */
void spawn(char **program);
/**
 * @brief Clean up the program and exit
 *
 * @param ret What to exit with: 0 for success, -1 for failure
 */
void exitWM(int ret);
/**
 * @brief Spawn all programs in autostart_list from config/autostart.h
 */
void autostart(void);
/**
 * @brief Give a window input focus if not root or undefined
 *
 * @param win The window to give focus
 */
void focusInput(xcb_drawable_t win);
/**
 * @brief Raise a window by setting its XCB_CONFIG_WINDOW_STACK_MODE to XCB_STACK_MODE_ABOVE
 *
 * @param win The window to be raised
 */
void raiseWin(xcb_drawable_t win);
/**
 * @brief Convert an xcb_keycode_t into an xcb_keysym_t by creating and destroying a key symbols table
 *
 * @param keycode The keycode to be converted
 * @return The keysym
 */
xcb_keysym_t getKeysym(xcb_keycode_t keycode);
/**
 * @brief Convert an xcb_keysym_t into an xcb_keycode_t by creating and destroying a key symbols table
 *
 * @param keysym The keysymbol to be converted
 * @return The keycode
 */
xcb_keycode_t *getKeycode(xcb_keysym_t keysym);

#endif

