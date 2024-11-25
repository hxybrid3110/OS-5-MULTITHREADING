#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include <pthread.h>

int user_main(int argc, char **argv);

typedef struct {
int L; 
int H;
std::function<void(int)> lambda;
} thread_args;

typedef struct {
int L1,L2; 
int H1,H2;
std::function<void(int,int)> lambda;
} thread_args_2;

void *thread_func(void *ptr) {
  thread_args * t = ((thread_args *) ptr);
  for(int i = t->L;i<t->H;i++){
    t->lambda(i);
  }
  return NULL;
}

void *thread_func_parelell(void *ptr) {
  thread_args_2 * t = ((thread_args_2*) ptr);
  for(int i = t->L1;i<t->H1;i++){
    for(int j = t->L2;j<t->H2;j++){
      t->lambda(i,j);
    }
  }
  return NULL;
}

void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads){
  clock_t starttime = clock();
  int chunk = (high-low)/numThreads;
  pthread_t tid[numThreads];
  thread_args args[numThreads];

  for(int i = 0;i<numThreads;i++){
    args[i].L = low + i*chunk;
    if(i==numThreads-1){
      args[i].H = high;
    }else{
      args[i].H = args[i].L + chunk;
    }
    args[i].lambda = lambda;
    if(pthread_create(&tid[i],NULL,thread_func,(void*) &args[i])){
      std::cout<<"Error in pthread_create.\n";
    }
  }

  for (int i=0; i<numThreads; i++) {
    if(pthread_join(tid[i] , NULL)){
      std::cout<<"Error in pthread_join.\n";
    }
  }
  clock_t endtime = clock();
  std::cout<<"Time taken in Execution of 1D parelell_for : "<< 1000.0*(endtime-starttime)/CLOCKS_PER_SEC<<" ms \n";
}

void parallel_for(int low1, int high1,  int low2, int high2, std::function<void(int, int)> &&lambda, int numThreads){
  clock_t starttime = clock();
  int chunk = (high1-low1)/numThreads;
  pthread_t tid[numThreads];
  thread_args_2 args[numThreads];

  for(int i = 0;i<numThreads;i++){
    args[i].L1 = low1 + i*chunk;
    if(i==numThreads-1){
      args[i].H1 = high1;
    }else{
      args[i].H1 = args[i].L1 + chunk;
    }
    args[i].L2 = low2;
    args[i].H2 = high2;
    args[i].lambda = lambda;
    if(pthread_create(&tid[i],NULL,thread_func_parelell,(void*) &args[i])){
      std::cout<<"Error in pthread_create.\n";
    }
  }

  for (int i=0; i<numThreads; i++) {
    if(pthread_join(tid[i] , NULL)){
      std::cout<<"Error in pthread_join.\n";
    }
  }
  clock_t endtime = clock();
  std::cout<<"Time taken in Execution of 2D parelell_for : "<< 1000.0*(endtime-starttime)/CLOCKS_PER_SEC<<" ms \n";
}


void demonstration(std::function<void()> && lambda) {
  lambda();
}


int main(int argc, char **argv) {
  int x=5,y=1;
  auto  lambda1 = [x,&y](void) {
    y = 5;
    std::cout<<"====== Welcome to Assignment-"<<y<<" of the CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  // Executing the lambda function
  demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

  int rc = user_main(argc, argv);
 
  auto lambda2 = []() {
    std::cout<<"====== Hope you enjoyed CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  demonstration(lambda2);
  return rc;
}

#define main user_main

