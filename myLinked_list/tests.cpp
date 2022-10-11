#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <map>
#include <deque>
#include "linkedlist.h"

#include <iostream>

using namespace std;

#ifdef _WIN32
int main(int argc, char* argv[])
{
    int result = Catch::Session().run(argc, argv);
    return result;
}
#endif

map<void*, bool> still_allocated;
int n_allocated_links = 0;
Link* last_alloc = nullptr;

deque<Link*> allocated_links;
bool track_links = false;

pair<LinkedList*, deque<Link*>> get_list(int sz = 0){
    // Return heap allocate to allow for a memory leak
    LinkedList* output = new LinkedList();
    deque<Link*> links;
    for(int i = 0; i < sz; ++i){
        Link* tmp = new Link(Thing(i*10));
        tmp->next = output->head;
        output->head = tmp;
        links.push_front(tmp);
    }
    return make_pair(output, links);
}

void* Link::operator new(size_t sz){
    last_alloc  = ::new Link(sz);
    still_allocated[last_alloc] = true;
    ++n_allocated_links;
    if(track_links) allocated_links.push_back(static_cast<Link*>(last_alloc));
    return last_alloc;
}

void Link::operator delete(void* ptr){
    still_allocated[ptr] = false;
    ::delete static_cast<Link*>(ptr);
    --n_allocated_links;
}

TEST_CASE("0-Constructor", "[0]"){
    LinkedList ll;
    REQUIRE(ll.head == nullptr);
}

TEST_CASE("1-push-front","[1]"){
    auto link_counter = n_allocated_links;
    LinkedList* myList;
    deque<Link*> links;
    tie(myList, links) = get_list();

    int n = 10;
    for(int i = 0; i < n; ++i){
        Link* prev_head = myList->head;
        myList->push_front(Thing(i));

        UNSCOPED_INFO("head should point to the new link");
        REQUIRE(myList->head == last_alloc);
        UNSCOPED_INFO("the value should have been set correctly");
        REQUIRE(myList->head->value.i == i);
        UNSCOPED_INFO("the old first link should be the second link now");
        REQUIRE(myList->head->next == prev_head);
    }
    int n_allocated = n_allocated_links - link_counter;
    UNSCOPED_INFO("there should only be n links in memory");
    REQUIRE(n_allocated == n);
    // Intentional memory leak
}

TEST_CASE("2-pop-front", "[2]"){
    GIVEN("A list with n items in it."){

        int n = 15;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);
        auto link_counter = n_allocated_links - n;
        Link* prev_head = nullptr;

        WHEN("the elements are popped from the front"){
            for(int i = 1; i < n; ++i){

                prev_head = myList->head;
                myList->pop_front();
                int n_links = n_allocated_links - link_counter;

                UNSCOPED_INFO("the head should be updated to the next link");
                REQUIRE(myList->head == links[1]);
                links.pop_front();

                UNSCOPED_INFO("the link should be freed");
                REQUIRE_FALSE(still_allocated[prev_head]);

                UNSCOPED_INFO("there should only be n-i links in memory");
                REQUIRE(n_links == n-i);
            }

            INFO("the last element is popped from the front");
            prev_head = myList->head;
            myList->pop_front();
            links.pop_front();
            int n_links = n_allocated_links - link_counter;

            UNSCOPED_INFO("the link should be freed");
            REQUIRE_FALSE(still_allocated[prev_head]);

            UNSCOPED_INFO("there should be no links left in memory");
            REQUIRE(n_links == 0);

            UNSCOPED_INFO("the list should be empty");
            REQUIRE(myList->head == nullptr);
        }
    }
}

