#include <iostream>
#include <thread>

struct Node{
    int value;
    Node* next;
};

class LinkedList{
    private:
    Node* head;
    std::mutex m_lock;
    
    public:
    LinkedList(){
        head = nullptr;

    }

    void insert(int value){
        Node* temp = (Node*)malloc(sizeof(Node));
        temp->value = value;
        std::lock_guard<std::mutex> mylock(m_lock); // lock_guard used to handle RAII itself
        temp->next = head;
        head = temp;
    }

    int lookup(int value){
        std::lock_guard<std::mutex> mylock(m_lock);
        Node* temp = head;
        while(temp!=nullptr){
            if(temp->value == value) return 1;
            temp = temp->next;
        }
        return 0;
    }

    void print(){
        Node* temp = head;
        int count = 0;
        while(temp!=nullptr){
            std::cout<<temp->value<<" ";
            temp = temp->next;
            count++;
        }
        std::cout<<std::endl<<"count:"<<count<<std::endl;
    }

    ~LinkedList(){
        Node* curr = head;
        while(curr!=nullptr){
            Node* next = curr->next;
            free(curr); // with malloc use free, with new use delete
            // malloc does not call constructors and free does not call destructors
            curr = next;
        }
    }
};

void func(LinkedList &list, int id){
    for(int i=0;i<100000;i++){
        list.insert(10*id + i);
    }
}

int main(){
    LinkedList list;
    
    std::thread t1(func, std::ref(list), 1); // std::ref is used because we want same linkedlist to pass in all threads and mutex is not copyable, constructs a reference wrapper
    // std::thread is different from normal function call, thread always copies/moves arguments that's why std::ref is needed, std::ref explicitly tells compiler the intentions of developer that it is fine.
    // Read:: https://stackoverflow.com/questions/34078208/passing-object-by-reference-to-stdthread-in-c11
    // Read:: https://stackoverflow.com/questions/73834500/stdthread-constructor-passing-a-value-by-reference-needs-to-call-ref-why
    // Crux: The variable can go out of scope from main thread leading to undefined behaviour, so compiler prevents that by throwing error, writing std::ref explicitly tells compiler that it is what we want.
    std::thread t2(func, std::ref(list), 2);
    std::thread t3(func, std::ref(list), 3);

    t1.join();
    t2.join();
    t3.join();

    list.print();
    return 0;
}

// can be scaled by using lock for each node. often less performant that above. 
// can try mixture of both that lock for fixed length of linked list