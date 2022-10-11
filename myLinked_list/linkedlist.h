#ifndef MYLINKEDLIST_H
#define MYLINKEDLIST_H

#include<stdexcept>
#include<iostream>

// Disables vector<T>, forward_list<T> and list<T> in STL
#define _GLIBCXX_VECTOR 1
#define _GLIBCXX_FORWARD_LIST 1
#define _GLIBCXX_LIST 1


// Thing that is stored inside the list.
//   You can ignore this completely.
class Thing{
public:
    Thing() : i(-1){}        ///< Default Constructor, Thing t;
    Thing(int i) : i(i) {}   ///< Construct from an int, Thing t(42);
    ~Thing() = default;       ///< Destructor

    bool operator==(const Thing& other) const{
        return i == other.i;
    }
public:
    int i;               ///< Value stored inside this Thing
};

// Represents the links/nodes in the linked list.
class Link{
public:
   Link *next = nullptr; // Next pointer, defaults to nullptr
   Thing value;          // Value, defaults to Thing()

   Link(){}                   // Default Constructor
   Link(Thing v) : value(v){} // Build a new Link from v

   void* operator new(size_t sz);
   void operator delete(void* ptr);
};


//Iterator for our linked list
class LinkedListIterator{
public:
   Link* ptr = nullptr; // Points to the current link.

   Thing& operator*();               // Dereference
   LinkedListIterator& operator++(); // Increment

   bool operator !=(const LinkedListIterator& other){
       return ptr != other.ptr;
   }
};


// Our Linked List
class LinkedList{
public:
   Link *head = nullptr;


   LinkedList();
   ~LinkedList();

   void push_front(Thing t);
   void pop_front();

   void push_back(Thing t);
   void pop_back();

   size_t size();

   Thing& front();
   Thing& back();

   Link*  get_link(int i);
   Thing& at(int i);

   LinkedListIterator begin();
   LinkedListIterator end();

   LinkedList *copy();
   void reverse();
};

#endif // MYLINKEDLIST_H
