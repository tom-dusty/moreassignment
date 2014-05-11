all : crypto

#INCLUDE_DIRS = -I gtest-1.7.0/include
INCLUDE_DIRS =
CPPFLAGS = $(INCLUDE_DIRS) -std=c++0x

#LIB_DIRS = -Lgtest-1.7.0/lib
#LIBS = -lgtest -lgtest_main -lpthread 
LDFLAGS = $(LIBS) $(LIB_DIRS) -fPIC
OBJECTS = *.o
SOURCES = tests.cpp *.h
PROGRAM = crypto

$(PROGRAM) : $(OBJECTS) $(SOURCES)
	g++  tests.cpp -o $(PROGRAM) $(CPPFLAGS) $(LDFLAGS)

tests.o : $(SOURCES)
	g++ -c tests.cpp $(CPPFLAGS)

run :
	./$(PROGRAM)

clean :
	rm *.o