TEST_CASE("3-push-back","[3]"){
    auto link_counter = n_allocated_links;
    LinkedList* myList;
    deque<Link*> links;
    tie(myList, links) = get_list();
    GIVEN("An empty list"){
        int n = 10;

        UNSCOPED_INFO("after the first push_back the head should point to the new link");
        myList->push_back((Thing(0))); links.push_back(last_alloc);
        REQUIRE(myList->head == last_alloc);
        auto first_link = last_alloc;
        UNSCOPED_INFO("after the first push_back there should be one item in the list");
        REQUIRE(myList->head->next == nullptr);
        UNSCOPED_INFO("after the first push_back there should only be 1 link in memory");
        int n_allocated = n_allocated_links - link_counter;
        REQUIRE(n_allocated == 1);

        for(int i = 1; i < n; ++i){
            myList->push_back(Thing(i)); links.push_back(last_alloc);

            UNSCOPED_INFO("head should still point to the first link");
            REQUIRE(myList->head == first_link);
            UNSCOPED_INFO("head should be " << i+1 << " links in memory");
            n_allocated = n_allocated_links - link_counter;
            REQUIRE(n_allocated == i+1);

            Link* curr = myList->head;
            for(unsigned int j = 0; j < links.size()-1; ++j){
                auto next_correct_link = links[j+1];
                UNSCOPED_INFO("links should be connected correctly from first to last");
                REQUIRE(curr->next == next_correct_link);
                curr = curr->next;
            }
            UNSCOPED_INFO("the last link should point to nullptr");
            auto last_link = curr;
            REQUIRE(last_link->next == nullptr);
            UNSCOPED_INFO("the last link should be the most recently allocated one");
            REQUIRE(last_link == last_alloc);

            UNSCOPED_INFO("the value should have been set correctly");
            REQUIRE(last_link->value.i == i);
        }
        n_allocated = n_allocated_links - link_counter;
        UNSCOPED_INFO("there should be exactly n links in memory");
        REQUIRE(n_allocated == n);
        // Intentional memory leak
    }
}


TEST_CASE("4-pop-back", "[4]"){
    GIVEN("A list with n items in it."){
        int n = 15;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);
        auto link_counter = n_allocated_links - n;
        Link* prev_head;
        (void) prev_head;
        WHEN("the elements are popped from the back"){
            for(int i = 1; i < n; ++i){

                prev_head = myList->head;
                myList->pop_back();
                int n_links = n_allocated_links - link_counter;

                UNSCOPED_INFO("the head should not change");
                REQUIRE(myList->head == links[0]);

                auto last = links.back(); links.pop_back();
                auto second_last = links.back();

                UNSCOPED_INFO("the second last link should now be the last one");
                REQUIRE(second_last->next == nullptr);

                UNSCOPED_INFO("the last link should have been freed");
                REQUIRE_FALSE(still_allocated[last]);

                UNSCOPED_INFO("there should only be n-i links in memory");
                REQUIRE(n_links == n-i);
            }

            INFO("once there is only 1 element left, pop it from the back");
            myList->pop_back();
            int n_links = n_allocated_links - link_counter;
            auto last = links.back(); links.pop_back();

            UNSCOPED_INFO("the link should have been freed");
            REQUIRE_FALSE(still_allocated[last]);

            UNSCOPED_INFO("there should be no links left in memory");
            REQUIRE(n_links == 0);

            UNSCOPED_INFO("the list should be empty");
            REQUIRE(myList->head == nullptr);
        }
    }
}

TEST_CASE("5-size", "[5]"){
    GIVEN("An empty list"){
        auto stuff = get_list();
        auto myList = stuff.first;
        THEN("Size should be 0"){
            REQUIRE(myList->size() == 0);
        }
    }

    GIVEN("A list with n items in it."){
        unsigned int n = 15;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);
        THEN("Size should be equal to n"){
            REQUIRE(myList->size() == n);
        }
        WHEN("calling push_front"){
            myList->push_front(Thing(42));
            THEN("Size should be equal to n+1"){
                REQUIRE(myList->size() == n+1);
            }
        }
        WHEN("calling pop_front"){
            myList->pop_front();
            THEN("Size should be equal to n-1"){
                REQUIRE(myList->size() == n-1);
            }
        }
        WHEN("calling push_back"){
            myList->push_back(Thing(42));
            THEN("Size should be equal to n+1"){
                REQUIRE(myList->size() == n+1);
            }
        }
        WHEN("calling pop_back"){
            myList->pop_back();
            THEN("Size should be equal to n-1"){
                REQUIRE(myList->size() == n-1);
            }
        }
    }

    GIVEN("A list with a random number of items in it."){
        unsigned int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);
        THEN("Size should be equal to n"){
            REQUIRE(myList->size() == n);
        }
        WHEN("calling push_front"){
            myList->push_front(Thing(42));
            THEN("Size should be equal to n+1"){
                REQUIRE(myList->size() == n+1);
            }
        }
        WHEN("calling pop_front"){
            myList->pop_front();
            THEN("Size should be equal to n-1"){
                REQUIRE(myList->size() == n-1);
            }
        }
        WHEN("calling push_back"){
            myList->push_back(Thing(42));
            THEN("Size should be equal to n+1"){
                REQUIRE(myList->size() == n+1);
            }
        }
        WHEN("calling pop_back"){
            myList->pop_back();
            THEN("Size should be equal to n-1"){
                REQUIRE(myList->size() == n-1);
            }
        }
    }
}


