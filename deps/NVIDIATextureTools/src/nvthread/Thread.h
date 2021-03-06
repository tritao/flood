// This code is in the public domain -- castano@gmail.com

#pragma once
#ifndef NV_THREAD_THREAD_H
#define NV_THREAD_THREAD_H

#include "nvthread.h"

#include "nvcore/Ptr.h"

namespace nv
{
    typedef void ThreadFunc(void * arg);

    class NVTHREAD_CLASS Thread
    {
        NV_FORBID_COPY(Thread);
    public:
        Thread();
        ~Thread();

        void start(ThreadFunc * func, void * arg);
        void wait();

        bool isRunning() const;

        static void spinWait(uint count);
        static void yield();
        static void sleep(uint ms);

        static void wait(Thread * threads, uint count);

    private:

        struct Private;
        AutoPtr<Private> p;

    public: // @@ Why public? Also in private?!
        ThreadFunc * func;
        void * arg;

    };

} // nv namespace

#endif // NV_THREAD_THREAD_H
