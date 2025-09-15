#include <iostream>
#include <memory>

using namespace std;

template <typename T>
class MySharedPtr {
private:
	T* ptr;
	size_t* ref_count = nullptr;

public:
	explicit MySharedPtr(T* p = nullptr)
		: ptr(p), ref_count(new size_t(1)) {}

	MySharedPtr(const MySharedPtr& other) noexcept
		: ptr(other.ptr), ref_count(other.ref_count)
	{
		if (ref_count) {
			++(*ref_count);
		}
	}

	MySharedPtr(MySharedPtr&& other) = delete;

	MySharedPtr& operator=(MySharedPtr&& other) = delete;  // first implementation

	//   MySharedPtr& operator=(const MySharedPtr& other) noexcept {
	   //	if (this != &other) {
	   //		release(); 
	   //		ptr = other.ptr;
	   //		ref_count = other.ref_count;
	   //		if (ref_count) {
	   //			++(*ref_count); 
	   //		}
	   //	}
	   //	return *this;
	   //}

	MySharedPtr& operator=(MySharedPtr other) noexcept { // pass copy by value
		swap(other);                                     //second implementation     
		return *this;
	}

	~MySharedPtr() {
		//delete ptr;
		release();
	}

	T& operator*() const {
		return *ptr;
	}

	T* operator->() const {
		return ptr;
	}
	size_t Get() const {
		return *ref_count;
	}


private:
	void release() {
		if (ref_count) {
			--(*ref_count);
			if (*ref_count == 0) {
				delete ptr;
				delete ref_count;
			}
			ptr = nullptr;
			ref_count = nullptr;
		}
	}

	void swap(MySharedPtr& other) noexcept {
		std::swap(ptr, other.ptr);
		std::swap(ref_count, other.ref_count);
	}
};


struct Temp {
	int value;
	void Hello() { std::cout << "Hello temp!\n"; }
};

int main() {

	MySharedPtr<Temp> ptr1(new Temp{ 10 });   //Default constructor
	ptr1->Hello();
	(*ptr1).value = 55;
	std::cout << "ptr1 value after: " << ptr1->value << "\n";

	std::cout << "ptr1 ref_count: " << ptr1.Get() << "\n";
	MySharedPtr<Temp> ptr2 = ptr1;            //Copy constructor

	std::cout << "ptr2 ref_count: " << ptr2.Get() << "\n";


	MySharedPtr<Temp> ptr3(new Temp{ 18 });
	std::cout << "ptr3 ref_count before : " << ptr3.Get() << "\n";
	ptr3 = ptr1;                              // copy assignment
	std::cout << "ptr1 ref_count: " << ptr1.Get() << "\n";
	std::cout << "ptr3 ref_count after: " << ptr3.Get() << "\n";

	return 0;
}

