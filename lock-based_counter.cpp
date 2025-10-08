#include <iostream>
#include <thread>


using namespace std;

int counter = 0;
mutex mymutex;

void increment(){
    lock_guard<mutex> lock(mymutex); // for RAII principles ensures unlocking mymutex in case critical section exits unexpectedly
    counter++;
}

void decrement(){
    mymutex.lock();
    counter--;
    mymutex.unlock();
}

int getvalue(){
    unique_lock<mutex> lock(mymutex); // in addition to lock_guard, unique_lock can be unlocked on our wish and ownership can be transferred
    return counter;
}

int main(){
    return 0;
}