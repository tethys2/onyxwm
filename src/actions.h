// actions.h
#ifndef ACTIONS_H
#define ACTIONS_H

#include "types.h"

/**
 * @brief Wrapper for exitWM
 *
 * @param arg Unused
 */
void quit(const arg_t *arg);
/**
 * @brief Wrapper for spawn
 *
 * @param arg Argument list for program starting with name and ending in NULL
 */
void spawnAct(const arg_t *arg);
/**
 * @brief Kills the currently focused window
 *
 * focused window is stored in foc_win
 *
 * @param arg Unused
 */
void killWindow(const arg_t *arg);

#endif
