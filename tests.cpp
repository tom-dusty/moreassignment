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

#define CATCH_CONFIG_MAIN 
#include "catch.hpp"     

#define UNIT_TESTING 

using namespace std;       


TEST_CASE("Vignere encoding and decoding")

{
	//Generates random letters between A and Z
	std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
   	std::uniform_int_distribution<int> rangeA(65, 90); //random letters
   	std::uniform_int_distribution<int> rangeL(1, 15); //random key lengths
   	std::uniform_int_distribution<int> rangeS(1, 20); //random sentence lengths
   	std::uniform_int_distribution<int> rangeK(0, 1000000000); //random xor key lengths


   	//Random Sentence
   	for(int x = 0;x<50;x++)
   	{
	   	vector<char> sentence(rangeS(e));
		string sentence1;
		generate(sentence.begin(), sentence.end(), [&]()->char{return rangeA(e);});
		transform(sentence.begin(), sentence.end(), sentence.begin(), [&](char in)->char{if(rangeL(e)<4)return ' ';return in;});
		transform(sentence.begin(), sentence.end(), back_inserter(sentence1), [&](char in)->char{return in;});
		ofstream fout3("input.txt");
		fout3 << sentence1;
		fout3.close();
		vector<char> keyin(rangeL(e));
		string key;
		generate(keyin.begin(), keyin.end(), [&]()->char{return rangeA(e);});
		transform(keyin.begin(), keyin.end(), back_inserter(key), [&](char in)->char{return in;});
		CryptoMachine<vignere,ecb, other, other> thingy(key);	
		int key2 = rangeK(e);

		ifstream fin("input.txt");
		ofstream fout("output.txt");
		ifstream fin2("output.txt");
		thingy.encode(fin, fout);
		fout.close();
		fin.close();
		ofstream fout1("input.txt");
		thingy.decode(fin2, fout1);
		fin2.close();
		fout1.close();
		ifstream fin3("input.txt");
		std::string temp2;
		char temp = fin3.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = fin3.get();
		}

		REQUIRE(temp2 == sentence1);
   	}

}

TEST_CASE("Vignere packing and unpacking")
{
	//Generates random letters between A and Z
	std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
   	std::uniform_int_distribution<int> rangeA(65, 90); //random letters
   	std::uniform_int_distribution<int> rangeL(1, 15); //random key lengths
   	std::uniform_int_distribution<int> rangeS(1, 20); //random sentence lengths
   	std::uniform_int_distribution<int> rangeK(0, 1000000000); //random xor key lengths


   	for(int x =0;x<50;x++)
   	{
   		//Random Sentence
		vector<char> sentence(rangeS(e));
		string sentence1;
		generate(sentence.begin(), sentence.end(), [&]()->char{return rangeA(e);});
		transform(sentence.begin(), sentence.end(), sentence.begin(), [&](char in)->char{if(rangeL(e)<4)return ' ';return in;});
		transform(sentence.begin(), sentence.end(), back_inserter(sentence1), [&](char in)->char{return in;});
		ofstream fout3("input.txt");
		fout3 << sentence1;
		fout3.close();
		vector<char> keyin(rangeL(e));
		string key;
		generate(keyin.begin(), keyin.end(), [&]()->char{return rangeA(e);});
		transform(keyin.begin(), keyin.end(), back_inserter(key), [&](char in)->char{return in;});
		CryptoMachine<vignere,ecb, other, pack> thingy(key);	
		int key2 = rangeK(e);

		std::string temp2;
		ifstream fin("input.txt");
		ofstream fout("output.txt");
		thingy.encode(fin, fout);
		fout.close();
		fin.close();
		ifstream fin4("output.txt");
		temp2 = "";
		char temp = fin4.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = fin4.get();
		}
		fin4.close();

		//Make sure data is actually packed
		REQUIRE(temp2.length()==(sentence1.length()*5)/8+1);
		ofstream fout1("input.txt");
		ifstream fin2("output.txt");
		thingy.decode(fin2, fout1);
		fin2.close();
		fout1.close();
		ifstream fin3("input.txt");
		temp2 = "";
		temp = fin3.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = fin3.get();
		}
		//Check if data correctly encoded and decoded

		REQUIRE(temp2 == sentence1);
   	}
   	
}

