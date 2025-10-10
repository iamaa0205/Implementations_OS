#include <iostream>
#include <thread>
// Assumption: This same implementation works for SPMC, MPSC and MPMC as well given that any thread can produce and any one consumer can consume.

template <typename T, size_t BUFFER_SIZE> // size_t is unsigned int normally used to represent size of data structures
class TSQueue{
    private:
    std::mutex mtx;
    std::condition_variable p_cv;
    std::condition_variable c_cv;
    int p_idx = 0;
    int c_idx = 0;
    int count = 0;
    T buffer[BUFFER_SIZE];

    public:
    void produce(T &value){
        std::unique_lock<std::mutex> mymtx(mtx);
        p_cv.wait(mymtx, [this] {return !(count==BUFFER_SIZE);});
        buffer[p_idx] = std::move(value);
        p_idx = (p_idx+1)%BUFFER_SIZE;
        count++;
        mymtx.unlock();  // need to unlock before as then consume thread may sleep again
        // can use scoping also so that unique lock goes out of scope and unlocks
        // improves performance also
        c_cv.notify_one();
    }

    T consume(){
        T val;
        {
            std::unique_lock<std::mutex> mymtx(mtx);
            c_cv.wait(mymtx, [this] {return !(count==0);});
            val = std::move(buffer[c_idx]);
            c_idx = (c_idx+1)%BUFFER_SIZE;
            count--;
        }
        // {} automatically unlocks the code by RAII principles
        // needed before calling notify_one
        // mymtx.unlock();
        p_cv.notify_one();
        return val;
    }

};
// can't use different lock for producer and consumer because count is shared variable and data race might happen

template <typename T, size_t size>
void producer(TSQueue<T, size>& q) {
    for (int i = 1; i <= 20; ++i) {
        q.produce(i);
        std::cout << "Produced: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

template <typename T, size_t size>
void consumer(TSQueue<T, size>& q) {
    for (int i = 1; i <= 20; ++i) {
        T value = q.consume();
        std::cout << "Consumed: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main() {
    TSQueue<int, 10> queue;

    std::thread prod(producer<int, 10>, std::ref(queue));
    std::thread cons(consumer<int, 10>, std::ref(queue));

    prod.join();
    cons.join();

    return 0;
}
