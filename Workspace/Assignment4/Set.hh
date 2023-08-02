/*
 * Copyright 2020, 2023 Jonathan Anderson
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

#ifndef SET_H_
#define SET_H_

#include <cassert>      // for assert()
#include <cstddef>      // for definition of size_t
#include <utility>      // for std::move()    // for definition of size_t
#include <functional>   // std::function
#include <memory>     // std::unique_ptr
#include <iostream>
#include <stack>
//
// This preprocessor definition expands to a function
// body that contains an assertion that will provoke
// a run-time error. You can remove UNIMPLEMENTED from
// each method as you implement it, and remove the
// following line once you've implemented everything.
//

/**
 * A set that holds values, ignoring duplicates.
 */
template<typename T, typename Comparator = std::less<T>>
class Set
{
public:

    struct Node
        {
            Node(T value) : element_(std::move(value)){}
            
            bool contains(const T& value) const{
                if (value == element_) return true;
                
                if (left_ && value < element_){
                    return left_ -> contains(value);
                }
                else if (right_ && value > element_){
                    return right_ -> contains(value);
                }
                return false;
			}

            T element_;
            std::shared_ptr<Node> left_;
            std::shared_ptr<Node> right_;
        };

	//! Default constructor
	Set()
    {
        root_ = nullptr;
    }

	//! Copy constructor
	Set(const Set& setToCopy)
    {
        root_ = copyTree(setToCopy.root_.get());
        
    }

	// Move constructor
	Set(Set&& setToMove)
    {
        root_ = std::move(setToMove.root_);
    }

	//! Add an element to this set.
	void insert(T value){
        insert(std::move(value), root_);
    }

    //recursive imlpementation of the insert method
    void insert(T &&value, std::shared_ptr<Node> &node){
        if (!node){
            node = std::make_shared<Node>(std::move(value));
            return;
        }
        
        if (compare_(node -> element_, value)){
            insert(std::move(value), node -> right_);
        }

        else if (compare_(value, node -> element_)){
            insert(std::move(value), node -> left_);
        }

        else return;
    }

	//! Is this set the empty set?
	bool empty() const {
        if (root_ ==  nullptr){
            return true;
        }
        else return false;
    }

	//! Is a specified element already in the set?
	bool contains(const T& value) const {

        return (!empty() and root_ -> contains(value));
    }

	//! Compute our intersection with another set.
	Set<T> intersection(const Set<T>& otherSet) {
        Set<T> result; // Create a new set to store the intersection

        // Call a recursive function to compute the intersection
        findIntersection(root_.get(), otherSet.root_.get(), result.root_);

        return result;
    }

	//! Compute our union with another set.
	Set<T> setUnion(const Set<T>& otherSet) {
        Set<T> result; // Create a new set to store the union

        // Call a recursive function to compute the union
        findUnion(root_.get(), otherSet.root_.get(), result.root_);

        return result;
    }

	//! An iterator over the set's values that promises not to change them.
	struct ConstIterator
	{
        ConstIterator(Node* node, std::stack<Node*> nextnodes) : current_(node), nextNodes_(nextnodes){}

        bool operator==(const ConstIterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }

		const T& operator*() const 
        {   
            if (current_ != nullptr){
                return current_ -> element_;
            }

        }

        //pre order traversal
		ConstIterator& operator++() {
            if (current_ -> left_)
                {
                    nextNodes_.push(current_);
                    current_ = current_ -> left_.get();
                }

                else if (current_ -> right_){
                    
                    current_ = current_ -> right_.get();
                }
                else {
                    if (nextNodes_.empty())
                    {
                        current_ = nullptr;
                    }

                    else
                    {
                        Node* previous = nextNodes_.top();
                        while (!(previous -> right_)){
                            nextNodes_.pop();
                            previous = nextNodes_.top();
                        }

                        nextNodes_.pop();
                        current_ = previous -> right_.get();
                    }
                }

                return *this;

            }
        
        Node* getCurrent(){
        return current_;
    }

    ConstIterator& operator++(int) 
    {
        ConstIterator temp = *this;
        ++(*this);
        return temp;
    }

    private:
        Node* current_;
        std::stack<Node*> nextNodes_;

	};

	//! An iterator over the set's values that *is* allowed to change values.
	struct Iterator
	{
        Iterator(Node* node, std::stack<Node*> nextnodes) : current_(node), nextNodes_(nextnodes){}


        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

