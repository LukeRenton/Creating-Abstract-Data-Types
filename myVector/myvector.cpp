#include "myvector.h"
bool Thing::verbose = false;
size_t Thing::last_alloc = 0;

/**
 * @brief MyVector::MyVector Construct a vector with size 0
 *
 * Remember that the data pointer should point to nothing, and
 * counter variables should be initialised.
 */
MyVector::MyVector()
{ 
    data = nullptr;
    n_items = 0;
    n_allocated = 0;

}

/**
 * @brief MyVector::~MyVector Free any memory that you have allocated do this last
 */
MyVector::~MyVector()
{
    // Do this last - if you have an error in your destructor it can crash your program
    //delete [] data;
}

/**
 * @brief MyVector::size
 * @return The number of items in the vector
 */
size_t MyVector::size() const
{
    return n_items;
}

/**
 * @brief MyVector::allocated_length
 * @return The length of the allocated data buffer
 */
size_t MyVector::allocated_length() const
{
    return n_allocated;
}

/**
 * @brief MyVector::push_back
 * @param t The thing to add
 *
 * Add a thing to the back of the vector.
 */
void MyVector::push_back(const Thing &t)
{
    ++n_items;
    if (n_allocated == 0){
        Thing *temp = new Thing[1];
        data = temp;
        ++n_allocated;
    }
    if (n_items > n_allocated){
        n_allocated = n_allocated * 2; //double the amount of memory

        Thing *temp = new Thing[n_allocated]; //create our temporary new memory on the heap

        for (size_t i=0;i<n_items;i++){
            temp[i] = data[i]; //move all the data across
        }
        delete [] data; //prevent memory leak
        data = temp; //make it permanent
    }

    data[n_items - 1] = t;
}
/**
 * @brief MyVector::pop_back
 * Remove the last item from the back.
 * Reallocate with half the space if less than a quarter of the vector is used.
 */
void MyVector::pop_back()
{
    --n_items;
    float check = float(n_items)/float(n_allocated);
    if (check < 0.25){
        Thing * temp = new Thing[allocated_length()/2];
        for (size_t i = 0;i<size();++i){
            temp[i] = *(data + i);
        }
        delete [] data;
        n_allocated /= 2;
        data = temp;
    }


}

/**
 * @brief MyVector::front
 * @return A reference to the first item in the array.
 * I will never call this on an empty list.
 */
Thing &MyVector::front()
{
    return *data;
}

/**
 * @brief MyVector::back
 * @return A reference to the last item in the array.
 *
 * Note that this might not be the back of the data buffer.
 * I will never call this on an empty list.
 */
Thing &MyVector::back()
{
    return data[n_items-1];

}
/**
 * @brief MyVector::begin
 * @return A pointer to the first thing.
 */
Thing *MyVector::begin()
{
    return data;
}

/**
 * @brief MyVector::end
 * @return A pointer to the memory address following the last thing.
 */
Thing *MyVector::end()
{
    return data + n_items;
}

/**
 * @brief MyVector::operator []
 * @param i
 * @return A reference to the ith item in the list.
 */
Thing &MyVector::operator[](size_t i)
{
   return data[i];
}

/**
 * @brief MyVector::at
 * @param i
 * @return A reference to the ith item in the list after checking
 * that the index is not out of bounds.
 */
Thing &MyVector::at(size_t i)
{
    if (i > n_items){
        throw std::out_of_range("Requested index out of bounds.");
    }
    return data[i];

}

/**
 * Reallocate the memory buffer to be "new_size" length, using new Thing[new_size]
 * Copy all items from the old buffer into the new one.
 * Delete the old buffer using delete[]
 */
void MyVector::reallocate(size_t new_size)
{
    Thing * temp = new Thing[new_size];
    for (size_t i = 0;i<n_items;i++){
        temp[i] = data[i];
    }
    delete [] data;
    data = temp;
    n_allocated = new_size;
}

