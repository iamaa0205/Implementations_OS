#include <iostream>
#include <thread>
#include <queue>
class Queue{
    private:
    std::queue<int> q;
    std::mutex mtx;
    std::condition_variable cv;

    public:
    void insert(int value){
        std::lock_guard<std::mutex> mymtx(mtx);
        q.push(value); // can use std::move like 
        // q.push(std::move(value));
        cv.notify_one();
    }

    int pop(){
        std::unique_lock<std::mutex> mymtx(mtx);
        // unique lock is used for wait in cvs since unique lock can be transferred between functions and 
        // provide some additional flexibilities like calling unlock and maybe call lock later using std::defer_lock
        while(q.empty()) cv.wait(mymtx); // there can be spurious wakeups therefore while loop nneded to check empty consition again 
        int val = q.front(); // can use std::move here also
        q.pop();
        return val;
    }
};

int main(){
    Queue q;
    

    return 0;
}