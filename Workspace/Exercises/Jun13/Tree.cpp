/*
1. by using a shared_pointer, a root tree node is able to access any child node on any level within the tree, meaning
that it is possible to keep the child nodes of a node that is being deleted. With a unique pointer, deleting a node will
mean all of its child nodes will also be deleted.
*/

#include <iostream>
#include <vector>
#include <memory>

template<typename T>
class Node{
public:
    T value;
    std::vector<std::unique_ptr<Node<T>>> children;
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
    root -> children.emplace_back(std::make_unique<Node<T>>(value));
    return *this;
}

/**
 * Add a subtree to the top level of this tree, using move
 * semantics to "steal" the subtree's nodes.
 */
Tree& addSubtree(Tree<T>&& subtree){
    root -> children.emplace_back(std::move(subtree.root));
    return *this;
}

std::unique_ptr<Node<T>> root;

};

int main(){
    //test case
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
