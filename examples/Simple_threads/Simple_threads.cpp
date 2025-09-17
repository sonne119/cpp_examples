#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <numeric>
#include <algorithm>

struct Temp {
	void EndWork() { std::cout << "end of working threads!\n"; }
};

template <typename T>
class A {

private:
	T* t_obj = nullptr;

public:
	A(T* obj) : t_obj(obj) {}
	~A() = default;

	void increment_of_range(int& n, std::vector<int>& vec, size_t start, size_t end)
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			vec[i] += n;
		}
	}

	void print_vector(const std::vector<int>& vec)
	{
        std::for_each(vec.begin(), vec.end(), [](int item) { std::cout << item << " "; });
		std::cout << std::endl;
	}
	void callTheEnd()
	{
		t_obj->EndWork();
	}
};

void initializeVector(std::vector<int>& vec) {
	std::iota(vec.begin(), vec.end(), 0);
}

int main()
{
	std::vector<std::unique_ptr<std::thread>> threads;
	threads.reserve(4);

	std::vector<int> vec(1000);
	initializeVector(vec);


	Temp t; int n = 5;
	A<Temp> a_obj(&t);

	size_t sizePart = vec.size() / 4;
	size_t start = 0;

	for (int i = 0, start = 0, end = sizePart; i < 4; ++i, start += sizePart, end += sizePart) {
		auto thread = std::thread([&a_obj, &n, &vec, start, end]() {
			a_obj.increment_of_range(n, vec, start, end);
			});
		if (thread.joinable())
			thread.join();
		start += sizePart;
		end += sizePart;

	}
	a_obj.print_vector(vec);
	
	a_obj.callTheEnd();

}
