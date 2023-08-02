#include <cassert>
#include <cstddef>      // for definition of size_t
#include <functional>   // std::function
#include <memory>
#include <iostream>       // std::unique_ptr

template<typename T, typename Comparator = std::less<T>>
class BinarySearchTree
{
public:
	//! Does this tree contain the given value?
	bool contains(const T &value)
	{
		return (not empty() and root_->contains(value));
	}

    void printDot(std::ostream &o) const{
		o << "digraph G {" << std::endl << root_ -> element_ << "[ label = " << root_ -> element_ << " ];" << std::endl;
		root_ -> printDot(o);
		o << "}" << std::endl;
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

    void print(std::ostream &o){
        root_ -> print(o);
    }

private:
	struct Node
	{
		// Did you know that structs can have methods too?
		Node(T &&value)
			: element_(value), count_(1)
		{
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

		const T& value() const { return element_; }

		bool contains(const T& value) const{
            if (value == element_){
                return true;
            }
            if (left_ && value < element_){
                return left_ -> contains(value);
            }
            else if (right_ && value > element_){
                return right_ -> contains(value);
            }
            return false;
        }

		Node& min(){
            if (!left_){
                return *this;
            }
            else return left_ -> min();
        }
		Node& max(){
            if (!right_){
                return *this;
            }
            else return right_ -> max();
        }

        void print(std::ostream &o){
            o << element_ << std::endl;
            if (left_){
                left_ -> print(o);
            }
            if (right_){
                right_ -> print(o);
            }
        }
		T takeMin();
		size_t maxDepth() const;

        size_t height_ = 0;
        int balance_ = 0;
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

    int height(Node* node) const {
        if (!node){
            return -1;
        }
        else return node -> height_;
    }

	void insert(T &&value, std::unique_ptr<Node> &node){
        if (!node){
            node = std::make_unique<Node>(std::move(value));
        }
        if (compare_(value, node -> element_)){
            insert(std::move(value), node -> left_);
        }

        else if (compare_(node -> element_, value)){
            insert(std::move(value), node -> right_);
        }

        else {
            (node -> count_)++;
        }
        balance(node.get());
    }

    static const int  ALLOWED_IMBALANCE = 1;

    void balance (Node* &node){
        if (!node){
            return;
        }

        if (height(node -> left_) - height(node -> right_) > ALLOWED_IMBALANCE){
            if ((height(node -> left_ -> left_)) >= height(node -> left_ -> right_)){
                rotateWithLeftChild(node.get());
            }
            else doubleWithLeftChild(node.get());
        }

        else {
            if (height(node -> right_.get()) - height(node -> left_.get()) > ALLOWED_IMBALANCE){
                if (height(node -> right_ -> right_) >= height(node -> right_ -> left_)){
                    rotateWithRightChild(node.get());
                }
                else doubleWithRightChild(node.get());
            }
        }
        node -> height_ = max(height(node -> left_.get()), height(node -> right_.get())) + 1;
    }

    void rotateWithLeftChild(Node* &node){
        Node* LC = node -> left_;
        node -> left_ = LC -> right_;
        LC -> right_ = node;
        node -> height_ = max(height(node -> left_.get()), height(node -> right_.get())) + 1;
        LC -> height_ = max(height(LC -> left_.get()), node -> height_.get()) + 1;
        node = LC;
    }

    void doubleWithLeftChild(Node* &node){
        rotateWithLeftChild(node -> left_.get());
        rotateWithLeftChild(node.get());
    }

	/**
	 * Internal implementation of recursive removal.
	 *
	 * @param   value      the value to remove
	 * @param   node       the root of the (sub-)tree being inserted into;
	 *                     may be null if the (sub-)tree is empty
	 */
	bool remove(const T &value, std::unique_ptr<Node> &node){
        if (!node){
            return false;
        }

        if (compare_(value, node -> value())){
            return remove(std::move(value), node -> left_);
        }

         else if (compare_(node -> value(), value)){
            return remove(std::move(value), node -> right_);
         }

         else {
            //value is now found
            //3 cases: 
            //1 - Node has no left child
            //2 - Node has no right child
            //3 - Node has both left and right children

            if (!node -> left_){
                //Case 1
                node = std::move(node -> right_);
            }

            else if (!node -> right_){
                //Case 2
                node = std::move(node -> left_);
            }
            else {
                //Case 3
                //Make successor node
                Node* successor = node -> right_.get();
                while (successor -> left_){
                    //Find the leftmost node of the right subtree
                    successor = successor -> left_.get(); 
                }
                //Replace node's value with successor's value
                node -> element_ = std::move(successor -> element_);
                //Recursively remove the successor node
                return remove(successor -> element_, node -> right_);
            }
            std::cout<<std::endl<<"Value removed! "<<std::endl;
            return true;

         }
         balance(node.get());
    }

	Comparator compare_;
	std::unique_ptr<Node> root_;
};
