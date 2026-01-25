#pragma once

/**
 * @brief Logs a message to stdout and log file
 *
 * Works like printf: arbitrary substitution arguments
 *
 * @param msg The message to log
 * @param n The number of arguments
 * @param ... Substitutions
 */
void logMessage(const char *msg, int n, ...);
/**
 * @brief Logs a message to stderr and log file
 *
 * Works like printf: arbitrary substitution arguments
 *
 * @param msg The message to log
 * @param n The number of arguments
 * @param ... Substitutions
 */
void logError(const char *msg, int n, ...);
