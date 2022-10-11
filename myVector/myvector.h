#ifndef MYVECTOR_H
#define MYVECTOR_H
#define _GLIBCXX_VECTOR 1

#include <iostream>

class Thing{
public:
    int i;
    static bool verbose;
    static size_t last_alloc;

    Thing() : i(-1){}
    Thing(int i) : i(i){}

    ~Thing(){
        if(Thing::verbose){
            std::cout << "Deleting thing with value " << i << std::endl;
        }
    }

    /**
     * @brief operator new[]
     * @param sz
     * @return
     * Ignore this function completely... its just here to help mark.
     */
    void* operator new[](size_t sz){
        size_t count = (sz - sizeof(size_t)) / sizeof(Thing);
        if(verbose){
           std::cout << "Allocated space for " << count << " thing(s)" << std::endl;
        }
        last_alloc = count;
        return malloc(sz);
    }

};

class MyVector
{
public:
    MyVector();
    ~MyVector();

    size_t size() const;
    size_t allocated_length() const;

    void push_back(const Thing& t);
    void pop_back();

    Thing& front();
    Thing& back();

    Thing* begin();
    Thing* end();

    Thing& operator[](size_t i);
    Thing& at(size_t i);

protected:
    void reallocate(size_t new_size);

    Thing* data;
    size_t n_items, n_allocated;
};

#endif // MYVECTOR_H
