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

#include <cassert>
#include <cstddef>      // for definition of size_t
#include <functional>   // std::function
#include <memory>       // std::unique_ptr
#include <iostream>

template<typename T, typename Comparator = std::less<T>>
class BinarySearchTree
{
public:

	void printDot(std::ostream &o) const{
		o << "digraph G {" << std::endl << root_ -> element_ << "[ label = " << root_ -> element_ << " ];" << std::endl;
		root_ -> printDot(o);
		o << "}" << std::endl;
	}
	//! Does this tree contain the given value?
	bool contains(const T &value)
	{
		return (!empty() and root_->contains(value));
	}

	size_t maxDepth() const
	{
		if (empty())
		{
			return 0;
		}

		return root_->maxDepth();
	}

	//! Is this tree empty?
	bool empty() const
	{
		return (not root_);
	}

	//! Insert a new value into the appropriate place in the tree.
	void insert(T value)
	{
		insert(std::move(value), root_);
	}

	/**
	 * Find the minimum value in the tree.
	 *
	 * @pre   tree is not empty
	 */
	const T& min() const
	{
		assert(root_);
		return root_->min().value();
	}

	/**
	 * Find the minimum value in the tree.
	 *
	 * @pre   tree is not empty
	 */
	const T& max() const
	{
		assert(root_);
		return root_->max().value();
	}

	/**
	 * Remove a value (if it exists) from the tree.
	 *
	 * @returns   whether or not anything was found to remove
	 */
	bool remove(const T &&value)
	{
		return remove(value, root_);
	}

	void print(){
		std::cout<<root_ -> element_ << std::endl;
		root_ -> print();
	}

	void rotateLeft(std::unique_ptr<Node> &node){
		if (!node || !node->right_)
			return;  // Cannot rotate if the node or its right child is null

		std::unique_ptr<Node> pivot = std::move(node->right_);
		node->right_ = std::move(pivot->left_);
		pivot->left_ = std::move(node);
		node = std::move(pivot);
	}
	void rotateRight(std::unique_ptr<Node> &node){
		if (!node || !node->left_)
			return;  // Cannot rotate if the node or its left child is null

		std::unique_ptr<Node> pivot = std::move(node->left_);
		node->left_ = std::move(pivot->right_);
		pivot->right_ = std::move(node);
		node = std::move(pivot);
	}

private:
	struct Node
	{
		// Did you know that structs can have methods too?
		Node(T &&value)
			: element_(value), count_(1)
		{
		}

		const T& value() const { return element_; }

		bool contains(const T& value) const{
			if (value == element_) return true;
			
			if (left_ && value < element_){
				return left_ -> contains(element_);
			}
			else if (right_ && value > element_){
				return right_ -> contains(element_);
			}
			return false;
			}

		void printDot(std::ostream &o) const{
			if (left_){
				o << element_ << " -> " << left_ -> element_ << "[ label = \"L\" ]" << std::endl;
				left_ -> printDot(o);
			}
			if (right_){
				o << element_ << " -> " << right_ -> element_ << "[ label = \"R\" ]" << std::endl;
				right_ -> printDot(o);
			}

		}
		
		Node& min(){
            
            if (left_){
				return left_ -> min();
            }

			else return *this;
        }
		Node& max(){
			
			if (right_){
				return right_ -> max();
            }

			else return *this;
		}
		T takeMin();
		size_t maxDepth() const{
			size_t leftDepth = 0;
			size_t rightDepth = 0;

			if (left_) {
				leftDepth = left_->maxDepth();
			}

			if (right_) {
				rightDepth = right_->maxDepth();
			}

			return 1 + std::max(leftDepth, rightDepth);
		}

		void print(){
			if (left_){
				std::cout<<left_ -> element_<<std::endl;
				left_ -> print();

			}
			if (right_){
				std::cout<<right_ -> element_ << std::endl;
				right_ -> print();
			}

		}

		T element_;
		size_t count_;
		std::unique_ptr<Node> left_;
		std::unique_ptr<Node> right_;
	};

	/**
	 * Internal implementation of recursive insert.
	 *
	 * @param   value      the value to insert
	 * @param   node       the root of the (sub-)tree being inserted into;
	 *                     may be null if the (sub-)tree is empty
	 */

	void insert(T &&value, std::unique_ptr<Node> &node){
        if (!node){
            node = std::make_unique<Node>(std::move(value));
            return;
        }
        
        if (compare_(node -> value(), value)){
            insert(std::move(value), node -> right_);
        }

        else if (compare_(value, node -> value())){
            insert(std::move(value), node -> left_);
        }

        else node -> count_++;
    }

	/**
	 * Internal implementation of recursive removal.
	 *
	 * @param   value      the value to remove
	 * @param   node       the root of the (sub-)tree being inserted into;
	 *                     may be null if the (sub-)tree is empty
	 */
	bool remove(const T &value, std::unique_ptr<Node> &node){
    if (!node) {
        return false;  // Value not found, return false
    }

    if (compare_(value, node->value())) {
        return remove(std::move(value), node->left_);  // Search in the left subtree
    } else if (compare_(node->value(), value)) {
        return remove(std::move(value), node->right_);  // Search in the right subtree
    } else {
        // Value found, handle removal cases
        if (!node->left_) {
            // Case 1: Node has no left child
            node = std::move(node->right_);  // Replace node with its right child
        } else if (!node->right_) {
            // Case 2: Node has no right child
            node = std::move(node->left_);  // Replace node with its left child
        } else {
            // Case 3: Node has both left and right children
            Node* successor = node->right_.get();
            while (successor->left_) {
                successor = successor->left_.get();  // Find the leftmost node in the right subtree
            }
            node->element_ = std::move(successor->element_);  // Replace node's value with successor's value
            return remove(successor->element_, node->right_);  // Recursively remove the successor node
        }
        return true;  // Value removed successfully
    }
}

	Comparator compare_;
	std::unique_ptr<Node> root_;
};

template<typename T, typename Comparator = std::less<T>>
class AVLTree : public BinarySearchTree<T, Comparator>
{
public:
    using BinarySearchTree<T, Comparator>::BinarySearchTree;

    void insert(T value) override
    {
        BinarySearchTree<T, Comparator>::insert(std::move(value), this->root_);
        rebalance(this->root_);
    }

private:
    void rebalance(std::unique_ptr<typename BinarySearchTree<T, Comparator>::Node>& node)
    {
        if (!node)
            return;

        int balanceFactor = getBalanceFactor(node);

        // Left-Left case
        if (balanceFactor > 1 && getBalanceFactor(node->left_) >= 0)
        {
            this->rotateRight(node);
        }
        // Right-Right case
        else if (balanceFactor < -1 && getBalanceFactor(node->right_) <= 0)
        {
            this->rotateLeft(node);
        }

        // No need to check for Left-Right and Right-Left cases when performing insertion

        // Recursively balance the child subtrees
        rebalance(node->left_);
        rebalance(node->right_);
    }

    int getHeight(std::unique_ptr<typename BinarySearchTree<T, Comparator>::Node>& node)
    {
        if (!node)
            return 0;
        return 1 + std::max(getHeight(node->left_), getHeight(node->right_));
    }

    int getBalanceFactor(std::unique_ptr<typename BinarySearchTree<T, Comparator>::Node>& node)
    {
        if (!node)
            return 0;
        return getHeight(node->left_) - getHeight(node->right_);
    }
};