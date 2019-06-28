#ifndef COROUTINE_h
#define COROUTINE_h

#include<ucontext.h>
#define DEFAULT_STACK_SIZE 1024
#define DEFAULT_COROUTINE_NUM 1024

enum CoroutineState{FREE,RUNNING,RUNNABLE,SUSPEND};

struct coroutine{
    ucontext_t context_c;
    void (*fun)(void *arg);
    enum CoroutineState state = FREE;
    char stack[DEFAULT_STACK_SIZE];
};

struct schedule{
    ucontext_t context_main;
    int running_coroutine;
    coroutine *coroutines;

    schedule():running_coroutine(-1){
        coroutines = new coroutine[DEFAULT_COROUTINE_NUM];
    }
};

//开启协程
int create_coroutine(schedule &s,void (*fun)(void *arg));

void run(schedule *s);

//挂起
int yield(schedule &s);

//恢复运行
void resume(schedule &s,int id);

//检查是否全部结束
bool allFinish(const schedule &s);
#endif
