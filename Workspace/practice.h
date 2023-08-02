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
			if (value == element_){
                return true;
            }

            if (left_ && value < element_){
                left_ -> contains(value);
            }

            else if (right_ && element_ < value){
                right_ -> contains(value);
            }
            else return false;
		}

		Node& min()
        {
            if (left_){
                return left_ -> min();
            }
            else return *this;
        }

		Node& max()
        {
            if (right_){
                return right_ -> max();
            }
            else return *this;
        }

		static NodePtr takeMin(NodePtr& n)
		{
		
		}	

		size_t maxDepth() const
		{	
			size_t leftDepth = 0;
            size_t rightDepth = 0;
            if (!this){
                return 0;
            }

            if(this->right_ == nullptr && this->left_ == nullptr)
			{
				return 1;
			}

            if (right_){
                rightDepth = right_ -> maxDepth();
            }
            if (left_){
                leftDepth = left_ -> maxDepth();
            }

            return std::max(rightDepth, leftDepth);
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

		}

		static void rotateRight(NodePtr& node)
		{
			std::unique_ptr<Node> temp = std::move(node);
            node = std::move(temp -> left_);

            if (node -> right_){
                temp -> left_ = std::move(node -> right_);
            }

            node -> right_ = std::move(temp);
            node -> balance_ = node -> balanceFactor();

		}

		static void rotateLeft(NodePtr& node)
		{
			std::unique_ptr<Node> temp = std::move(node);
            node = std::moove(temp -> right_);

            if (node -> left_){
                temp -> right_ = std::move(node -> left_);
            }

            node -> left_ = std::move(temp);
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
        if (!node){
            node = std::make_unique<Node>(std::move(value));
        }

        else if (compare_(value, node -> element_)){
            insert(std::move(value), node -> left_);
        }

        else if (compare_(node -> element_, value)) {
            insert(std::move(value), node -> right_);
        }
        else node -> count_++
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
            //value not found, no removals made
            return false;
        }

        else if (compare_(value, node -> element_)){
            return remove(std::move(value), node -> left_);
        }

        else if (compare_(node -> element_, value)){
            return remove(std::move(value), node -> right_);
        }

        else {
            //value found

            //node has no left pointer
            //node has no right pointer
            //node has both left and right pointers 

            if (!node -> left_){
                node = std::move(node -> right_)
            }

            else if (!node -> right_){
                node = std::move(node -> left_)
            }

            else {
                Node* suc = node -> right_.get();

            }
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

