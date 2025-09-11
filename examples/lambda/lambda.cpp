#include <iostream>
#include <vector>
#include <algorithm>

class A {
public:
    void operator()(long& obj) const {
        obj = static_cast<long>(std::pow(obj, 2)); // store result back
        //  obj *= obj;
    }
};

class B {
public:

    void operator()(const long& obj) const { std::cout << "item  : " << obj << std::endl; };
};


int main() {
    std::vector<long> vector;
    for (size_t i = 0; i < 100; i++) {
        vector.push_back(i);
    }

    //std::for_each(vector.begin(), vector.end(), A()); // modifies vector
    //std::for_each(vector.begin(), vector.end(), B()); // prints 

    std::vector<long> v;

    auto add = [&](int n) {      // pass n param
        for (auto& item : vector)
        {
            item += n;           // take refference of item from vector and add n param 
                                 // which was passed to Lambda
        };
    };

    add(12);

    for (auto i : vector)
    {
        std::cout << "item  : " << i << std::endl;
    };
}

