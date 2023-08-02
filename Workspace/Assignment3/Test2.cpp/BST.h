
#include <cassert>
#include <cstddef>      // for definition of size_t
#include <functional>   // std::function
#include <memory>       // std::unique_ptr
#include <iostream>
#include <stack>
#include <ostream>


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

    enum class Traversal
    {
        PreOrder,
        InOrder,
        PostOrder,
    };

    class Iterator
    {
    public:
        Iterator(Node* node, Traversal traversal, std::stack<Node*> nextnodes) : travMethod_(traversal), current_(node), nextNodes_(nextnodes){}
        
        Iterator operator++(int) {
            if (travMethod_ == Traversal::PreOrder) {
                if (current_ -> left_){
                    nextNodes_.push(current_);
                    current_ = current_ -> left_.get();
                }

                else if (current_ -> right_){
                    
                    current_ = current_ -> right_.get();
                }
                else {
                    if (nextNodes_.empty()){
                        current_ = nullptr;
                    }
                    else{
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

            else if (travMethod_ == Traversal::InOrder) {

                if (current_ -> right_){
                    current_ = current_ -> right_.get();
                    while (current_ -> left_){
                        nextNodes_.push(current_);
                        current_ = current_ -> left_.get();

                    }
                }

                else {
                    if (!nextNodes_.empty()){
                        current_ = nextNodes_.top();
                        nextNodes_.pop();
                    }
                    else current_ = nullptr;
                }

                return *this;

            } 

            else if (travMethod_ == Traversal::PostOrder) {
                previous_ = current_;
                if (current_ -> right_){
                    current_ = current_ -> right_.get();
                    while (current_ -> left_){
                        nextNodes_.push(current_);
                        current_ = current_ -> left_.get();
                    }
                }

            return *this;
        }
        }


        const T& operator*() const
        {
            return current_->element_;
        }

        bool operator!=(const Iterator& other) const
        {
            return current_ != other.current_;
        }


    private:
        Node* current_;
        Traversal travMethod_;
        std::stack<Node*> nextNodes_;
        Node* previous_;
    };

    void insert(T value)
    {
        insert(std::move(value), root_);
    }

    void dot(std::ostream& o) const
    {
        o << "digraph {\n";

        if (root_)
        {
            root_->dot(o);
        }

        o << "}\n";
    }

    Iterator begin(Traversal order){
        if (order == Traversal::PreOrder){
            //if (root_){}
            std::stack<Node*> dummy;
            return Iterator(root_.get(), Traversal::PreOrder, dummy);
        }

        else if (order == Traversal::InOrder){
            if (root_){
                std::stack<Node*> nextnodes;
                Node* current = root_.get();
                while (current -> left_){
                    nextnodes.push(current);
                    current = current -> left_.get();
                }
                return Iterator(current, Traversal::InOrder, nextnodes);
                    
            }
        }
        else if (order == Traversal::PostOrder){
            if (root_){
                std::stack<Node*> nextnodes;
                Node* current = root_.get();
                bool rootright = true;
                while (current -> left_){
                    nextnodes.push(current);
                    current = current -> left_.get();
                }
                return Iterator(current, Traversal::InOrder, nextnodes);
                    
            }
        }
}

    Iterator end(){
        std::stack<Node*> dummy;
        return Iterator(nullptr, Traversal::PreOrder, dummy);
    }

private:
    void insert(T&& value, std::unique_ptr<Node>& node)
    {
        if (!node)
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
