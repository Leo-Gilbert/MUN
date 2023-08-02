/*
 * Copyright 2018 Jonathan Anderson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "BST.h"

int main()
{
	BinarySearchTree<int> bst;

	bst.insert(7);
	bst.insert(3);
	bst.insert(9);
	bst.insert(1);
	bst.insert(8);
	bst.insert(5);
	bst.insert(100);
	bst.insert(6);
	bst.insert(4);

	std::cout << "min value: " << bst.min() << "\n";
	std::cout << "max value: " << bst.max() << "\n";

	bst.print();
	bst.printDot(std::cout);
	bst.rotateLeft(bst.root_);
	bst.rotateRight(bst.root_);
	//std::cout<<bst.contains(4)<<std::endl;

	//std::cout<<bst.maxDepth()<<std::endl;

	//std::cout<<bst.remove(7)<<std::endl;

	//bst.print();

	return 0;
}