CXXFLAGS=	-std=c++14

all: bst-test
clean:
	rm -f *.o bst

bst-test: bst-test.o
	$(CXX) $(LDFLAGS) bst-test.o -o bst-test

bst-test.o: BST.h