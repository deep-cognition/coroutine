#include<stdio.h>
#include"coroutine.h"

schedule s;

void fun1(void *arg){
    printf("1\n");
    yield(s);
    printf("3\n");
    yield(s);
    printf("5\n");
}

void fun2(void *arg){
    printf("2\n");
    yield(s);
    printf("4\n");
    yield(s);
    printf("6\n");
}

int main(){
    int id1 = create_coroutine(s,fun1);
    int id2 = create_coroutine(s,fun2);
    while(allFinish(s)==0){
        resume(s,id1);
        resume(s,id2);
    }
    return 0;
}