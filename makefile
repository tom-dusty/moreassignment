all : crypto

#INCLUDE_DIRS = -I gtest-1.7.0/include
INCLUDE_DIRS =
CPPFLAGS = $(INCLUDE_DIRS) -std=c++0x

#LIB_DIRS = -Lgtest-1.7.0/lib
LIBS = -lboost_program_options
LDFLAGS = $(LIBS) $(LIB_DIRS) -fPIC
OBJECTS = cmdline_parser.o driver.o
SOURCES = *.cpp tests.cpp *.h
PROGRAM = crypto

tests : $(OBJECTS) $(SOURCES) tests.cpp
	g++  tests.cpp -o tests $(CPPFLAGS) $(LDFLAGS)


$(PROGRAM) : $(SOURCES) cmdline_parser.cpp $(OBJECTS)
	g++  driver.cpp cmdline_parser.o -o $(PROGRAM) $(CPPFLAGS) $(LDFLAGS)

cmdline_parser.o : cmdline_parser.cpp
	g++ -std=c++0x cmdline_parser.cpp -c $(CPPFLAGS) $(LDFLAGS)

run :
	./$(PROGRAM)

clean :
	rm *.o