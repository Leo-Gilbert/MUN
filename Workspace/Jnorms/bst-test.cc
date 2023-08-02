#include "BST.h"

int main()
{
	BinarySearchTree<int> bst;

    bst.insert(5);
    bst.insert(3);
    bst.insert(1);
    bst.insert(8);
    bst.insert(7);
    bst.insert(6);

    bst.print();

    bst.remove(6);
    
    

	//bst.genTree(10);

	//std::cout << "min value: " << bst.min() << "\n";
	//std::cout << "max value: " << bst.max() << "\n";

	//bst.printDot(std::cout);

    bst.print();


	return 0;
}