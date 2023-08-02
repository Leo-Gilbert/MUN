/*
 * Copyright 2018-2019 Jonathan Anderson
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
#include <memory> 
#include <stack>      // std::unique_ptr
#include <iostream>

template<typename T, typename Comparator = std::less<T>>
class BinarySearchTree
{
public:
	struct Node
	{
		Node(T value) : element_(std::move(value))
		{
		}

		void dot(std::ostream &o) const
		{
			// NOTE: this is slightly more complicated than strictly
			//       necessary (using addresses as names, etc.), but
			//       it will produce valid Dot output even when the
			//       node values are things like money (e.g., $17)

			o
				<< "  \"" << this << "\""
				<< " [ label = \"" << element_ << "\" ];\n";

			if (left_)
			{
				o
					<< "  \"" << this << "\""
					<< " -> "
					<< "\"" << left_.get() << "\""
					<< " [ label = \"L\" ]\n";

				left_->dot(o);
			}

			if (right_)
			{
				o
					<< "  \"" << this << "\""
					<< " -> "
					<< "\"" << right_.get() << "\""
					<< " [ label = \"R\" ]\n";

				right_->dot(o);
			}
		}

		T element_;
		std::unique_ptr<Node> left_;
		std::unique_ptr<Node> right_;
	};
public:
	enum class Traversal
	{
		PreOrder,
		InOrder,
		PostOrder,
	};

	//! Insert a new value into the appropriate place in the tree.
	void insert(T value)
	{
		insert(std::move(value), root_);
	}

	void dot(std::ostream &o) const
	{
		o << "digraph {\n";

		if (root_)
		{
			root_->dot(o);
		}

		o << "}\n";
	}

	/**
	 * An iterator that can traverse the BST in some order.
	 *
	 * The iterator contains a "current" node, a stack of parent nodes and
	 * a Traversal value to remind it which strategy it's following.
	 */
	class Iterator
	{
	public:

	Iterator(typename BinarySearchTree<T, Comparator>::Node* node, Traversal traversal) : travMethod_(traversal), current_(node){}
		/**
		 * Move to the next node in the tree that should be accessed.
		 *
		 * This operator method just calls private methods to try and
		 * keep the logic of the various traversal mechanisms clear.
		 */
		Iterator operator ++ (int){
			
			switch (travMethod_) {
				case Traversal::PreOrder:

					return;

				case Traversal::InOrder:
					return;
					
				case Traversal::PostOrder:
					return;
			}
		}

		//! Dereference the iterator at its current position
		const T& operator * ();

		//! Is this iterator *not* the same as another?
		bool operator != (const Iterator &other);

		// add whatever else you need here
		typename BinarySearchTree<T, Comparator>::Node* current_;
		Traversal travMethod_;
		std::stack<typename BinarySearchTree<T, Comparator>::Node*> nextNodes_;

	};

	//PreOrder - first visit
	//InOrder - Second visit
	//PostOrder - last visit

	Iterator begin(Traversal order){

		if (root_) {
			switch (order) {
				case Traversal::PreOrder: {
					Iterator beginiter = Iterator(root_.get());
					if (root_ -> right_){
						beginiter -> nextNodes_.push(root_ -> right_.get());
					}
					if (root_ -> left_){
						beginiter -> nextNodes_.push(root_ -> left_.get());
					} 
					
					return Iterator(root_);
				}
				case Traversal::InOrder: {
					Iterator temp = Iterator(root_.get());
					while (temp -> left_){
						temp -> nextNodes_.push(temp -> left_.get());
						temp = temp -> left_.get();
					}
					return temp;
				}

				case Traversal::PostOrder: {
					return Iterator(root_);
				}
			}
		}
		return end();
		}
	Iterator end(){
		return Iterator(nullptr, Traversal::InOrder);
	}

	/**
	 * Internal implementation of recursive insert.
	 *
	 * @param   value      the value to insert
	 * @param   node       the root of the (sub-)tree being inserted into;
	 *                     may be null if the (sub-)tree is empty
	 */
	void insert(T &&value, std::unique_ptr<Node> &node)
	{
		if (not node)
		{
			node = std::make_unique<Node>(std::move(value));
		}
		else if (compare_(value, node->element_))
		{
			insert(std::move(value), node->left_);
		}
		else if (compare_(node->element_, value))
		{
			insert(std::move(value), node->right_);
		}
		else
		{
			// assuming no duplicates
		}
	}

	Comparator compare_;
	std::unique_ptr<Node> root_;
};