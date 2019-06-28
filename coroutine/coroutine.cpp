#ifndef COROUTINE_CPP
#define COROUTINE_CPP

#include "coroutine.h"

int create_coroutine(schedule &s,void (*fun)(void *arg)){
    //寻找一个空闲的协程
    int id = 0;
    for(id = 0;id<DEFAULT_COROUTINE_NUM;++id){
        if(s.coroutines[id].state==FREE){
            break;
        }
    }
    //寻找失败返回-1
    if(id==DEFAULT_COROUTINE_NUM){
        return -1;
    }

    //切换
    coroutine *c = &(s.coroutines[id]);
    c->state = RUNNING;
    c->fun = fun;

    getcontext(&(c->context_c));

    c->context_c.uc_stack.ss_sp = c->stack;
    c->context_c.uc_stack.ss_size = DEFAULT_STACK_SIZE;
    c->context_c.uc_stack.ss_flags = 0;
    c->context_c.uc_link = &(s.context_main);
    s.running_coroutine = id;

    makecontext(&(c->context_c),(void (*)())run,1,&s);
    swapcontext(&(s.context_main), &(c->context_c));

    return id;
}

void run(schedule *s)
{
    int id = s->running_coroutine;

    if(id != -1){
        coroutine *t = &(s->coroutines[id]);
        void *a;
        t->fun(a);
        t->state = FREE;
        s->running_coroutine = -1;
    }
}


int yield(schedule &s){
    if(s.running_coroutine!=-1){
        coroutine *c = &(s.coroutines[s.running_coroutine]);
        c->state = SUSPEND;
        s.running_coroutine = -1;
        swapcontext(&(c->context_c),&(s.context_main));
    }
}

void resume(schedule &s,int id){
    coroutine *c = &(s.coroutines[id]);
    if(c->state==SUSPEND)
    {
        c->state = RUNNING;
        swapcontext((&s.context_main),&(c->context_c));
    }
}

bool allFinish(const schedule &s){
    for(int i=0;i<DEFAULT_COROUTINE_NUM;i++){
        if(s.coroutines[i].state!=FREE){
            return false;
        }
    }
    return true;
}

#endif