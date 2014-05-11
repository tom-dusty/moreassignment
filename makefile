all : crypto

#INCLUDE_DIRS = -I gtest-1.7.0/include
INCLUDE_DIRS =
CPPFLAGS = $(INCLUDE_DIRS) -std=c++0x

#LIB_DIRS = -Lgtest-1.7.0/lib
LIBS = -lboost_program_options
LDFLAGS = $(LIBS) $(LIB_DIRS) -fPIC
OBJECTS = *.o
SOURCES = tests.cpp *.h
PROGRAM = crypto

tests : $(OBJECTS) $(SOURCES)
	g++  tests.cpp -o $(PROGRAM) $(CPPFLAGS) $(LDFLAGS)

$(PROGRAM) : $(OBJECTS) 
	g++ -std=c++0x driver.o cmdline_parser.o -o $(CPPFLAGS) $(LDFLAGS)

driver.o : $(SOURCES)
	g++  driver.cpp -c $(PROGRAM) $(CPPFLAGS) $(LDFLAGS)

tests.o : $(SOURCES)
	g++ -c tests.cpp $(CPPFLAGS)

cmdline_parser.o : cmdline_parser.cpp
	g++ -std=c++0x cmdline_parser.cpp -c $(CPPFLAGS) $(LDFLAGS)

run :
	./$(PROGRAM)

clean :
	rm *.o