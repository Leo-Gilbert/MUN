#include <iostream>
#include <vector>
#include <memory>

template<typename T>
class Tree {
private:
    struct Node {
        T data;
        std::vector<std::unique_ptr<Node>> children;

        explicit Node(const T& value) : data(value) {}
    };

    std::unique_ptr<Node> root;

public:
    Tree& setRoot(T value) {
        root = std::make_unique<Node>(value);
        return *this;
    }

    Tree& addChild(T value) {
        if (!root) {
            root = std::make_unique<Node>(value);
        } else {
            root->children.emplace_back(std::make_unique<Node>(value));
        }
        return *this;
    }

    Tree& addSubtree(Tree<T>&& subtree) {
        if (!root) {
            root = std::move(subtree.root);
        } else {
            root->children.emplace_back(std::move(subtree.root));
        }
        return *this;
    }
};

int main() {
    Tree<int> tree;
    tree.setRoot(1);
    tree.addChild(2);
    tree.addChild(3);

    Tree<int> subtree;
    subtree.setRoot(4);
    subtree.addChild(5);
    subtree.addChild(6);

    tree.addSubtree(std::move(subtree));
    tree.addChild(7);

    return 0;
}