#include <iostream>

using namespace std;
template<class T>

class node {
public:
//value and pointer to next node contained in each node
T value;
node* nextptr;
//constructor which sets value to passed parameter and nextpointer to null pointer
node(T value) : value(value), nextptr(nullptr){}
};

template<class T>
class linkedList{
private:
//values for head node and size of list
node<T>* head;
int size;
public:
//constructor which sets the head node as a nulll pointer and size to 0
linkedList() : head(nullptr), size(0) {}

void insertNode(T value){
    node<T> *newNode = new node(value);
    if (head == nullptr){
        head = newNode;
    }
    else{
        node<T>* currentNode = head;
        while (currentNode -> nextptr != nullptr){
            currentNode = currentNode -> nextptr;
        }
        currentNode -> nextptr = newNode;
    }
    size++;
}

void printList(){
    node<T>* currentNode = head;
    if (head == nullptr){
        cout << "Empty List! " << endl;
    }
    else {
        int nodenum = 1;
        while (currentNode != nullptr){
            cout << "Node #" << nodenum<< " " << currentNode -> value << endl;
            nodenum++;
            currentNode = currentNode -> nextptr;
        }
    }

}

};

int main(){
    //test case
    linkedList<string> myList;
    myList.insertNode("slajda");
    myList.insertNode("dog");
    myList.insertNode("cat");
    myList.printList();

}