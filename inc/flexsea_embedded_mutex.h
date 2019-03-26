#ifndef __FLEXSEA_MUTEX_
#define __FLEXSEA_MUTEX_


#include <stdint.h>
#if defined(__linux)
#include <pthread.h>
typedef pthread_mutex_t MutexFlag;
#define MUTEX_LOCKED 1
#elif defined(__WIN32) 
#include <pthread.h>
typedef pthread_mutex_t MutexFlag;
#define MUTEX_LOCKED 1
#else
typedef uint8_t MutexFlag;
//typedef volatile pthread_mutex_t MutexFlag;
#define MUTEX_UNLOCKED 0
#endif

__attribute__((always_inline)) static inline uint8_t INIT_MUTEX(MutexFlag* flag)
{
#if defined(__WIN32) || defined(__linux)
	return pthread_mutex_init(flag, NULL);
#else
	return MUTEX_UNLOCKED; // indicates sucess (See man pages)
#endif
}

__attribute__((always_inline)) static inline uint8_t UNLOCK_MUTEX(MutexFlag* flag)
{
#if defined(__WIN32) || defined(__linux) 
	return pthread_mutex_unlock(flag);
#else
	return MUTEX_UNLOCKED;
#endif
}


__attribute__((always_inline)) static inline uint8_t TRY_LOCK_MUTEX(MutexFlag* flag)
{
#if defined(__WIN32) || defined(__linux)
	return pthread_mutex_trylock(flag);
#else
	return MUTEX_LOCKED;
#endif
}

__attribute__((always_inline)) static inline uint8_t LOCK_MUTEX(MutexFlag* flag)
{
#if defined(__WIN32) || defined(__linux)
	return pthread_mutex_lock(flag);
#else
	return MUTEX_LOCKED;
#endif
}
#endif //__FLEXSEA_MUTEX_
