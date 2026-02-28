//
//  ParentGuard.cpp
//  ParentGuard
//
//  Created by Alvin on 28.02.26.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/event.h>
#include <signal.h>

static void *parent_watch_thread(void *arg)
{
    pid_t ppid = getppid();
    printf("Child PID=%d watching parent PID=%d\n", getpid(), ppid);
    if (ppid == 1) {
        printf("Parent is launchd. Either process started as daemon or parent is already dead. Quiting!\n");
        _exit(0);
    }

    int kq = kqueue();
    if (kq == -1)
    {
        perror("kqueue");
        return NULL;
    }

    struct kevent ev;
    EV_SET(&ev, ppid, EVFILT_PROC, EV_ADD | EV_ENABLE, NOTE_EXIT, 0, NULL);

    if (kevent(kq, &ev, 1, NULL, 0, NULL) == -1)
    {
        perror("kevent register");
        return NULL;
    }

    printf("Parent watcher running in background...\n");

    while (1)
    {
        struct kevent event;
        int nev = kevent(kq, NULL, 0, &event, 1, NULL);

        if (nev > 0 && (event.fflags & NOTE_EXIT))
        {
            printf("Parent exited. Child exiting.\n");
            _exit(0);
        }
    }

    return NULL;
}

__attribute__((constructor))
static void library_init(void)
{
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (pthread_create(&tid, &attr, parent_watch_thread, NULL) != 0)
    {
        perror("pthread_create");
    }

    pthread_attr_destroy(&attr);
}
