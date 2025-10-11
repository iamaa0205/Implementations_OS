#include <iostream>
#include <thread>

// Unlike lock-based_SPSC_array this does not use count and use two mutex
// In this case maximum capacity is BUFFER_SIZE-1
template <typename T, size_t BUFFER_SIZE>
class SPSC{
    private:
    T buffer[BUFFER_SIZE];
    std::condition_variable c_cv;
    std::condition_variable p_cv;
    std::mutex c_mtx;
    std::mutex p_mtx;
    int p_idx = 0; // index to produce next item
    int c_idx = 0; // last index whose item is consumed

    public:
    void produce(T& value){
        {
            std::unique_lock<std::mutex> mymtx(p_mtx);
            p_cv.wait(p_mtx, [this] {return !((p_idx+1)%BUFFER_SIZE == c_idx);})
            buffer[p_idx] = value;
            p_idx = (p_idx+1)%BUFFER_SIZE;
        }
        c_cv.notify_one();
    }

    T consume(){
        T val;
        {
            std::unique_lock<std::mutex> mymtx(c_mtx);
            c_cv.wait(c_mtx, [this] {return !(p_idx==c_idx);})
            val = buffer[c_idx+1];
            c_idx = (c_idx+1)%BUFFER_SIZE;
        }
        p_cv.notify_one();
        return val;
    }
};

int main(){

    return 0;
}