#include <iostream>
#include <vector>
#include <list>

template<typename Iter>

void quicksort(const Iter& begin, const Iter& end){
    auto pivot = *begin; // first element in container

    //contianers for container elements with respect to the chosen pivot
    std::list<int> smallerElements;
    std::list<int> equalElements;
    std::list<int> greaterElements;

    if (begin == end){
        return;
    }

    //sorting array elements with respect to pivot
    for (Iter i = begin; i != end; i++) {
        if (*i == pivot){
            equalElements.push_back(*i);
        }
        else if (*i < pivot){
            smallerElements.push_back(*i);
        }
        else if (*i > pivot){
            greaterElements.push_back(*i);
        }
    }

    //recursive calls to sort sub-containers
    quicksort(smallerElements.begin(), smallerElements.end());
    quicksort(greaterElements.begin(), smallerElements.end());

    //splicing sub containers together
    smallerElements.splice(smallerElements.end(), equalElements);
    smallerElements.splice(smallerElements.end(), greaterElements);

    auto newval = smallerElements.begin();

    //replacing original container elements with new container elements
    for (Iter i = begin; i != end; i++){
        *i = *newval;
        advance(newval, 1);
    }

}

int main(){

    //test case
    std::list<int> myList = {1, 5, 23, 352, 32, 1 , 32, 1 ,22, 7, 5, 1};

    quicksort(myList.begin(), myList.end());

    for (int i : myList){
        std::cout<<i<<"\n";
    }


}