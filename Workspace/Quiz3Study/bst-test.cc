#include "BST.h"

int main()
{
	BinarySearchTree<int> bst;

	bst.insert(3);
	bst.insert(2);
	bst.insert(1);

	std::cout << "min value: " << bst.min() << "\n";
	std::cout << "max value: " << bst.max() << "\n";

	bst.print(std::cout);

	//std::cout << bst.contains(5);

	//bst.remove(7);

	bst.printDot(std::cout);


	//std::cout << bst.contains(5);

	return 0;
}