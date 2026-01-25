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
 * @retval 1 program should quit with status 0 (--help, --version, etc.)
 * @retval -1 fatal error (bad syntax or unknown argument)
 */
int parseArguments(int argc, char *argv[]);

/* vim:set ft=c:set syn=c.doxygen: */
