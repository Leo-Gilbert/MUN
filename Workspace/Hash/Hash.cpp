#include <iostream>
#include <list>

template<typename T>
class hashTable {

public:
    hashTable(){}

    size_t generateHashCode(const std::string &value)
    {
        return (value[0] - 'A') % 26;
    }

    void insert(const std::string& value)
        {
            size_t hashCode = generateHashCode(value);
            std::list<std::string>& chain = Table[hashCode];

            for (const auto& str : chain) {
                if (str == value) {
                    return; // If the string already exists, don't insert it again.
                }
            }

            chain.push_back(value); // If the string doesn't exist, insert it at the end of the linked list.
        }

    void printTable() const
    {
        for (size_t i = 0; i < 26; ++i) {
            std::cout << i << " ";
            for (const auto& str : Table[i]) {
                std::cout << str << " ";
            }
            std::cout << std::endl;
        }
    }

    void printBarData() const {
        std::cout << "Cell numbers: " << std::endl;
        for (size_t i = 0; i < 26; i++){
            std::cout << i << std::endl;
        }

        std::cout << "Cell sizes: " << std::endl;
        for (size_t i = 0; i < 26; i++){
            std::cout << Table[i].size() << std::endl;
        }
    }

private:
    std::list<std::string> Table[26];

};

int main(){
    
    hashTable<std::string> lehashtable;
    lehashtable.insert("Dog");
    lehashtable.insert("Iguana");
    lehashtable.insert("Pengulin");
    lehashtable.insert("Panda");
    lehashtable.insert("Pidgeon");
    lehashtable.insert("Zebra");
    lehashtable.insert("Armadillo");
    lehashtable.insert("Cow");
    lehashtable.insert("The Biggest Bird");
    lehashtable.insert("Antarctic Scale Worm");
    lehashtable.insert("Baboon");
    lehashtable.insert("Gerbil");
    lehashtable.insert("Wombat");
    lehashtable.insert("Newt");
    lehashtable.insert("Hamster");
    lehashtable.insert("Hippopotamus");
    lehashtable.printTable();
    lehashtable.printBarData();

}