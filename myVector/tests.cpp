#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cstring>

#define _GLIBCXX_VECTOR 1
#include "myvector.h"

//#ifdef _WIN32
//int main(int argc, char* argv[])
//{
//    int result = Catch::Session().run(argc, argv);
//    return result;
//}
//#endif

class MyVectorExtended : public MyVector{
public:
    Thing* get_data(){ return data;}
    size_t get_n_items(){ return n_items; }
    size_t get_n_allocated(){ return n_allocated;}
};

TEST_CASE("Constructor Tests"){
    // Don't look at this code, it will make you sad :(
    MyVectorExtended* buf = (MyVectorExtended*)malloc(sizeof(MyVectorExtended));
    std::memset(buf,1, sizeof(MyVectorExtended));
    new (buf) MyVectorExtended;
    Thing* data = buf->get_data();
    SECTION("Initialise Data Members"){
        INFO("Make sure that data points to null in a newly constructed MyVector.");
        REQUIRE(data == nullptr);
    }
    SECTION("n_items"){
        INFO("Make sure that n_items is 0 in a newly constructed MyVector.")
                size_t n_items = buf->get_n_items();
        REQUIRE(n_items == 0);
    }
    SECTION("n_allocated"){
        INFO("Make sure that n_allocated is 0 in a newly constructed MyVector.")
                size_t n_allocated = buf->get_n_allocated();
        REQUIRE(n_allocated == 0);
    }
    // Intentional memory leak to avoid calling the destructor.
}


TEST_CASE_METHOD(MyVector, "Size and Allocation Functions"){
    INFO("Check that the size() and allocated_length() report "
         "the values in n_items and n_allocated.");

    SECTION("Empty List"){
        REQUIRE(n_items == 0);
        REQUIRE(n_allocated == 0);
    }
    SECTION("Manually changing n_items/n_allocated"){
        INFO("Here I manually change the value of the variable "
             "to a random number to check that you're outputting"
             " the variable");

        size_t r1 = rand();
        size_t r2 = rand();
        n_items = r1;
        n_allocated = r2;
        REQUIRE(size() == r1);
        REQUIRE(allocated_length() == r2);
    }
}
TEST_CASE_METHOD(MyVector, "Buffer Reallocation"){
    SECTION("Is reallocate() working?"){
        Thing::last_alloc = 0;
        size_t random_number = rand() % 10;
        ++random_number;
        reallocate(random_number);
        REQUIRE(random_number == Thing::last_alloc);
    }
    SECTION("Checking reallocation, push_back, and pop_back"){
        INFO("None");
        Thing::last_alloc = 0;
        INFO("Empty Vector");
        REQUIRE(size() == 0);
        INFO("Pushed 1st thing");
        push_back(Thing(0));
        REQUIRE(size() == 1);
        REQUIRE(Thing::last_alloc == 1);
        INFO("Pushed 2nd thing");
        push_back(Thing(1));
        REQUIRE(size() == 2);
        REQUIRE(Thing::last_alloc == 2);
        INFO("Pushed 3rd thing");
        push_back(Thing(2));
        REQUIRE(size() == 3);
        REQUIRE(Thing::last_alloc == 4);
        INFO("Pushed 4th thing");
        push_back(Thing(3));
        REQUIRE(size() == 4);
        REQUIRE(Thing::last_alloc == 4);
        INFO("Pushed 5th thing");
        push_back(Thing(4));
        REQUIRE(size() == 5);
        REQUIRE(Thing::last_alloc == 8);
        INFO("Pushed 6th thing");
        push_back(Thing(5));
        REQUIRE(size() == 6);
        REQUIRE(Thing::last_alloc == 8);
        INFO("Pushed 7th thing");
        push_back(Thing(6));
        REQUIRE(size() == 7);
        REQUIRE(Thing::last_alloc == 8);
        INFO("Pushed 9th thing");
        push_back(Thing(7));
        REQUIRE(size() == 8);
        REQUIRE(Thing::last_alloc == 8);
        INFO("Pushed 9th thing");
        push_back(Thing(8));
        REQUIRE(size() == 9);
        REQUIRE(Thing::last_alloc == 16);
        INFO("Popped 9th thing");
        pop_back();
        REQUIRE(size() == 8);
        REQUIRE(Thing::last_alloc == 16);
        INFO("Popped 8th thing");
        pop_back();
        REQUIRE(size() == 7);
        REQUIRE(Thing::last_alloc == 16);
        INFO("Popped 7th thing");
        pop_back();
        REQUIRE(size() == 6);
        REQUIRE(Thing::last_alloc == 16);
        INFO("Popped 6th thing");
        pop_back();
        REQUIRE(size() == 5);
        REQUIRE(Thing::last_alloc == 16);
        INFO("Popped 5th thing");
        pop_back();
        REQUIRE(size() == 4);
        REQUIRE(Thing::last_alloc == 16);
        INFO("Popped 4th thing");
        pop_back();
        REQUIRE(size() == 3);
        REQUIRE(Thing::last_alloc == 8);
        INFO("Popped 3rd thing");
        pop_back();
        REQUIRE(size() == 2);
        REQUIRE(Thing::last_alloc == 8);
        INFO("Popped 2nd thing");
        pop_back();
        REQUIRE(size() == 1);
        REQUIRE(Thing::last_alloc == 4);
        INFO("Popped 1st thing");
        pop_back();
        REQUIRE(size() == 0);
        REQUIRE(Thing::last_alloc == 2);
    }
}

TEST_CASE_METHOD(MyVector, "Testing operator[]() and at()"){
    for(int i = 0; i < 10; ++i){
        push_back(Thing(i));
    }
    for(int i = 0; i < 10; ++i){
        Thing& myThing = operator[](i);
        REQUIRE(myThing.i == i);
    }
    for(int i = 0; i < 10; ++i){
        Thing& myThing = at(i);
        REQUIRE(myThing.i == i);
    }
    REQUIRE_THROWS(at(15));
    REQUIRE_THROWS(at(-1));
}


TEST_CASE_METHOD(MyVector, "begin(), end(), front(), back()"){
    REQUIRE(begin() == nullptr);
    REQUIRE(end() == nullptr);

    push_back(Thing(42));
    REQUIRE(begin()->i == 42);
    REQUIRE((end()-1)->i == 42);
    REQUIRE(front().i == 42);
    REQUIRE(back().i == 42);

    for(int i = 0; i < 10; ++i){
        push_back(Thing(i));
        REQUIRE(begin()->i == 42);
        REQUIRE(front().i == 42);
        REQUIRE(back().i == i);
        REQUIRE((end()-1)->i == i);
    }
}
