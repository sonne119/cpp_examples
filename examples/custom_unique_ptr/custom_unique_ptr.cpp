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

int main() {
	MyUniquePtr<Temp> ptr1(new Temp());
	ptr1->Hello();

	MyUniquePtr<Temp> ptr2 = std::move(ptr1);
	
    if (!ptr1.Get()) {
		std::cout << "ptr1 was moved to ptr2 and now is null \n";
	}

	Temp* raw_ptr = ptr2.Release();
	
    if (!ptr2.Get()) {
		std::cout << "ptr2 was released and pass owner to raw_ptr  and now is null \n";
	}
	raw_ptr->Hello(); // call method from raw pointer

	return 0;
}

