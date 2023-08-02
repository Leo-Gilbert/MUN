/*
1. This does not necessitate a change to my smart pointer strategy, as each node 
will have only 1 pointer directed towards it.
*/

#include <iostream>
#include <memory>

template<typename T>
class Node{
public:
    T value;
    std::unique_ptr<Node<T>> childptr;
    std::unique_ptr<Node<T>> siblingptr;
    Node(T value) : value(value){}
};

template<typename T>

class Tree{
public:
/**
 * Set the value of the tree's root node.
 */
Tree& setRoot(T value){
    root = std::make_unique<Node<T>>(value);
    return *this;
}

/**
 * Add a leaf node to the top level of this tree.
 */
Tree& addChild(T value){
    //checking if root has a child yet
    if (root -> childptr == nullptr){
        root -> childptr = std::make_unique<Node<T>>(value);
        //printing value to terminal
        std::cout<<root->childptr->value<<std::endl;
    }

    else {
        //traversing the children nodes until you reach a node with no sibling
        //Each row of children functions almost exactly like a linked list
        Node<T>* currentNode = root -> childptr.get();
            while (currentNode -> siblingptr) {
                currentNode = currentNode -> siblingptr.get();
            }
            //setting a new sibling node
            currentNode->siblingptr = std::make_unique<Node<T>>(value);

            std::cout<<currentNode->siblingptr->value<<std::endl;
    }
    return *this;
}

/**
 * Add a subtree to the top level of this tree, using move
 * semantics to "steal" the subtree's nodes.
 */
Tree& addSubtree(Tree<T>&& subtree){
    //if root has no children, setting root of subtree as the first child
    if (root -> childptr == nullptr){
        root -> childptr = std::move(subtree.root);

    }

    else {
        //traversing the siblings until a sibling points to NULL
        Node<T>* currentNode = root -> childptr.get();
            while (currentNode -> siblingptr) {
                currentNode = currentNode -> siblingptr.get();
            }
            //setting the next sibling to the root of the subtree
            currentNode -> siblingptr = std::move(subtree.root);

    }
    return *this;
}

template<typename VisitFn>
void preOrderTraversal(VisitFn visit) const {
    preOrderTraversalHelper(root.get(), visit);
}

template<typename VisitFn>
void postOrderTraversal(VisitFn visit) const {
    postOrderTraversalHelper(root.get(), visit);
}

//recursive helper function which calls visit on current node and current node's child
template<typename VisitFn>
void preOrderTraversalHelper(const Node<T>* node, VisitFn visit) const {
    if (node) {
        visit(node->value);
        preOrderTraversalHelper(node->childptr.get(), visit);
        preOrderTraversalHelper(node->siblingptr.get(), visit);
    }
}

//recusrsive helper function which calls visit on current node and current node's sibling
template<typename VisitFn>
void postOrderTraversalHelper(const Node<T>* node, VisitFn visit) const {
    if (node) {
        postOrderTraversalHelper(node->childptr.get(), visit);
        visit(node->value);
        postOrderTraversalHelper(node->siblingptr.get(), visit);
    }
}

std::unique_ptr<Node<T>> root;

};

//visit function which displays current node information
template<typename T>
void visit(const T &value)
{
	static size_t i = 0;
	std::cout << "Node " << i++ << ": " << value << "\n";
}

int main(){

    Tree<int> tree;
    tree.setRoot(1);
    tree.addChild(2);
    tree.addChild(3);

    Tree<int> subtree;
    subtree.setRoot(4);
    subtree.addChild(5);
    subtree.addChild(9);

    tree.addSubtree(std::move(subtree));

    std::cout << "Pre-order traversal:\n";
    tree.preOrderTraversal(visit<int>);

    std::cout << "\nPost-order traversal:\n";
    tree.postOrderTraversal(visit<int>);
}