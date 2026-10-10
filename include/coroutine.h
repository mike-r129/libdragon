/**
 * @file coroutine.h
 * @author Max Bebök <beboek.max@gmail.com>
 * @brief Cooperative coroutines API
 * @ingroup lowlevel
 */

#ifndef LIBDRAGON_COROUTINE_H
#define LIBDRAGON_COROUTINE_H

#include "preview.h"
#include "ucontext.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

///@cond
typedef struct coroutine_s coroutine_t;
///@endcond

/**
 * @brief Creates a new coroutine
 * @preview
 * This creates a new execution context for the specified function.
 * After creation, the coroutine can be executed with coro_resume.
 * 
 * @param fn function to use for the coroutine
 * @param arg argument to pass into the function
 * @param stack_size stack size to be allocated
 * @return created coroutine
 */
LIBDRAGON_PREVIEW_API
coroutine_t *coro_create(void (*fn)(void *), void *arg, size_t stack_size);

#if defined(__cplusplus) && defined(__GNUC__) && !defined(__clang__)
// GCC 16 predeclares the C++20 coroutine builtins coro_resume(void*) and
// coro_destroy(void*); our public functions of the same name take a
// coroutine_t*. The user declaration replaces the builtin (intended), but
// -Wbuiltin-declaration-mismatch is an error under -Werror: silence it here.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
#endif
/**
 * @brief Resumes execution
 * @preview
 * 
 * Continues to execute the coroutine until the next yield.
 * If the function reached the end, it will no longer be called.
 * To check if a coroutine is done, use coro_finished.
 * 
 * @param co coroutine to resume
 */
LIBDRAGON_PREVIEW_API
void coro_resume(coroutine_t *co);
#if defined(__cplusplus) && defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

/**
 * @brief Yields and gives back control to the caller context.
 * @preview
 * 
 * This can be used within the context of a coroutine to pause execution.
 * When doing so, it gives back control to the code that called coro_resume.
 */
LIBDRAGON_PREVIEW_API
void coro_yield(void);

/**
 * @brief Puts the current coroutine to sleep.
 * @preview
 * 
 * This function will yield, and prevent execution of the current coroutine until
 * the given time as passed.
 * Note that the time is a minimum, and the actual time depends on how often you
 * try to execute the function via coro_resume.
 * 
 * This function should be preferred over spin-waiting inside the coroutine,
 * since it avoids a context switch alltogether if the time has not passed yet.
 * 
 * @param ticks minimum time to wait in ticks
 */
LIBDRAGON_PREVIEW_API
void coro_sleep(uint64_t ticks);

#if defined(__cplusplus) && defined(__GNUC__) && !defined(__clang__)
// See coro_resume: GCC 16's coro_destroy(void*) builtin.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
#endif
/**
 * @brief Frees all resources associated with a coroutine.
 * @preview
 *
 * After this function is called it is no longer safe to resume execution,
 * or interact with the coroutine in any way.
 * 
 * @param co coroutine to free
 */
LIBDRAGON_PREVIEW_API
void coro_destroy(coroutine_t *co);
#if defined(__cplusplus) && defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

/**
 * @brief Checks if a coroutine has finished execution.
 * @preview
 * 
 * @param co coroutine to check
 * @return true if finished
 */
LIBDRAGON_PREVIEW_API
bool coro_finished(coroutine_t *co);

/**
 * @brief Returns the currently active coroutine
 * @preview
 * 
 * If currently inside a coroutine, it will return the handle.
 * When outside, it returns NULL.
 * 
 * This can also be used to detect and change behaviour of a function
 * depending on if it's running in a coroutine or not.
 * 
 * @return active coroutine, NULL if in main context
 */
LIBDRAGON_PREVIEW_API
coroutine_t* coro_get_current(void);

#ifdef __cplusplus
}
#endif

#endif

