//testing
//Thomas Dusterwald
//5 April 2014
/*
Some general notes:
The tests are divided into five Test cases:
	Node - tests the Node class, including the 6 special member functions
	Iterator - tests the tree_iterator methods for correctness
	Parse Tree - tests parse_tree methods and 6 special member functions
	Polymorphism - Makes sure the polymorphic calls are functioning correctly
	Randomized Tree Testing - Creates 500 random trees and checks them for correctness
*/

//Neccessary includes
#include <iostream>		
#include <string>		
#include <random>			//to generate randomized test cases
#include <chrono>			//For system time
#include "CryptoMachine.h"
#include "types.h"

#define CATCH_CONFIG_MAIN /
#include "catch.hpp"     

#define UNIT_TESTING 

using namespace std;       

TEST_CASE("Vignere encoding and decoding")
{
	//Generates random letters between A and Z
	std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
   	std::uniform_int_distribution<int> rangeA(65, 90); //random letters
   	std::uniform_int_distribution<int> rangeL(1, 15); //random word lengths
   	std::uniform_int_distribution<int> rangeK(0, 1000000000); //random xor key lengths


   	//Random key
	string key = "";
	int key2 = rangeK(e);

	int length = rangeL(e);
	for(int x = 0;x<length;x++)
	{
		key+=rangeA(e);
	}

	int length = rangeL(e);
	int length2 = rangeL(e);
	for(int y = 0;x<length2;y++)
	{
		for(int x = 0;x<length;x++)
		{
			key+=rangeA(e);
		}
	}


	SECTION("Encryption/decryption")
	{
		CryptoMachine<vignere,other, other, pack> thingy(key);	
	}

	CryptoMachine<vignere,other, other, pack> thingy(key);
	CryptoMachine<vignere,ecb, other, pack> thingy2(key);
	CryptoMachine<vignere,ecb,group,other>thingy3(key);
	CryptoMachine<vignere,ecb,other,pack>thingy6(key);
	CryptoMachine<xorencrypt,ecb,other,other>thingy4(key2);
	CryptoMachine<xorencrypt,ecb,group,other>thingy5(key2);
	CryptoMachine<xorencrypt,ecb,other,pack>thingyC(key2);
	CryptoMachine<xorencrypt,ecb,group,pack>thingyD(key2);

	CryptoMachine<xorencrypt,cbc,other,other>thingy8(key2, 10000);
	CryptoMachine<xorencrypt,cbc,group,other>thingy9(key2, 10000);
	CryptoMachine<xorencrypt,cbc,other,pack>thingyA(key2, 10000);
	CryptoMachine<xorencrypt,cbc,group,pack>thingyB(key2, 10000);
	CryptoMachine<vignere,ecb,group,pack>thingy7(key);

	ifstream fin("input.txt");
	ofstream fout("output.txt");
	ifstream fin2("output.txt");
	thingyD.encode(fin, fout);
	fout.close();
	fin.close();
	thingyD.decode(fin2, std::cout);
	fin2.close();
}

TEST_CASE("Vignere packing and unpacking")
{

}

TEST_CASE("Vignere grouping")
{

}

TEST_CASE("Xor encoding and decoding")
{

}

TEST_CASE("Xor packing and unpacking")
{

}

TEST_CASE("Xor grouping")


