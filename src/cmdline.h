/**
 * @file cmdline.h
 * @brief command line handling
 */
#pragma once

/**
 * @brief parses arguments
 *
 * Meant to be used to parse the args passed to main, but could be used for
 * other purposes
 *
 * @param argc argument count
 * @param argv array of argument strings
 *
 * @returns result of parsing the arguments
 * @retval 0 success
 * @retval 1 non-fatal error (unknown argument)
 * @retval -1 fatal error (bad syntax)
 */
int parseArguments(int argc, char *argv[]);

/* vim:set ft=c: */
