#include <cassert>
#include <cstddef>      // for definition of size_t
#include <functional>   // std::function
#include <memory>       // std::unique_ptr
#include <iostream>
#include <cstdlib>
#include <ctime>

template<typename T, typename Comparator = std::less<T>>
class BinarySearchTree
{
public:
	struct Node;
	using NodePtr = std::unique_ptr<Node>;
	//! Does this tree contain the given value?
	bool contains(const T &value)
	{
		return (not empty() and root_->contains(value));
	}

	void genTree(int n)
	{	
		srand(time(NULL));
		for (int i = 0; i < n; ++i)
		{	
			int x = rand() % 100;
			insert (x);
		}
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
		return ( root_ == nullptr);
	}

	//! Insert a new value into the appropriate place in the tree.
	void insert(T value)
	{
		insert(std::move(value), root_);
	}

	void print() const;

	void printDot(std::ostream &o) const
	{	
		o << "digraph G {" << "\n";
		root_->printDot(o);
		o << "}";
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

	struct Node
	{
		// Did you know that structs can have methods too?
		Node(T &&value)
			: element_(value), count_(1)
		{
		}

		const T& value() const { return element_; }

		bool contains(const T& value) const
		{	
			if (value == element_)
			{
				return true;
			}
			else if(value < element_ && this->left_ != nullptr)
			{
				this->left_->contains(value);
			}
			else if(element_ < value && this->right_ != nullptr)
			{
				this->right_->contains(value);
			}
			else
			{
				return false;
			}
		}

		Node& min()
        {
            if (left_ != nullptr)
            {
                return left_->min();
            }
            else
            {
                return *this;
            }   
        }

		Node& max()
        {
            if (right_ != nullptr)
            {
                return right_->max();
            }
            else
            {
                return *this;
            } 
        }

		static NodePtr takeMin(NodePtr& n)
		{
		if (n->left_ != nullptr)
		{
			return takeMin(n->left_);
		}
		else
		{	
			NodePtr temp = std::move(n);

			if (n->right_ != nullptr)
			{
				n = std::move(n->right_);
			}

			return temp;
		}
		}	

		size_t maxDepth() const
		{	
			if(this == nullptr)
			{
				return 0;
			}
			if(this->right_ == nullptr && this->left_ == nullptr)
			{
				return 1;
			}
			
			size_t rightDepth_ = 0;
			size_t leftDepth_ = 0;

			if(this->right_ != nullptr)
			{
				rightDepth_ = 1 + this->right_->maxDepth();
			}
			if(this->left_ != nullptr)
			{
				leftDepth_ = 1 + this->left_->maxDepth();
			}

			if(rightDepth_ < leftDepth_)
			{
				return leftDepth_;
			}
			else
			{
				return rightDepth_;
			}
		}

        void print() const
        {
            if (left_)
            {
                left_->print();
            }

			if (count_ > 1)
			{
				std::cout << element_ << "-" << count_ << " ";
			}
			else
			{
				std::cout << element_ <<  " ";
			}

            if (right_)
            {
                right_->print();
            }
        }

		void printDot(std::ostream& o) const
        {	
			if (count_ > 1)
			{
				o << element_ << " [ label = \"" << element_ << " - " << count_ << "\" ];" << std::endl;
			}
			else
			{
				o << element_ << " [ label = \"" << element_ << "\" ];" << std::endl;
			}
			if (left_)
            {
                o << element_ << " -> " << left_->element_ << "[ label = \"L\" ];" << std::endl;
            }

            if (right_)
            {
                o << element_ << " -> " << right_->element_ << "[ label = \"R\" ];" << std::endl;
            }

			o << std::endl;

            if (left_)
            {
                left_->printDot(o);
            }

            if (right_)
            {
                right_->printDot(o);
            }
        }

		int balanceFactor() const
		{	
			int leftFac = 0;
			int rightFac = 0;

			if (this == nullptr)
			{
				return 0;
			}

			if (this->left_ != nullptr)
			{
				leftFac = this->left_->maxDepth();
			}
			if (this->right_ != nullptr)
			{
				rightFac = this->right_->maxDepth();
			}

			return rightFac - leftFac;
		}

		static void rotateRight(NodePtr& node)
		{
			NodePtr temp = std::move(node);
			node = std::move(temp->left_);

			if (node->right_ != nullptr)
			{
				temp->left_ = std::move(node->right_);
			}
			node->right_ = std::move(temp);
			node->balFac = node->balanceFactor();
		}

		static void rotateLeft(NodePtr& node)
		{
			NodePtr temp = std::move(node); //
			node = std::move(temp->right_);

			if (node->left_ != nullptr)
			{
				temp->right_ = std::move(node->left_);
			}
			node->left_ =  std::move(temp);
			node->balFac = node->balanceFactor();
		}

		size_t maxDepth_ = 0;
		int balFac = 0;
		
		T element_;
		size_t count_ = 0;
		NodePtr left_ = nullptr;
		NodePtr right_ = nullptr;

		bool movedLeft;
	};

	/**
	 * Internal implementation of recursive insert.
	 *
	 * @param   value      the value to insert
	 * @param   node       the root of the (sub-)tree being inserted into;
	 *                     may be null if the (sub-)tree is empty
	 */
	void insert(T &&value, NodePtr &node)
	{	
		if (node == nullptr)
		{
			node = std::make_unique<Node>(std::move(value));
		}
		else if (compare_(value, node->value()))
		{
			node->movedLeft = true;
			insert(std::move(value), node->left_);
		}
		else if (compare_(node->value(), value))
		{
			node->movedLeft = false;
			insert(std::move(value), node->right_);
		}
		else
		{
			++node->count_;
		}

		node->balFac = node->balanceFactor();

		if(node->balFac == 2)
		{
			if(node->right_->movedLeft == false)
			{
				Node::rotateLeft(node);
			}
			else
			{
				Node::rotateRight(node->right_);
				Node::rotateLeft(node);
			}		
		}
		else if(node->balFac == -2)
		{
			if(node->left_->movedLeft == true)
			{
				Node::rotateRight(node);
			}
			else
			{
				Node::rotateLeft(node->left_);
				Node::rotateRight(node);
			}
		}	
	}

	/**
	 * Internal implementation of recursive removal.
	 *
	 * @param   value      the value to remove
	 * @param   node       the root of the (sub-)tree being inserted into;
	 *                     may be null if the (sub-)tree is empty
	 */
	bool remove(const T &value, NodePtr &node)
	{
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
	}

	Comparator compare_;
	NodePtr root_;
};

template<typename T, typename Comparator>
void BinarySearchTree<T, Comparator>::print() const
{
    if (root_)
    {
        root_->print();
    }
}