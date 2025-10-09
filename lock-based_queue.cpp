#include <iostream>
#include <thread>
#include <queue>
template <typename T>
class Queue{
    private:
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv;

    public:
    void insert(T value){
        std::lock_guard<std::mutex> mymtx(mtx);
        q.push(std::move(value)); // can use std::move like 
        // q.push(std::move(value));
        cv.notify_one();
    }

    T wait_pop(){
        std::unique_lock<std::mutex> mymtx(mtx);
        // unique lock is used for wait in cvs since unique lock can be transferred between functions and 
        // provide some additional flexibilities like calling unlock and maybe call lock later using std::defer_lock
        while(q.empty()) cv.wait(mymtx); // there can be spurious wakeups therefore while loop nneded to check empty consition again 
        // cv.wait(mymtx,[this] {return !q.empty();}); //can also pass like this pedicate in wait function
        T val = std::move(q.front()); // can use std::move here also
        q.pop();
        return val;
    }

    bool try_pop(T &value){
        std::lock_guard<std::mutex> mymtx(mtx);
        if(q.empty()) return false;
        else{
            value = std::move(q.front());
            q.pop();
            return true;
        }
    }
};

int main(){
    std::thread producers[5];
    return 0;
}