/* thread.h
 * - Thread Abstraction Function Headers
 *
 * Copyright (c) 1999, 2000 the icecast team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

/* renamed from thread_t due to conflict on OS X */

typedef struct thread_tag {
	/* the local id for the thread, and it's name */
	long thread_id;
	char *name;

	/* the time the thread was created */
	time_t create_time;
	
	/* the file and line which created this thread */
	char *file;
	int line;

    /* is the thread running detached? */
    int detached;

	/* the system specific thread */
	pthread_t sys_thread;
} thread_type;

typedef struct mutex_tag {
#ifdef DEBUG_MUTEXES
	/* the local id and name of the mutex */
	long mutex_id;
	char *name;

	/* the thread which is currently locking this mutex */
	long thread_id;

	/* the file and line where the mutex was locked */
	char *file;
	int line;	

#endif

	/* the system specific mutex */
	pthread_mutex_t sys_mutex;
} mutex_t;

typedef struct cond_tag {
#ifdef THREAD_DEBUG
	long cond_id;
	char *name;
#endif

	pthread_mutex_t cond_mutex;
	pthread_cond_t sys_cond;
} cond_t;

typedef struct rwlock_tag {
#ifdef THREAD_DEBUG
	long rwlock_id;
	char *name;

	/* information on which thread and where in the code
	** this rwlock was write locked
	*/
	long thread_id;
	char *file;
	int line;
#endif

	pthread_rwlock_t sys_rwlock;
} rwlock_t;

#define thread_create(n,x,y,z) thread_create_c(n,x,y,z,__LINE__,__FILE__)
#define thread_mutex_create(x) thread_mutex_create_c(x,__LINE__,__FILE__)
#define thread_mutex_lock(x) thread_mutex_lock_c(x,__LINE__,__FILE__)
#define thread_mutex_unlock(x) thread_mutex_unlock_c(x,__LINE__,__FILE__)
#define thread_cond_create(x) thread_cond_create_c(x,__LINE__,__FILE__)
#define thread_cond_signal(x) thread_cond_signal_c(x,__LINE__,__FILE__)
#define thread_cond_broadcast(x) thread_cond_broadcast_c(x,__LINE__,__FILE__)
#define thread_cond_wait(x) thread_cond_wait_c(x,__LINE__,__FILE__)
#define thread_cond_timedwait(x,t) thread_cond_wait_c(x,t,__LINE__,__FILE__)
#define thread_rwlock_create(x) thread_rwlock_create_c(x,__LINE__,__FILE__)
#define thread_rwlock_rlock(x) thread_rwlock_rlock_c(x,__LINE__,__FILE__)
#define thread_rwlock_wlock(x) thread_rwlock_wlock_c(x,__LINE__,__FILE__)
#define thread_rwlock_unlock(x) thread_rwlock_unlock_c(x,__LINE__,__FILE__)
#define thread_exit(x) thread_exit_c(x,__LINE__,__FILE__)

#define MUTEX_STATE_NOTLOCKED -1
#define MUTEX_STATE_NEVERLOCKED -2
#define MUTEX_STATE_UNINIT -3
#define THREAD_DETACHED 1
#define THREAD_ATTACHED 0

/* init/shutdown of the library */
void thread_initialize(void);
void thread_initialize_with_log_id(int log_id);
void thread_shutdown(void);

/* creation, destruction, locking, unlocking, signalling and waiting */
thread_type *thread_create_c(char *name, void *(*start_routine)(void *), 
        void *arg, int detached, int line, char *file);
void thread_mutex_create_c(mutex_t *mutex, int line, char *file);
void thread_mutex_lock_c(mutex_t *mutex, int line, char *file);
void thread_mutex_unlock_c(mutex_t *mutex, int line, char *file);
void thread_mutex_destroy(mutex_t *mutex);
void thread_cond_create_c(cond_t *cond, int line, char *file);
void thread_cond_signal_c(cond_t *cond, int line, char *file);
void thread_cond_broadcast_c(cond_t *cond, int line, char *file);
void thread_cond_wait_c(cond_t *cond, int line, char *file);
void thread_cond_timedwait_c(cond_t *cond, int millis, int line, char *file);
void thread_cond_destroy(cond_t *cond);
void thread_rwlock_create_c(rwlock_t *rwlock, int line, char *file);
void thread_rwlock_rlock_c(rwlock_t *rwlock, int line, char *file);
void thread_rwlock_wlock_c(rwlock_t *rwlock, int line, char *file);
void thread_rwlock_unlock_c(rwlock_t *rwlock, int line, char *file);
void thread_rwlock_destroy(rwlock_t *rwlock);
void thread_exit_c(int val, int line, char *file);

/* sleeping */
void thread_sleep(unsigned long len);

/* for using library functions which aren't threadsafe */
void thread_library_lock(void);
void thread_library_unlock(void);
#define PROTECT_CODE(code) { thread_library_lock(); code; thread_library_unlock(); }

/* thread information functions */
thread_type *thread_self(void);

/* renames current thread */
void thread_rename(const char *name);

/* waits until thread_exit is called for another thread */
void thread_join(thread_type *thread);

#endif  /* __THREAD_H__ */



