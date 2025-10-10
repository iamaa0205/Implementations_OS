// Source: https://geidav.wordpress.com/2016/03/23/test-and-set-spinlocks/
// Notes: While the TAS Lock is extremely easy to implement, its scalability is very bad. Already with just a few threads competing for the lock, the amount of required cache line invalidations to acquire/release the lock quickly degrades performance.
// 1. std::atomic_bool::exchange() always invalidates the cache line Locked resides in, regardless of whether it succeeded in updating Locked or not.
// 2. When the spinlock is released, all waiting threads simultaneously try to acquire it. This is sometimes referred to as Thundering Herd problem. Acquiring the lock means first invalidating the cache line copy of all threads waiting for the lock and then reading the valid cache line copy from the core which released the lock. With t threads this results in O(t) memory bus transactions.
// The latter issue is inherent to TAS Locks, because just a single synchronization variable is used and shared between all threads. However, the first issue can be fixed by testing if the lock is free before calling exchange(). First testing if the lock is free only loads the synchronization variable and therefore doesn’t cause a cache line invalidation. This spinlock variant is called Test-And-Test-And-Set (TTAS) Lock.
#include <iostream>
#include <thread>
#include <atomic>

class SpinLockTTAS{
    private:
    std::atomic_flag lock=ATOMIC_FLAG_INIT;
    
    public:
    void lock(){
        while(1){
            while(lock.test(std::memory_order_relaxed)==1);
            if(lock.test_and_set(std::memory_order_acquire)==0){
                return; // check only once
            }
        }
    }

    void unlock(){
        lock.clear(std::memory_order_release);
    }
};

int main(){
    return 0;
}