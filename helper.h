#ifndef _HELPER_H_
#define _HELPER_H_
#include <sys/inotify.h>
void displayInotifyEvent(struct inotify_event *i);
int playSound(char *path);
#endif