		T& operator*() const 
        {
            if (current_ != nullptr){
                return current_ -> element_;
            }
        }
		Iterator& operator++() {
            //pre order traversal method
            if (current_ -> left_)
                {
                    nextNodes_.push(current_);
                    current_ = current_ -> left_.get();
                }

                else if (current_ -> right_){
                    
                    current_ = current_ -> right_.get();
                }
                else {
                    if (nextNodes_.empty())
                    {
                        current_ = nullptr;
                    }

                    else
                    {
                        Node* previous = nextNodes_.top();
                        while (!(previous -> right_)){
                            nextNodes_.pop();
                            previous = nextNodes_.top();
                        }

                        nextNodes_.pop();
                        current_ = previous -> right_.get();
                    }
                }

                return *this;

            }
    
    Iterator& operator++(int) 
    {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    Node* getCurrent(){
        return current_;
    }

    private:
        Node* current_;
        std::stack<Node*> nextNodes_;
	};

	// Iterators over const and non-const sets:
	Iterator begin() {
        if (root_){
            return Iterator(root_.get(), std::stack<Node*>());
        }
        else return Iterator(nullptr, std::stack<Node*>());
    }
	ConstIterator begin() const {
        if (root_){
            return ConstIterator(root_.get(), std::stack<Node*>());
        }
        else return ConstIterator(nullptr, std::stack<Node*>());
    }

	Iterator end() {
        return Iterator(nullptr, std::stack<Node*>());
    }

	ConstIterator end() const {
        return ConstIterator(nullptr, std::stack<Node*>());
    }

	/**
	 * Look up a specific value within this set.
	 *
	 * This should return an iterator that is equal to end() if the value
	 * is not found.
	 */
	Iterator find(const T& value) {
        Iterator current = Iterator(root_.get(), std::stack<Node*>());
        while (current.getCurrent() != nullptr)
        {
            if (*current == value){
                return current;
            }

            ++current;

        }

        return end();

    }

	ConstIterator find(const T& value) const {
        ConstIterator current = ConstIterator(root_.get(), std::stack<Node*>());
        while (current.getCurrent() != nullptr)
        {
            if (*current == value){
                return current;
            }

            ++current;

        }

        return end();
    }

private:

    void findUnion(const Node* node1, const Node* node2, std::shared_ptr<Node>& resultNode) const {
        if (!node1 && !node2) {
            return; // Both trees are empty, no union to compute
        }

        // Insert the element from the first tree into the result set
        if (node1 && (!node2 || compare_(node1->element_, node2->element_))) {
            resultNode = std::make_shared<Node>(node1->element_);
            findUnion(node1->left_.get(), node2, resultNode->left_);
            findUnion(node1->right_.get(), node2, resultNode->right_);
        }
        // Insert the element from the second tree into the result set
        else if (node2 && (!node1 || compare_(node2->element_, node1->element_))) {
            resultNode = std::make_shared<Node>(node2->element_);
            findUnion(node1, node2->left_.get(), resultNode->left_);
            findUnion(node1, node2->right_.get(), resultNode->right_);
        }
        // Both nodes have the same element, insert it into the result set and check subtrees
        else {
            resultNode = std::make_shared<Node>(node1->element_);
            findUnion(node1->left_.get(), node2->left_.get(), resultNode->left_);
            findUnion(node1->right_.get(), node2->right_.get(), resultNode->right_);
        }
    }

    void findIntersection(const Node* node1, const Node* node2, std::shared_ptr<Node>& resultNode) const {
    if (!node1 || !node2) {
        return; // One of the trees is empty, no intersection to compute
    }

    // If the current elements are the same, insert it into the result set
    if (node1->element_ == node2->element_) {
        resultNode = std::make_shared<Node>(node1->element_);
        findIntersection(node1->left_.get(), node2->left_.get(), resultNode->left_);
        findIntersection(node1->right_.get(), node2->right_.get(), resultNode->right_);
    }
    // If the current element in the first tree is smaller, move to the right subtree of the first tree
    else if (compare_(node1->element_, node2->element_)) {
        findIntersection(node1->right_.get(), node2, resultNode);
    }
    // If the current element in the second tree is smaller, move to the right subtree of the second tree
    else {
        findIntersection(node1, node2->right_.get(), resultNode);
    }
}

    std::shared_ptr<Node> copyTree(Node* node)
    {
        if (node == nullptr) {
            return nullptr;
        }

        // Create a new node for the current element
        std::shared_ptr<Node> newNode = std::make_shared<Node>(node->element_);

        // Recursively copy the left and right subtrees
        newNode->left_ = copyTree(node->left_.get());
        newNode->right_ = copyTree(node->right_.get());

        return newNode;
    }

    Comparator compare_;
    std::shared_ptr<Node> root_;

};

#endif