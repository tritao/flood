/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Event.h"
#include "Core/Pointers.h"
#include <vector>

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

/**
 * A thread is the entity within a process that can be scheduled for
 * execution. All threads of a process share its virtual address space
 * and system resources.
 */

enum class ThreadPriority
{
	Low,
	Normal,
	High
};

struct Thread;
typedef Delegate2<Thread*, void*> ThreadFunction;

struct API_CORE Thread
{
	void* Handle;
	volatile bool IsRunning;
	ThreadPriority Priority;
	ThreadFunction Function;
	void* Userdata;
};

API_CORE Thread* ThreadCreate(Allocator*);
API_CORE void    ThreadDestroy(Thread*);
API_CORE bool    ThreadStart(Thread*, ThreadFunction, void*);
API_CORE bool    ThreadJoin(Thread*);
API_CORE bool    ThreadPause(Thread*);
API_CORE bool    ThreadResume(Thread*);
API_CORE bool    ThreadSetPriority(Thread*, ThreadPriority);
API_CORE void    ThreadSetName(Thread*, const char* name);

typedef scoped_ptr<Thread, ThreadDestroy> ThreadPtr;
#define pThreadCreate(alloc, ...) CreateScopedPtr(ThreadCreate, alloc, \
    __VA_ARGS__)

//-----------------------------------//

/**
 * A mutex is used in concurrent programming to avoid the simultaneous
 * use of a common resource, such as a global variable, by pieces of 
 * computer code called critical sections.
 */

struct API_CORE Mutex;

API_CORE Mutex* MutexCreate(Allocator*);
API_CORE void   MutexDestroy(Mutex*);
API_CORE void   MutexInit(Mutex*);
API_CORE void   MutexLock(Mutex*);
API_CORE void   MutexUnlock(Mutex*);

typedef scoped_ptr<Mutex, MutexDestroy> MutexPtr;
#define pMutexCreate(alloc, ...) CreateScopedPtr(MutexCreate, alloc, \
    __VA_ARGS__)

//-----------------------------------//

struct API_CORE Condition;

API_CORE Condition* ConditionCreate(Allocator*);
API_CORE void       ConditionDestroy(Condition*);
API_CORE void       ConditionInit(Condition*);
API_CORE void       ConditionWait(Condition*, Mutex*);
API_CORE void       ConditionWakeOne(Condition*);
API_CORE void       ConditionWakeAll(Condition*);

typedef scoped_ptr<Condition, ConditionDestroy> ConditionPtr;
#define pConditionCreate(alloc, ...) CreateScopedPtr(ConditionCreate, \
    alloc, __VA_ARGS__)

//-----------------------------------//

/**
 * Synchronizes access to variables that are shared by multiple threads.
 * Operations on these variables are performed atomically.
 * Remarks: On Windows this should be aligned to 32-bits.
 */

typedef volatile int32 Atomic;

API_CORE int32 AtomicRead(volatile Atomic* atomic);
API_CORE int32 AtomicWrite(volatile Atomic* atomic, int32 value);
API_CORE int32 AtomicAdd(volatile Atomic* atomic, int32 value);
API_CORE int32 AtomicIncrement(volatile Atomic* atomic);
API_CORE int32 AtomicDecrement(volatile Atomic* atomic);

//-----------------------------------//

/**
 * Tasks provide an higher level interface to concurrency than threads.
 * They can be managed by the engine and grouped in different hardware
 * threads.
 */

struct Task;
typedef Delegate1<Task*> TaskFunction;

struct API_CORE Task
{
	int16 group;
	int16 priority;
	TaskFunction callback;
	void* userdata;
};

API_CORE Task* TaskCreate(Allocator*);
API_CORE void  TaskDestroy(Task*);
API_CORE void  TaskRun(Task*);

typedef scoped_ptr<Task, TaskDestroy> TaskPtr;
#define pTaskCreate(alloc, ...) CreateScopedPtr(TaskCreate, alloc, \
    __VA_ARGS__)

enum class TaskState
{
	Added,
	Started,
	Finished
};

struct API_CORE TaskEvent
{
	Task* task;
	TaskState state;
};

//-----------------------------------//

NAMESPACE_EXTERN_END

// Workaround for template cyclic dependency.
#include "Core/ConcurrentQueue.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

struct API_CORE TaskPool
{
	std::vector<Thread*> Threads;
	ConcurrentQueue<Task*> Tasks;
	ConcurrentQueue<TaskEvent> Events;
	Event1<TaskEvent> OnTaskEvent;
	bool IsStopping;
};

API_CORE TaskPool*  TaskPoolCreate(Allocator*, int8 Size);
API_CORE void       TaskPoolDestroy(TaskPool*);
API_CORE void       TaskPoolAdd(TaskPool*, Task*, uint8 Priority);
API_CORE void       TaskPoolUpdate(TaskPool*);

typedef scoped_ptr<TaskPool, TaskPoolDestroy> TaskPoolPtr;
#define pTaskPoolCreate(alloc, ...) CreateScopedPtr(TaskPoolCreate, \
    alloc, __VA_ARGS__)

//-----------------------------------//

NAMESPACE_EXTERN_END