TEST_CASE("Vignere grouping")
{
	//Generates random letters between A and Z
	std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
   	std::uniform_int_distribution<int> rangeA(65, 90); //random letters
   	std::uniform_int_distribution<int> rangeL(1, 15); //random key lengths
   	std::uniform_int_distribution<int> rangeS(1, 20); //random sentence lengths
   	std::uniform_int_distribution<int> rangeK(0, 1000000000); //random xor key lengths


   	//Random Sentence
   	for(int x = 0;x<50;x++)
   	{
	   	vector<char> sentence(rangeS(e));
		string sentence1;
		generate(sentence.begin(), sentence.end(), [&]()->char{return rangeA(e);});
		transform(sentence.begin(), sentence.end(), sentence.begin(), [&](char in)->char{if(rangeL(e)<4)return ' ';return in;});
		transform(sentence.begin(), sentence.end(), back_inserter(sentence1), [&](char in)->char{return in;});
		ofstream fout3("input.txt");
		fout3 << sentence1;
		fout3.close();
		vector<char> keyin(rangeL(e));
		string key;
		generate(keyin.begin(), keyin.end(), [&]()->char{return rangeA(e);});
		transform(keyin.begin(), keyin.end(), back_inserter(key), [&](char in)->char{return in;});
		CryptoMachine<vignere,ecb, group, other> thingy(key);	
		int key2 = rangeK(e);

		ifstream fin("input.txt");
		ofstream fout("output.txt");
		ifstream fin2("output.txt");
		thingy.encode(fin, fout);
		fout.close();
		fin.close();
		ofstream fout1("input.txt");
		thingy.decode(fin2, fout1);
		fin2.close();
		fout1.close();
		ifstream fin3("input.txt");
		std::string temp2;
		char temp = fin3.get();
		int countout = 0;
		while(temp!=EOF)
		{
			if(temp==' ')
				countout++;
			temp = fin3.get();
		}
		int countin = 0;
		for_each(sentence1.begin(),sentence1.end(),[&countin](char in){if(in==' ')countin++;});
		int input = (sentence1.length()-countin)/5;
		//Require that the correct number of spaces have been inserted in the right place
		REQUIRE(input == countout);
   	}

}


TEST_CASE("Xor EBC encoding and decoding")
{
//Generates random letters between A and Z
	std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::uniform_int_distribution<int> rangeA(65, 90); //random letters
   	std::uniform_int_distribution<int> rangeL(1, 15); //random key lengths
   	std::uniform_int_distribution<int> rangeS(1, 20); //random sentence lengths
   	std::uniform_int_distribution<int> rangeK(0, 1000000000); //random xor key  and IV lengths


   	//Random Sentence
   	for(int x = 0;x<50;x++)
   	{
	   	vector<char> sentence(rangeS(e));
		string sentence1;
		generate(sentence.begin(), sentence.end(), [&]()->char{return rangeA(e);});
		transform(sentence.begin(), sentence.end(), sentence.begin(), [&](char in)->char{if(rangeL(e)<4)return ' ';return in;});
		transform(sentence.begin(), sentence.end(), back_inserter(sentence1), [&](char in)->char{return in;});
		ofstream fout3("input.txt");
		fout3 << sentence1;
		fout3.close();
		int key = rangeK(e);
		CryptoMachine<xorencrypt,ecb, other, other> thingy(key);	

		ifstream fin("input.txt");
		ofstream fout("output.txt");
		ifstream fin2("output.txt");
		thingy.encode(fin, fout);
		fout.close();
		fin.close();
		ofstream fout1("input.txt");
		thingy.decode(fin2, fout1);
		fin2.close();
		fout1.close();
		ifstream fin3("input.txt");
		std::string temp2;
		char temp = fin3.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = fin3.get();
		}

		REQUIRE(temp2 == sentence1);
   	}
}

TEST_CASE("CBC encoding an decoding")
{
	//Generates random letters between A and Z
	std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
   	std::uniform_int_distribution<int> rangeA(65, 90); //random letters
   	std::uniform_int_distribution<int> rangeL(1, 15); //random key lengths
   	std::uniform_int_distribution<int> rangeS(1, 20); //random sentence lengths
   	std::uniform_int_distribution<int> rangeK(0, 1000000000); //random xor key lengths


   	//Random Sentence
   	for(int x = 0;x<50;x++)
   	{
	   	vector<char> sentence(rangeS(e));
		string sentence1;
		generate(sentence.begin(), sentence.end(), [&]()->char{return rangeA(e);});
		transform(sentence.begin(), sentence.end(), sentence.begin(), [&](char in)->char{if(rangeL(e)<4)return ' ';return in;});
		transform(sentence.begin(), sentence.end(), back_inserter(sentence1), [&](char in)->char{return in;});
		ofstream fout3("input.txt");
		fout3 << sentence1;
		fout3.close();
		int key = rangeK(e);
		int IV = rangeK(e);
		CryptoMachine<xorencrypt,cbc, other, other> thingy(key, IV);	

		ifstream fin("input.txt");
		ofstream fout("output.txt");
		ifstream fin2("output.txt");
		thingy.encode(fin, fout);
		fout.close();
		fin.close();
		ofstream fout1("input.txt");
		thingy.decode(fin2, fout1);
		fin2.close();
		fout1.close();
		ifstream fin3("input.txt");
		std::string temp2;
		char temp = fin3.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = fin3.get();
		}

		REQUIRE(temp2 == sentence1);
   	}
}


