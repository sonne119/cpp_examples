#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>
#include <functional>


class Singleton {
public:
    static Singleton* getInstance(const std::string& value);
    static Singleton& GetInstance(const std::string& value);

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    const std::string& Value() const noexcept {
        return value_;
    }
    void Print() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "This is #" << this << " singleton" << "\n";
    }

private:
    explicit Singleton(const std::string& value) : value_(value) {}
    ~Singleton() = default;

    static Singleton* instance_;
    static std::mutex mutex_;
    std::string value_;
};


Singleton* Singleton::instance_ = nullptr;
std::mutex Singleton::mutex_;
//std::once_flag Singleton::flag_;

Singleton* Singleton::getInstance(const std::string& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance_ == nullptr) {
        instance_ = new Singleton(value);
    }
    return instance_;
}


Singleton& Singleton::GetInstance(const std::string& value) {
    static Singleton instance(value); 
    return instance;
}

//Singleton& Singleton::GetInstance(const std::string& value) {
//    std::call_once(flag_, [&]() {
//        instance_ = new Singleton(value);
//        });
//    return *instance_;
//}


void worker(std::function<Singleton& (const std::string&)> getSingleton, std::string value)
{
    for (size_t i = 0; i < 3; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        Singleton& singleton = getSingleton(value);
        std::cout << singleton.Value() << "\n";
        singleton.Print();
    }
}

int main() {
    std::cout << "RESULT:\n";

    auto use_ptr = [](const std::string& v) -> Singleton& { return *Singleton::getInstance(v); };
    auto use_ref = [](const std::string& v) -> Singleton& { return Singleton::GetInstance(v); };

    std::thread t1(worker, use_ptr, std::string("BAR"));
    std::thread t2(worker, use_ptr, std::string("FOO"));
    t1.join();
    t2.join();

    std::thread t3(worker, use_ref, std::string("foo"));
    std::thread t4(worker, use_ref, std::string("bar"));
    t3.join();
    t4.join();

    std::cout << "End" << "\n";

    return 0;
}

//RESULT:
//FOO
//This is #0000020624709C50 singlton
//FOO
//This is #0000020624709C50 singlton
//FOO
//This is #0000020624709C50 singlton
//FOO
//This is #0000020624709C50 singlton
//foo
//This is #000002062471C200 singlton
//foo
//This is #000002062471C200 singlton
//foo
//This is #000002062471C200 singlton
//foo
//This is #000002062471C200 singlton

