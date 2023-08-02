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

std::unique_ptr<Node<T>> root;

};

int main(){

    Tree<int> tree;
    tree.setRoot(1);
    tree.addChild(2);
    tree.addChild(3);

    Tree<int> subtree;
    subtree.setRoot(1);
    subtree.addChild(4);
    subtree.addChild(9);

    tree.addSubtree(std::move(subtree));
}