TEST_CASE("6-front-back", "[6]"){
    GIVEN("A list with n items in it."){
        unsigned int n = 15;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        UNSCOPED_INFO("Front should reference the first item");
        REQUIRE(&myList->front() == &myList->head->value);
        Link* last_link = links.back();
        UNSCOPED_INFO("Back should reference the last item");
        REQUIRE(&myList->back() == &last_link->value);
    }

    GIVEN("A list with a random number of items in it."){
        unsigned int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        UNSCOPED_INFO("Front should reference the first item");
        REQUIRE(&myList->front() == &myList->head->value);
        Link* last_link = links.back();
        UNSCOPED_INFO("Back should reference the last item");
        REQUIRE(&myList->back() == &last_link->value);
    }
}

TEST_CASE("7-get-link", "[7]"){
    GIVEN("A list with n items in it."){
        unsigned int n = 15;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        for(unsigned int i = 0; i < n; ++i){
            Link* link_i = links[i];
            UNSCOPED_INFO("get_link(i) should traverse to the ith link");
            REQUIRE(myList->get_link(i) == link_i);
        }
    }

    GIVEN("A list with a random number of items in it."){
        unsigned int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        for(unsigned int i = 0; i < n; ++i){
            Link* link_i = links[i];
            UNSCOPED_INFO("get_link(i) should traverse to the ith link");
            REQUIRE(myList->get_link(i) == link_i);
        }
    }
}

TEST_CASE("8-get-link-throws", "[8]"){
    GIVEN("A list with n items in it."){
        unsigned int n = 15;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        UNSCOPED_INFO("get_link(i) where i < 0 should throw an error");
        REQUIRE_THROWS(myList->get_link(-1));

        UNSCOPED_INFO("get_link(i) where i == n should throw an error");
        REQUIRE_THROWS(myList->get_link(n));

        UNSCOPED_INFO("get_link(i) where i > n should throw an error");
        REQUIRE_THROWS(myList->get_link(n+1));
    }

    GIVEN("A list with a random number of items in it."){
        unsigned int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        UNSCOPED_INFO("get_link(i) where i < 0 should throw an error");
        REQUIRE_THROWS(myList->get_link(-1));

        UNSCOPED_INFO("get_link(i) where i == n should throw an error");
        REQUIRE_THROWS(myList->get_link(n));

        UNSCOPED_INFO("get_link(i) where i > n should throw an error");
        REQUIRE_THROWS(myList->get_link(n+1));
    }
}

TEST_CASE("9-at", "[9]"){
    GIVEN("A list with n items in it."){
        unsigned int n = 20;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        for(unsigned int i = 0; i < n; ++i){
            Link* link_i = links[i];
            UNSCOPED_INFO("at(i) should return a reference to the ith value");
            REQUIRE(&myList->at(i) == &link_i->value);
        }
    }

    GIVEN("A list with a random number of items in it."){
        unsigned int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        for(unsigned int i = 0; i < n; ++i){
            Link* link_i = links[i];
            UNSCOPED_INFO("at(i) should return a reference to the ith value");
            REQUIRE(&myList->at(i) == &link_i->value);
        }
    }
}


