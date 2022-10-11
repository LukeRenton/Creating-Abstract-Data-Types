#include "linkedlist.h"

/**
 * @brief LinkedListIterator::operator*
 * @return Return a reference to the thing in the link that we're pointing to
 */
Thing &LinkedListIterator::operator*()
{
       return ptr->value;
}

/**
 * @brief LinkedListIterator::operator++
 * Make the current iterator point to the next link in the list.
 * @return Return a reference to this object.
 */
LinkedListIterator &LinkedListIterator::operator++()
{
    ptr = ptr->next;
    return *this;
}


/**
 * @brief LinkedList::LinkedList
 * Already correct as head is set to nullptr in the header file.
 */
LinkedList::LinkedList()
{
    // This is already correct.

}

/**
 * @brief LinkedList::~LinkedList
 * Free every link in the list. Do this function last.
 */
LinkedList::~LinkedList()
{
    // Consider using functions that already exist.
    while (head != nullptr){
        pop_back();
    }
}

/**
 * @brief LinkedList::push_front
 * @param t
 * Push t to the front of the list
 */
void LinkedList::push_front(Thing t)
{
    Link * temp = new Link(t);
    temp->next = head;
    head = temp;
}

/**
 * @brief LinkedList::pop_front
 * Remove the front item in the list
 */
void LinkedList::pop_front()
{
    Link *temp = head;
    head = head->next;
    delete temp;
}

/**
 * @brief LinkedList::push_back
 * @param t
 * Add t to the back of the list
 */
void LinkedList::push_back(Thing t)
{
    Link *curr = head;
    Link *temp = new Link(t);
    if (head == nullptr){
        head = temp;
    }
    else{
        while (curr->next != nullptr){
             curr = curr->next;
        }
        curr->next = temp;
    }
}

/**
 * @brief LinkedList::pop_back
 * Remove the last link in the list
 */
void LinkedList::pop_back()
{

    Link *curr = head;
    if (curr->next == nullptr){
        std::cout<< head->value.i << std::endl;
        delete curr;
        head = nullptr;


    }
   else{
        while (curr->next->next != nullptr){
             curr = curr->next;
        }
        delete curr->next;
        curr->next = nullptr;
    }

}

/**
 * @brief LinkedList::size
 * @return number of items in the list
 */
size_t LinkedList::size()
{
    if (head == nullptr){
        return 0;
    }
    size_t counter = 0;
    Link * curr = head;
    while (curr->next != nullptr){
        curr = curr->next;
        ++counter;
    }
    ++counter;
    return counter;
}

/**
 * @brief LinkedList::front
 * @return a reference to the first item in the list
 */
Thing &LinkedList::front()
{
    return head->value;
}

/**
 * @brief LinkedList::back
 * @return a reference to the last item in the list
 */
Thing &LinkedList::back()
{
    if (head == nullptr){
        return head->value;
    }
    Link * curr = head;
    while (curr->next != nullptr){
        curr = curr->next;
    }
    return curr->value;
}

/**
 * @brief LinkedList::get_link
 * @param i
 * @return A pointer to the ith link
 * @throws std::out_of_range("i out of bounds") - try do this without calling size()
 */
Link *LinkedList::get_link(int i)
{
    if (i > size() - 1 || i < 0){
        throw std::out_of_range("i out of bounds");
    }
    Link * curr = head;
    for (size_t j = 0;j<i; j++){
        curr = curr->next;
    }
    return curr;

}

/**
 * @brief LinkedList::at
 * @param i
 * @return A reference to the thing at index i
 * @throws std::out_of_range("i out of bounds") - try do this without calling size()
 */
Thing &LinkedList::at(int i)
{

    return get_link(i)->value;
}

/**
 * @brief LinkedList::begin
 * @return a LinkedListIterator object referencing the first item
 */
LinkedListIterator LinkedList::begin()
{
    // Remember to implemente both LinkedListIterator::operator* and
    //   LinkedListIterator::operator++ in order to pass the test cases.
    // They are at the top of this file.
    LinkedListIterator iter;
    iter.ptr = head;
    return iter;
}

/**
 * @brief LinkedList::end
 * @return a LinkedListIterator representing one past the last item
 * Let this be a LinkedLIstIterator with ptr equal to the nullptr
 */
LinkedListIterator LinkedList::end()
{
        LinkedListIterator last;
        last.ptr = nullptr;
        return last;
}

/**
 * @brief LinkedList::copy
 * @return A pointer to a copy of the list
 * Allocate a new list on the heap, then add all the items to the list.
 * Do not allocate the list on the stack, because it would be destroyed when the function ends.
 *  - See the C++ Concepts PDF example.
 */
LinkedList *LinkedList::copy()
{
    LinkedList * myList = new LinkedList;
    for (size_t i = 0;i<size();i++){
        myList->push_back(at(i));
    }
    return myList;
}

/**
 * @brief LinkedList::reverse
 * Reverse the list.
 *  half marks: all the values are reversed
 *  remaining marks: list is reversed by changing link pointers *only*
 *    - if it was expensive to copy Thing objects, then reversing the values would take
 *       a lot of time. If you can do it by only changing the link pointers, then it will
 *       always be fast no matter how big the thing objects are.
 *  You can do this in O(n^2) time quite easily. Try think about how you could do this in O(n) time.
 */
void LinkedList::reverse()
{
    Link * curr = head;
    Link * prev = nullptr;
    Link * temp = nullptr;
    while(curr->next != nullptr){
        temp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = temp;
    }
    curr->next = prev; //change the null ptr to the previous one
    head = curr;

}

