#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>	// NAME_MAX
#include "helper.h"

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
#define SUSPEND_FILE "/sys/power/state"
#define LID_FILE "/proc/acpi/button/lid/LID0/state"
#define STARTUP_SOUND "./sounds/startup.wav"
#define SHUTDOWN_SOUND "./sounds/shutdown.wav"
#define MASK IN_MODIFY | IN_CLOSE_WRITE | IN_OPEN

#define OPEN_SLEEP_THRESHOLD 2

typedef enum {AWAKE, SLEEP_INTENDED, SLEEP_BEGIN, SLEEP} sleep_t;

int main() {
	int inotifyFd, wd, lfd;
	char buf[BUF_LEN] __attribute__ ((aligned(4)));
	ssize_t numRead;
	char *p;
	struct inotify_event *event;

	sleep_t STATE = AWAKE;
	int openCount = 0;

	inotifyFd = inotify_init();                 /* Create inotify instance */
	if (inotifyFd < 0) {
		fprintf(stderr, "inotify_init() failed: %d\n", inotifyFd);
		exit(1);
	}

	wd = inotify_add_watch(inotifyFd, SUSPEND_FILE, MASK);

	if (wd < 0) {
		fprintf(stderr, "inotify_add_watch() failed: %d\n", wd);
		exit(1);
	}	

	lfd = inotify_add_watch(inotifyFd, LID_FILE, MASK);

	for (;;) {                                  /* Read events forever */
		numRead = read(inotifyFd, buf, BUF_LEN);
		if (numRead == 0) {
			fprintf(stderr, "read() from inotify fd returned 0!\n");
		}

		if (numRead < 0) {
			fprintf(stderr, "read() from inotify fd returned: %zd\n", numRead);
			exit(1);
		}

		for (p = buf; p < buf + numRead; ) {
			event = (struct inotify_event *) p;

			if (event->wd == wd) {

				if (event->mask & IN_OPEN && STATE == AWAKE) {
					openCount++;
					printf("Open count: %d\n", openCount);
					if (openCount >= OPEN_SLEEP_THRESHOLD) {
						openCount = 0;
						playSound(SHUTDOWN_SOUND);
						STATE = SLEEP_INTENDED;
					}
				}
				if (event->mask & IN_MODIFY && STATE == SLEEP_INTENDED) {
					STATE  = SLEEP_BEGIN;
				}
				if (event->mask & IN_CLOSE_WRITE && STATE == SLEEP_BEGIN) {
					STATE = SLEEP;
				}
				if (STATE == SLEEP) {
					playSound(STARTUP_SOUND);
					STATE = AWAKE;
				}
			}

			else if (event->wd == lfd) {
				system("cat "LID_FILE);
			}

			displayInotifyEvent(event);
			p += sizeof(struct inotify_event) + event->len;	
		}
	}
	exit(EXIT_SUCCESS);
}