TEST_CASE("10-at-throws", "[10]"){
    GIVEN("A list with n items in it."){
        unsigned int n = 20;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        UNSCOPED_INFO("at(i) where i < 0 should throw an error");
        REQUIRE_THROWS(myList->at(-1));

        UNSCOPED_INFO("at(i) where i == n should throw an error");
        REQUIRE_THROWS(myList->at(n));

        UNSCOPED_INFO("at(i) where i > n should throw an error");
        REQUIRE_THROWS(myList->at(n+1));
    }

    GIVEN("A list with a random number of items in it."){
        unsigned int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        UNSCOPED_INFO("at(i) where i < 0 should throw an error");
        REQUIRE_THROWS(myList->at(-1));

        UNSCOPED_INFO("at(i) where i == n should throw an error");
        REQUIRE_THROWS(myList->at(n));

        UNSCOPED_INFO("at(i) where i > n should throw an error");
        REQUIRE_THROWS(myList->at(n+1));
    }
}

TEST_CASE("11-iterators-begin-end", "[11]"){
    GIVEN("A list with n items in it."){
        unsigned int n = 20;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        UNSCOPED_INFO("The begin() iterator should point to the first link");
        REQUIRE(myList->begin().ptr == myList->head);

        UNSCOPED_INFO("The end() iterator should point nullptr");
        REQUIRE(myList->end().ptr == nullptr);


        LinkedListIterator curr_iterator = myList->begin();
        for(unsigned int i = 0; i < n; ++i){
            Link* link_i = links[i];
            Thing& thing_i = link_i->value;

            UNSCOPED_INFO("current iterator should reference the correct link");
            REQUIRE(curr_iterator.ptr == link_i);
            UNSCOPED_INFO("dereferencing the current iterator should reference the correct thing");
            REQUIRE(*curr_iterator == thing_i);
            UNSCOPED_INFO("dereferencing the current iterator should reference the correct thing");
            REQUIRE(&*curr_iterator == &thing_i);

            UNSCOPED_INFO("incrementing the current iterator should return a reference to itself");
            LinkedListIterator* curr_address = &curr_iterator;
            REQUIRE(curr_address == &(++curr_iterator));

            UNSCOPED_INFO("incrementing the current iterator should reference the next link");
            REQUIRE(curr_iterator.ptr == link_i->next);
        }
    }
    GIVEN("A list with a random number of items in it."){
        unsigned int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        UNSCOPED_INFO("The begin() iterator should point to the first link");
        REQUIRE(myList->begin().ptr == myList->head);

        UNSCOPED_INFO("The end() iterator should point nullptr");
        REQUIRE(myList->end().ptr == nullptr);


        LinkedListIterator curr_iterator = myList->begin();
        for(unsigned int i = 0; i < n; ++i){
            Link* link_i = links[i];
            Thing& thing_i = link_i->value;

            UNSCOPED_INFO("current iterator should reference the correct link");
            REQUIRE(curr_iterator.ptr == link_i);
            UNSCOPED_INFO("dereferencing the current iterator should reference the correct thing");
            REQUIRE(*curr_iterator == thing_i);
            UNSCOPED_INFO("dereferencing the current iterator should reference the correct thing");
            REQUIRE(&*curr_iterator == &thing_i);

            UNSCOPED_INFO("incrementing the current iterator should return a reference to itself");
            LinkedListIterator* curr_address = &curr_iterator;
            REQUIRE(curr_address == &(++curr_iterator));

            UNSCOPED_INFO("incrementing the current iterator should reference the next link");
            REQUIRE(curr_iterator.ptr == link_i->next);
        }
    }
}

