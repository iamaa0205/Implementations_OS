#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

atomic_flag flag = ATOMIC_FLAG_INIT;

void lock(){
    while(flag.test_and_set(memory_order_acquire)==1);
}

void unlock(){
    flag.clear(memory_order_release);
}

int counter=0;

void func(){
    for(int i=0;i<1e7;i++){
        lock();
        counter++;
        unlock();
    } 
}

int main(){
    thread t1(func);
    thread t2(func);

    t1.join();
    t2.join();
    cout<<counter<<endl;
    return 0;
}
