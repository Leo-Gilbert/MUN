
/*
 * Copyright 2018 Jonathan Anderson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdint>
#include <memory>

template<typename T>
class Node
{
public:

T value;
Node<T>* nextptr;
Node(T value) : value(value), nextptr(nullptr){}

};


template<typename T>
class List
{
public:
	//! An iterator over the list
	class iterator
	{
	public:

        iterator(Node<T>* current) : current(current){}
		/**
		 * The dereference operator.
		 *
		 * @returns   a reference to the "current" element
		 */
		T& operator*(){
			return current -> value;
		}

		/**
		 * Pre-increment operator (i.e., `++i`).
		 *
		 * This method increments the iterator and then returns a
		 * reference to the newly-incremented iterator.
		 *
		 * @returns   a reference to this iterator, after incrementing
		 */
		iterator& operator++(){
			current = current -> nextptr;
			return *this;
		}

		/**
		 * Post-increment operator (i.e., `i++`).
		 *
		 * This method returns a copy of this iterator as it currently
		 * is (i.e., pointing where it currently points) and _then_
		 * increments itself.
		 *
		 * @param     ignored   this is only used to distinguish the two
		 *                      increment operators (pre- and post-)
		 *                      from each other: its value should be
		 *                      ignored
		 *
		 * @returns   an iterator to the previously-current element
		 */
		iterator operator++(int ignored){
			Node<T>* temp = current;
			current = current -> nextptr;
			return temp; 
		}

		//! Is this iterator pointing at the same place as another one?
		bool operator== (const iterator& same) const{
			if (same.current == current){
				return true;
			}
			else return false;
		}

		//! Is this iterator pointing at a different place from another?
		bool operator!= (const iterator& same) const{
			if (same.current != current){
				return true;
			}
			else return false;
		}

        Node<T>* current;
	};


	//! Default constructor
	List() : head(nullptr), tail(nullptr), size_(0) {}

	//! Copy constructor
	List(const List& list) : head(nullptr), tail(nullptr), size_(0) {
    Node<T>* current = list.head;
    while (current != nullptr) {
        push_back(current->value);
        current = current->nextptr;
    }
}

	//! Move constructor
	List(List&& list){
		head = list.head;
		tail = list.tail;
		list.head = NULL;
		list.tail = NULL;
		size_ = list.size_;
		list.size_ = 0;
	}

	//! Destructor
	~List(){
		
		Node<T>* current = head;
		while (current != nullptr){
			Node<T>* next = current -> nextptr;
			delete current;
			current = next;
		}

		head = nullptr;
		tail = nullptr;
		size_ = 0;

	}

	//! Copy assignment operator
	List& operator= (const List& list){
    if (this != &list)
    {
        // Delete existing nodes
        Node<T>* current = head;
        while (current != nullptr)
        {
            Node<T>* next = current->nextptr;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size_ = 0;

        // Copy elements from the source list
        current = list.head;
        while (current != nullptr)
        {
            push_back(current->value);
            current = current->nextptr;
        }
    }
    return *this;
}
	//! Move assignment operator
	List& operator=(List&& list) {
    if (this != &list) {
        // Delete existing nodes
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* next = current->nextptr;
            delete current;
            current = next;
        }

        // Move elements from the source list
        head = list.head;
        tail = list.tail;
        size_ = list.size_;

        // Reset the source list
        list.head = nullptr;
        list.tail = nullptr;
        list.size_ = 0;
    }
    return *this;
}


	//
	// Accessors:
	//
	//! How many elements are in this list?
	size_t size() const{
        return size_;
    }

	//! Is this list empty?
	bool empty() const{
        if (head == nullptr){
            return true;
        }
        else return false;
    }

	//! Get an iterator to the beginning of the list
	iterator begin(){
        return iterator(head);
    }

	//! Get an iterator just past the end of the list
	iterator end(){
        return iterator(tail -> nextptr);
    }


	//
	// Mutators:
	//
	//! Copy an element to the front of the list
	void push_front(const T& value_)
	{
		Node<T>* newhead = new Node<T>(value_);
		newhead -> value = value_;
		if (empty()){
			head = newhead;
			tail = head;
		}
		else{
			newhead -> nextptr = head;
			head = newhead;
		}
		size_++;
        
    }

	//! Move an element to the front of the list
	void push_front(T&& value_){
		Node<T>* newhead = new Node<T>*(std::move(value_));
		if (empty()){
			head = newhead;
			tail = head;
		}
		else{
			newhead -> nextptr = head;
			head = newhead;
		}
		size_++;
	}

	//! Copy an element to the back of the list
	void push_back(const T& value_){
		Node<T>* newtail = new Node<T>(value_);
		if (empty()){
			head = newtail;
			tail = head;
		}
		else {
			tail -> nextptr = newtail;
			tail = newtail;
		}
		size_++;
	}

	//! Add an element to the back of the list
	void push_back(T&& value_){
		Node<T>* newtail = new Node<T>(std::move(value_));
		if (empty()){
			head = newtail;
			tail = head;
		}
		else {
			tail -> nextptr = newtail;
			tail = newtail;
		}
		size_++;
	}

	/**
	 * Copy an element into an arbitrary location.
	 *
	 * This method makes a copy of an element and inserts that copy into a
	 * location pointed at by the given iterator. After insertion into a
	 * list of n elements, the list should contain n+1 elements (i.e., no
	 * existing element should be replaced).
	 *
	 * @returns   an iterator pointing at the newly-inserted element
	 */
	iterator insert(iterator index, const T& value_){
		Node<T>* newnode = new Node<T>(value_);
		if (index == NULL){
			tail -> nextptr = newnode;
			tail = newnode;
		}
		else if (index == head) {
		
			newnode -> nextptr = head;
			head = newnode;
		}
		else {
        // Insert in the middle of the list
        Node<T>* next = index.current;
        Node<T>* prev = head;
        while (prev->nextptr != next) {
            prev = prev->nextptr;
        }
        prev->nextptr = newnode;
        newnode->nextptr = next;
    }

		size_++;

		return iterator(newnode);
	}

	/**
	 * Move an element into an arbitrary location.
	 *
	 * This method inserts an element into a location pointed at by the
	 * given iterator, using move semantics to avoid copies. After insertion
	 * into a list of n elements, the list should contain n+1 elements
	 * (i.e., no existing element should be replaced).
	 *
	 * @returns   an iterator pointing at the newly-inserted element
	 */
	
	iterator insert(iterator index, T&& value_){
		Node<T>* newnode = new Node<T>(std::move(value_));
		if (index == NULL){
			tail -> nextptr = newnode;
			tail = newnode;
		}
		else if (index == head) {
		
			newnode -> nextptr = head;
			head = newnode;
		}
		else {
        // Insert in the middle of the list
        Node<T>* next = index.current;
        Node<T>* prev = head;
        while (prev->nextptr != next) {
            prev = prev->nextptr;
        }
        prev->nextptr = newnode;
        newnode->nextptr = next;
    }

		size_++;

		return iterator(newnode);

	}

	//! Remove an element from an arbitrary location
	void erase(iterator deletion){

		Node<T>* del = deletion.current;
		Node<T>* current = head;
		while (current -> nextptr != del){
			current = current ->nextptr;
		}
		current -> nextptr = del -> nextptr;
		delete del;
		size_--;

	}

private:
	// Add whatever you need to add here
    Node<T>* head;
    Node<T>* tail;
    size_t size_;

};