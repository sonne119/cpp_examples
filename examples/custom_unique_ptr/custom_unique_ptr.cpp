#include <iostream>
#include <memory>

using namespace std;

template <typename T>
class MyUniquePtr {
private:
    T* ptr; 

public:
   
    explicit MyUniquePtr(T* p = nullptr) : ptr(p) {}

   
    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    
    MyUniquePtr(MyUniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;        
            ptr = other.ptr;   
            other.ptr = nullptr;
        }
        return *this;
    }

  
    ~MyUniquePtr() {
        delete ptr;
    }


    T& operator*() const {
        return *ptr;
    }


    T* operator->() const {
        return ptr;
    }

    T* Get() const {
        return ptr;
    }

    T* Release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
};


struct Temp {
    void Hello() { std::cout << "Hello temp!\n"; }
};

