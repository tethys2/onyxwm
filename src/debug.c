#include "debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

static FILE *LOG_FILE = NULL;

bool open_log(){
  if(LOG_FILE != (FILE*)NULL){
    return true;
  }
  int home_dir = open(getenv("HOME"),O_DIRECTORY |O_RDONLY);
  if(openat(home_dir,".local/share", O_RDONLY| O_DIRECTORY) < 0){
    fprintf(stderr,"~/.local/share does not exist or can't be read, can't open log file\n");
    return false;
  }
  if(openat(home_dir,".local/share/onyxwm", O_RDONLY| O_DIRECTORY) < 0){
    printf("~/.local/share/onyxwm does not exist, creating\n");
    if(mkdirat(home_dir,".local/share/onyxwm", 0777) < 0){
      fprintf(stderr,"Couldn't create ~/.local/share/onyxwm\n");	
      return false;
    }
  }
  char log_path[99] = "";
  strcpy(log_path, getenv("HOME"));
  strcat(log_path,"/.local/share/onyxwm/onyxwm.log");
  LOG_FILE = fopen(log_path,"w");
  return LOG_FILE != NULL;
};
void flog(char *type, const char *fmt,va_list args){
  if(!open_log()) return;
  char msg[42];
  strftime(msg, sizeof(msg), "%T", localtime(&(time_t){time(NULL)}));
  fprintf(LOG_FILE,"[%s]", msg);
  fprintf(LOG_FILE, " %s ", type);
  vfprintf(LOG_FILE,fmt , args);
}

void log_msg(const char *fmt, ...){
  va_list args;
  va_start(args);
  printf(fmt, args);
  va_end(args);
  va_start( args);
  flog("MSG", fmt, args);
  va_end(args);
}
void log_err(const char *fmt, ...){
  va_list args;
  va_start( args);
  fprintf(stderr, fmt,args);
  va_end(args);
  va_start(args);
  flog("ERR", fmt, args);
  va_end(args);
}
