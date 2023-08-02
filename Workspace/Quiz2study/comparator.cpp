#include <iostream>
#include <list>

template<typename T>
static void swap(T &x, T &y)
{
	// initially we didn't have the std::move(),
	// which limited the types we could swap
	T tmp = std::move(x);
	x = std::move(y);
	y = std::move(tmp);
}

template<typename Comparator, typename Iter>
void bubbleSort(Iter begin, Iter end, Comparator compare = Comparator())
{
	for (auto i = begin; i != end; i++)
	{
		for (auto j = begin; j != end; )
		{
			auto current = j;
			auto next = ++j;

			if (next == end)
			{
				break;
			}

			if (compare(*next, *current))
			{
				swap(*next, *current);
			}
		}
	}
}

int main(){

double values[] = { 1, 12, 7, 3 };
constexpr size_t len = sizeof(values) / sizeof(values[0]);
bubbleSort<std::greater<double>>(values, values + len);

for (int i = 0; i < len; i++){
    std::cout<<"Value: " << values[i] << std::endl;
}

int grades[] = { 1, 12, 7, 3 };
bubbleSort<std::less<int>>(grades, grades + sizeof(grades) / sizeof(grades[0]));

for (int i = 0; i < len; i++){
    std::cout<<"Grade: " << values[i] << std::endl;
}

std::list<std::string> names = { "Bob", "Alice", "Charlie" };
bubbleSort<std::greater<std::string>>(names.begin(), names.end());

}