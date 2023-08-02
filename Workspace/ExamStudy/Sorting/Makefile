CXXFLAGS=	-std=c++11 -Wall

BINARY=	sorting
SRCS=	sorting.cpp
OBJS=	${SRCS:.cpp=.o}

all: ${BINARY}
clean:
	rm -f ${BINARY} *.o

${OBJS}: sorting.h

sorting: ${OBJS}
	${CXX} ${OBJS} -o ${BINARY}

	