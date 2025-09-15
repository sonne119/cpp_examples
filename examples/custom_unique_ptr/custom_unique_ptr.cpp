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
           // m_ptr = std::exchange(other.m_ptr, nullptr);
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

int main() {
	MyUniquePtr<Temp> ptr1(new Temp());
	ptr1->Hello(); // call method from unique pointer

	MyUniquePtr<Temp> ptr2 = std::move(ptr1); //move ownership from ptr1 to ptr2
	
    if (!ptr1.Get()) {
		std::cout << "ptr1 was moved to ptr2 and now is null \n";
	}

    MyUniquePtr<Temp> ptr3(new Temp());
    MyUniquePtr<Temp> ptr4(new Temp());
    MyUniquePtr<Temp> ptr5(nullptr);

    ptr3 = std::move(ptr4); //std::move(ptr4) casts ptr4 to an rvalue reference (MyUniquePtr&&)

    ptr5 = MyUniquePtr<Temp>(new Temp()); // move assignment from a temporary MyUniquePtr

	Temp* raw_ptr = ptr5.Release();
	
    if (!ptr5.Get()) {
		std::cout << "ptr2 was released and pass owner to raw_ptr  and now is null \n";
	}
	raw_ptr->Hello(); // call method from raw pointer


    Temp& tempRef = *raw_ptr; //derreference raw pointer to get reference 
                              //and giving you the actual  object it points to
    tempRef.Hello(); // call method from reference
    raw_ptr->Hello(); // call method from pointer

	return 0;
}