TEST_CASE("12-copy", "[12]"){
    GIVEN("A list with a random number of items in it."){
        auto link_counter = n_allocated_links;
        unsigned int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        track_links = true;
        allocated_links = deque<Link*>();

        THEN("Copy the list"){
            LinkedList* myList2 = nullptr;
            myList2 = myList->copy();
            REQUIRE(myList2 != nullptr);
            REQUIRE(myList2 != myList);
            UNSCOPED_INFO("Should have allocated n new links");
            REQUIRE(allocated_links.size() == n);
            UNSCOPED_INFO("There should only be 2n links in memory");
            unsigned int n_links = n_allocated_links - link_counter;
            REQUIRE(n_links == 2*n);

            Link* prev_mylist_link_i = nullptr;
            Link* prev_mylist2_link_i = nullptr;
            for(unsigned int i = 0; i < n; ++i){
                UNSCOPED_INFO("get_link(i) should return the relevant link in each list and should not be the same");
                REQUIRE(myList2->get_link(i) != myList->get_link(i));
                Link* mylist_link_i = myList->get_link(i);
                Link* mylist2_link_i = myList2->get_link(i);
                if(prev_mylist2_link_i != nullptr){
                    UNSCOPED_INFO("Traversing the lists should work");
                    REQUIRE(prev_mylist_link_i->next == mylist_link_i);
                    UNSCOPED_INFO("Traversing the lists should work");
                    REQUIRE(prev_mylist2_link_i->next == mylist2_link_i);
                }
            }
        }
    }
}

TEST_CASE("13-reverse-values", "[13]"){
    GIVEN("A list with a random number of items in it."){
        unsigned int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(n);

        track_links = true;
        allocated_links = deque<Link*>();

        Thing orig_front = myList->front();
        Thing orig_back  = myList->back();

        std::reverse(links.begin(), links.end());
        WHEN("Reverse the list"){
            myList->reverse();
            THEN("The values should be in reverse order"){
                for(unsigned int i = 0; i < n; ++i){
                    auto& rev_value_i = links[i]->value;
                    UNSCOPED_INFO("at(i) should return the correct value");
                    REQUIRE(myList->at(i).i == rev_value_i.i);
                }
                UNSCOPED_INFO("front should return the old back item");
                REQUIRE(myList->front().i == orig_back.i);
                UNSCOPED_INFO("back should return the old front item");
                REQUIRE(myList->back().i == orig_front.i);
            }
        }
    }
}

TEST_CASE("14-reverse-pointers", "[14]"){
    GIVEN("A list with a random number of items in it. Try reverse it by adjusting link pointers only."){
        auto link_counter = n_allocated_links;
        int n = rand() % 100 + 50;
        LinkedList* myList;
        deque<Link*> links;
        tie(myList, links) = get_list(static_cast<int>(n));

        track_links = true;
        allocated_links = deque<Link*>();

        //std::reverse(links.begin(), links.end());
        WHEN("Reverse the list"){
            myList->reverse();
            int n_links = n_allocated_links - link_counter;

            UNSCOPED_INFO("Shouldn't allocate any new links");
            REQUIRE(n_links == n);

            for(int i = 0; i < n-1; ++i){
                auto& rev_value_i = links[n-1-i]->value;
                UNSCOPED_INFO("at(i) should return the correct value");
                REQUIRE(myList->at(i).i == rev_value_i.i);


                UNSCOPED_INFO("Checking that the link pointers were reversed not just values");
                REQUIRE(links[n-1-i]->next == links[n-1-i - 1]);
            }
            auto& rev_value_i = links[0]->value;
            UNSCOPED_INFO("at(i) should return the correct value");
            REQUIRE(myList->at(n-1).i == rev_value_i.i);

            UNSCOPED_INFO("Checking that the link pointers were reversed not just values");
            REQUIRE(links[0]->next == nullptr);
        }
    }
}
TEST_CASE("15-destructor", "[15]"){
    int n = rand() % 100 + 50;
    GIVEN("A list with a random number of items in it." ){
        WHEN("Destructing the list"){
            THEN("There should be no memory leaks"){
                INFO("n = " << n);
                LinkedList* myList;
                deque<Link*> links;
                tie(myList, links) = get_list(static_cast<unsigned int>(n));
                auto link_counter = n_allocated_links;

                delete myList;
                for(int i = 0; i < n; ++i){
                    Link* link_i = links[i];
                    UNSCOPED_INFO("All links should have been freed.");
                    REQUIRE_FALSE(still_allocated[link_i]);
                }
                int n_links = n_allocated_links - link_counter;

                UNSCOPED_INFO("n links should have been freed.");
                REQUIRE(n_links == -n);
            }
        }
    }
}



