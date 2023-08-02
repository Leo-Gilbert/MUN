#include <iostream>
#include <memory>

template<typename T>
class Node {
public:
    T value;
    std::unique_ptr<Node<T>> child;
    std::unique_ptr<Node<T>> sibling;

    Node(T value) : value(value), child(nullptr), sibling(nullptr) {}
};

template<typename T>
class Tree {
public:
    /**
     * Set the value of the tree's root node.
     */
    Tree& setRoot(T value) {
        root = std::make_unique<Node<T>>(value);
        return *this;
    }

    /**
     * Add a leaf node to the top level of this tree.
     */
    Tree& addChild(T value) {
        if (!root->child) {
            root->child = std::make_unique<Node<T>>(value);
        } else {
            Node<T>* currentNode = root->child.get();
            while (currentNode->sibling) {
                currentNode = currentNode->sibling.get();
            }
            currentNode->sibling = std::make_unique<Node<T>>(value);
            std::cout<<currentNode->sibling->value<<std::endl;
        }
        return *this;
    }

    /**
     * Add a subtree to the top level of this tree, using move
     * semantics to "steal" the subtree's nodes.
     */
    Tree& addSubtree(Tree<T>&& subtree) {
        if (!root->child) {
            root->child = std::move(subtree.root);
        } else {
            Node<T>* currentNode = root->child.get();
            while (currentNode->sibling) {
                currentNode = currentNode->sibling.get();
            }
            currentNode->sibling = std::move(subtree.root);
        }
        return *this;
    }

    std::unique_ptr<Node<T>> root;
};

int main() {
    Tree<int> tree;
    tree.setRoot(1);
    tree.addChild(2);
    tree.addChild(3);

    //tree.addSubtree(std::move(subtree));

    // Traverse the tree and print the values
    Node<int>* currentNode = tree.root.get();
    std::cout << currentNode -> value << std::endl;
    if (currentNode -> child) {
        currentNode = currentNode -> child.get();
        std::cout << currentNode -> value << std::endl;
        while (currentNode -> sibling) {
            currentNode = currentNode -> sibling.get();
            std::cout << currentNode -> value << std::endl;
        }
    }

    return 0;
}