TEST_CASE("Xor packing and unpacking")
{
	std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
   	std::uniform_int_distribution<int> rangeA(65, 90); //random letters
   	std::uniform_int_distribution<int> rangeL(1, 15); //random key lengths
   	std::uniform_int_distribution<int> rangeS(1, 20); //random sentence lengths
   	std::uniform_int_distribution<int> rangeK(0, 1000000000); //random xor key lengths


   	//Random Sentence
   	for(int x = 0;x<50;x++)
   	{
	   	vector<char> sentence(rangeS(e));
		string sentence1;
		generate(sentence.begin(), sentence.end(), [&]()->char{return rangeA(e);});
		transform(sentence.begin(), sentence.end(), sentence.begin(), [&](char in)->char{if(rangeL(e)<4)return ' ';return in;});
		transform(sentence.begin(), sentence.end(), back_inserter(sentence1), [&](char in)->char{return in;});
		ofstream fout3("input.txt");
		fout3 << sentence1;
		fout3.close();
		int key = rangeK(e);
		CryptoMachine<xorencrypt,ecb, other, pack> thingy(key);	

		std::string temp2;
		ifstream fin("input.txt");
		ofstream fout("output.txt");
		thingy.encode(fin, fout);
		fout.close();
		fin.close();
		ifstream fin4("output.txt");
		temp2 = "";
		char temp = fin4.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = fin4.get();
		}
		fin4.close();

		//Make sure data is actually packed
		REQUIRE(temp2.length()==(sentence1.length()*5)/8+1);
		ofstream fout1("input.txt");
		ifstream fin2("output.txt");
		thingy.decode(fin2, fout1);
		fin2.close();
		fout1.close();
		ifstream fin3("input.txt");
		temp2 = "";
		temp = fin3.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = fin3.get();
		}
		//Check if data correctly encoded and decoded

		REQUIRE(temp2 == sentence1);
   	}
}

TEST_CASE("Xor grouping")
{
	std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
   	std::uniform_int_distribution<int> rangeA(65, 90); //random letters
   	std::uniform_int_distribution<int> rangeL(1, 15); //random key lengths
   	std::uniform_int_distribution<int> rangeS(1, 20); //random sentence lengths
   	std::uniform_int_distribution<int> rangeK(0, 1000000000); //random xor key lengths


   	//Random Sentence
   	for(int x = 0;x<50;x++)
   	{
	   	vector<char> sentence(rangeS(e));
		string sentence1;
		generate(sentence.begin(), sentence.end(), [&]()->char{return rangeA(e);});
		transform(sentence.begin(), sentence.end(), sentence.begin(), [&](char in)->char{if(rangeL(e)<4)return ' ';return in;});
		transform(sentence.begin(), sentence.end(), back_inserter(sentence1), [&](char in)->char{return in;});
		ofstream fout3("input.txt");
		fout3 << sentence1;
		fout3.close();
		int key = rangeK(e);
		CryptoMachine<xorencrypt,ecb, group, other> thingy(key);	

		ifstream fin("input.txt");
		ofstream fout("output.txt");
		ifstream fin2("output.txt");
		thingy.encode(fin, fout);
		fout.close();
		fin.close();
		ofstream fout1("input.txt");
		thingy.decode(fin2, fout1);
		fin2.close();
		fout1.close();
		ifstream fin3("input.txt");
		std::string temp2;
		char temp = fin3.get();
		int countout = 0;
		while(temp!=EOF)
		{
			if(temp==' ')
				countout++;
			temp = fin3.get();
		}
		int countin = 0;
		for_each(sentence1.begin(),sentence1.end(),[&countin](char in){if(in==' ')countin++;});
		int input = (sentence1.length()-countin)/5;
		//Require that the correct number of spaces have been inserted in the right place
		REQUIRE(input == countout);
	}
}