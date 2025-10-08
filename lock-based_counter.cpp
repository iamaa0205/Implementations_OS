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

// can be scaled with approximate counter, taking one global lock,global counter, local lock and counter for each CPU. Now every thread running on CPU acquires corresponding lock and increments corresponding counter until specific threshold. Then it increments it in global counter setting local to 0 again.
// At any time maximum erorr possible in global counter is no. of CPU* threshold
// can get exact if all locks acquired in order but is not scalable

