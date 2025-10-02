// Peterson’s Algorithm is a classic software-based solution for the critical section problem in operating systems. It ensures mutual exclusion between two processes
// It guarantees: 1. Mutual Exclusion 2. Progress 3. Bounded Waiting

#include <iostream>
#include <thread>

using namespace std;

bool flag[2];
int turn;
int counter;
void init() {
    flag[0] = false;
    flag[1] = false;
    turn = 0;
}

void algo(int id){
    int other = 1-id;
    flag[id] = true;
    turn = 1-id;

    while(flag[other] && turn==other);
    //critical section
    for(int i=0;i<1e7;i++) counter++;
    cout<<"Process "<<id<<" is in critical section"<<endl;
    flag[id] = false;
    //remainder section
    cout<<"Process "<<id<<" is in remainder section"<<endl; 
    return;
}

int main(){
    cout<<"Peterson's Algorithm Initialized"<<endl;
    init();

    thread t1(algo,0);
    thread t2(algo,1);

    t1.join();
    t2.join();
    cout<<"Final Counter Value: "<<counter<<endl;
    return 0;
}