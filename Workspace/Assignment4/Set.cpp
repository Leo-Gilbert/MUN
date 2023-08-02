#include <iostream>
#include "Set.hh" // Assuming your Set class is defined in Set.h

int main() {
    Set<int> integers; // Creating a set of integers

    // Insert some numbers into the set
    integers.insert(10);
    integers.insert(20);
    integers.insert(30);
    integers.insert(40);
    integers.insert(50);

    // Test the contains method for each number
    for (int i = 10; i <= 50; i += 10) {
        bool isPresent = integers.contains(i);
        std::cout << "Is " << i << " present in the set? " << (isPresent ? "Yes" : "No") << std::endl;
    }

    return 0;
}
