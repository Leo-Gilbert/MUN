#include <iostream>
#include <list>

template<class T>

class queue
{
private:
std::list<T> values;
public:
void enqueue(T value)
{
    values.push_front(value);
}

void dequeue()
{
if(!empty())
    values.pop_front();
}

bool empty(){
return values.empty();
}

void printQ(){
   for (T i : values){
    std::cout<<i<<" ";
   } 
   std::cout << std::endl;
}
};

int main(){
    queue<int> myQ;
    int choice;
    while (true){
        std::cout << "Options:\n";
        std::cout << "1. Enqueue\n";
        std::cout << "2. Dequeue\n";
        std::cout << "3. Quit\n";
        std::cout << "Enter your choice: ";

        std::cin >> choice;

        if (choice == 1){    
        int nextQ;
        std::cout<<"Enter the number you would like to queue: ";
        std::cin >> nextQ;
        std::cout << std::endl;
        myQ.enqueue(nextQ);
        myQ.printQ();
        }
        else if (choice == 2){
            if (!myQ.empty()){
            myQ.dequeue();
            myQ.printQ();
            }
            else std::cout<<"Queue already empty! \n";
        }

        else if (choice == 3){
            break;
        }

        else std::cout << "Invalid choice. /n";
    }


}