all : crypto

#INCLUDE_DIRS = -I gtest-1.7.0/include
INCLUDE_DIRS =
CPPFLAGS = $(INCLUDE_DIRS) -std=c++0x

#LIB_DIRS = -Lgtest-1.7.0/lib
LIBS = -lboost_program_options
LDFLAGS = $(LIBS) $(LIB_DIRS) -fPIC
OBJECTS = cmdline_parser.o driver.o
SOURCES = *.cpp *.h
PROGRAM = crypto

tests : $(OBJECTS) $(SOURCES)
	g++  tests.cpp -o $(PROGRAM) $(CPPFLAGS) $(LDFLAGS)

$(PROGRAM) : $(SOURCES) cmdline_parser.cpp
	g++  driver.cpp cmdline_parser.o -o $(PROGRAM) $(CPPFLAGS) $(LDFLAGS)

tests.o : $(SOURCES)
	g++ -c tests.cpp $(CPPFLAGS)

cmdline_parser.o : cmdline_parser.cpp
	g++ -std=c++0x cmdline_parser.cpp -c $(CPPFLAGS) $(LDFLAGS)

run :
	./$(PROGRAM)

clean :
	rm *.o