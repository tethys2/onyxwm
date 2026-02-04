#pragma once

/**
 * @brief Logs a message to stdout and log file
 *
 * Works like printf: arbitrary substitution arguments
 *
 * @param msg The message to log
 * @param ... Substitutions
 */
void logMessage(const char *msg, ...);
/**
 * @brief Logs a message to stderr and log file
 *
 * Works like printf: arbitrary substitution arguments
 *
 * @param msg The message to log
 * @param ... Substitutions
 */
void logError(const char *msg, ...);
