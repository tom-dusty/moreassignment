// Cryptomachine class
// Thomas Dusterwald
// 19 April 2014

#ifndef _CRYPTOMACHINE_H
#define _CRYTOMACHINE_H

#include <iostream>
#include <algorithm>
#include <bitset>
#include <cstdint>
#include "types.h"

//Functor used for vignere encoding. Makes use of ascii values for the chars instead of using a lookup table
class vig {
	int count;
	std::vector<char> key;
public:
	vig(std::vector<char> keyIn) :count(0), key(keyIn){}
	char operator()(char in)
	{
		char ret = 32;
		if(in == 32)
			in = 91;
		//in-=13;
		if(((key[count]-11)%27+(in-11)%27)<26)
		{
			ret =  (key[count]+in)%26+65;
		}
		else if(((key[count]-11)%27+(in-11)%27)!=26)
		{
			ret = (key[count]+in-1)%26+65;
		}
		count = (count +1)%(key.size());
		return ret;
	}
};

//Decoding functor for vignere encoding.
class vigdec {
	int count;
	std::vector<char> key;
public:
	vigdec(std::vector<char> keyIn) :count(0), key(keyIn){}
	char operator()(char in)
	{
		char ret = 32;
		if(in == 32)
			in = 91;
		//in-=13;
		if(in>=key[count])
		{
			ret =  (in-key[count])%27+65;
			if(ret == 91)
				ret = 32;
		}
		else
		{
			ret = ((in-11)%27+27-(key[count]-11)%27)+65;
			if(ret == 91)
				ret = 32;
		}
		count = (count +1)%(key.size());
		return ret;
	}
};

//grouping functor
class gro {
public:
	int count;
	std::string grouped;
	gro() : count(0), grouped(""){};
	void operator()(char in)
	{
		grouped += in;
		if(count%5 == 4)
			grouped += ' ';
		count++;
	}
};

//xor ecb encoder/decoder functor
class xoring {
public:
	int count;
	std::vector<char> key;
	xoring(std::vector<char> keyIn) : key(keyIn), count(0){};
	char operator()(char in)
	{
		char tbr = in xor key[count];
		count = (count+1)%4;
		return tbr;
	}
};

//xor cbc encoder functor
class xorcbcenc {
public:
	int count;
	std::vector<char> key;
	std::vector<char> IV;
	xorcbcenc(std::vector<char> keyIn,std::vector<char> IVin) : key(keyIn),IV(IVin), count(0){};
	char operator()(char in)
	{
		char tbr = (in xor IV[count]) xor key[count];
		IV[count] = tbr;
		count = (count+1)%4;
		return tbr;
	}
};

//xor cbc decoder functor
class xorcbcdec {
public:
	int count;
	std::vector<char> key;
	std::vector<char> IV;
	xorcbcdec(std::vector<char> keyIn,std::vector<char> IVin) : key(keyIn),IV(IVin), count(0){};
	char operator()(char in)
	{
		char tbr = (in xor IV[count]) xor key[count];
		IV[count] = in;
		count = (count+1)%4;
		return tbr;
	}
};

// Templated with four types that modify functionality

template <typename Cipher, typename Mode, typename Group, typename Pack>
class CryptoMachine 
{
public:
	std::vector<char> key;
	
	//Default Constructor
	CryptoMachine(){};
	//Takes in a string key. Converts to char vector and stores in key
	CryptoMachine(std::string keyIn)
	{
		std::transform(keyIn.begin(), keyIn.end(),keyIn.begin(), [](char z) -> char{return toupper(z);});
		std::copy(keyIn.begin(), keyIn.end(), std::back_inserter(key));
	}

	void encode(std::istream & in, std::ostream & out);
	void decode(std::istream & in, std::ostream & out);
};

//Cryptomachine templated with vignere and ECB options
template <typename Group, typename Pack> 
class CryptoMachine <vignere, ecb, Group, Pack>
{
private:
	std::vector<char> key;

public:
	CryptoMachine(std::string keyIn)
	{
		std::transform(keyIn.begin(), keyIn.end(),std::back_inserter(key), [](char z) -> char{return toupper(z);});
	}

	void encode(std::istream & in, std::ostream & out)
	{
		vig translater(key);
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), translater);//prints key
		out << temp2;
	}

	void decode(std::istream & in, std::ostream & out)
	{
		vigdec translater(key);
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), translater);//prints key
		out << temp2;
	}
};

//vignere ecb pack
template <typename Group> 
class CryptoMachine <vignere, ecb, Group, pack>
{
private:
	std::vector<char> key;

public:
	CryptoMachine(std::string keyIn)
	{
		std::transform(keyIn.begin(), keyIn.end(),std::back_inserter(key), [](char z) -> char{return toupper(z);});
	}

	void encode(std::istream & in, std::ostream & out)
	{
		vig translater(key);
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), translater);
		//pack
		std::transform(temp2.begin(), temp2.end(),temp2.begin(),[](char z)->char{if(z==32){return 26;};return z-65;});
		std::bitset<1000> bits;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((temp2.size()*5)/8+1);
		int count = 0;
		std::string temp3;
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		out << temp3;
	}

	void decode(std::istream & in, std::ostream & out)
	{
		vigdec translater(key);
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((temp2.size()*8)/5);
		int count = (temp2.size()*8)/5;
		std::string temp3;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());

		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==26){return 32;};return z+65;});
		std::transform(temp3.begin(), temp3.end(),temp3.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), translater);
		out << temp3;
	}
};

//Vignere with Grouping option
template <typename Pack> 
class CryptoMachine <vignere, ecb, group, Pack>
{
private:
	std::vector<char> key;
	
public:
	CryptoMachine(std::string keyIn)
	{
		std::transform(keyIn.begin(), keyIn.end(),std::back_inserter(key), [](char z) -> char{return toupper(z);});
	}

	void encode(std::istream & in, std::ostream & out)
	{
		vig translater(key);
		gro groupMe;
		std::string temp2;
		std::string temp3;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}

		//Grouping part:
		//	removes spaces
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		std::copy_if(temp2.begin(), temp2.end(), back_inserter(temp3), [](char z)->bool{if(z==32)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),groupMe);
		temp3 = y.grouped;
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), translater);
		out << temp3;
	}

	void decode(std::istream & in, std::ostream & out)
	{
		vigdec translater(key);
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), translater);
		out << temp2;
	}
};

//vignere ecb pack and group
template <> 
class CryptoMachine <vignere, ecb, group, pack>
{
private:
	std::vector<char> key;

public:
	CryptoMachine(std::string keyIn)
	{
		std::transform(keyIn.begin(), keyIn.end(),std::back_inserter(key), [](char z) -> char{return toupper(z);});
	}

	void encode(std::istream & in, std::ostream & out)
	{
		vig translater(key);
		std::string temp2;
		std::string temp3;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		//Group
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		std::copy_if(temp2.begin(), temp2.end(), back_inserter(temp3), [](char z)->bool{if(z==32)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),gro());
		temp2 = y.grouped;
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), translater);
		//pack
		std::transform(temp2.begin(), temp2.end(),temp2.begin(),[](char z)->char{if(z==32){return 26;};return z-65;});
		std::bitset<1000> bits;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((temp2.size()*5)/8+1);
		int count = 0;
		temp3 = "";
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		out << temp3;
	}

	void decode(std::istream & in, std::ostream & out)
	{
		vigdec translater(key);
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((temp2.size()*8)/5);
		int count = (temp2.size()*8)/5;
		std::string temp3;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());

		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==26){return 32;};return z+65;});
		std::transform(temp3.begin(), temp3.end(),temp3.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), translater);
		out << temp3;
	}
};


//xor ecb
template <typename Group, typename Pack> 
class CryptoMachine <xorencrypt, ecb, Group, Pack>
{
private:
	std::vector<char> key;

public:
	CryptoMachine(uint32_t keyIn)
	{
		std::bitset <32> bits(keyIn);
		std::bitset <8> bits2;
		for(int i =0;i<32;++i)
		{	
			bits2[i%8] = bits[i];
			if(i%8==7)
			{
				key.push_back(bits2.to_ulong());
			}
		}
	}

	void encode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xoring(key));
		out << temp2;

	}

	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xoring(key));
		out << temp2;
	}
};

//xor ecb grouping
template <typename Pack> 
class CryptoMachine <xorencrypt, ecb, group, Pack>
{
private:
	std::vector<char> key;

public:
	CryptoMachine(uint32_t keyIn)
	{
		std::bitset <32> bits(keyIn);
		std::bitset <8> bits2;
		for(int i =0;i<32;++i)
		{	
			bits2[i%8] = bits[i];
			if(i%8==7)
			{
				key.push_back(bits2.to_ulong());
			}
		}
	}

	void encode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		std::string temp3;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});

		//Grouping part:
		//	removes spaces
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		std::copy_if(temp2.begin(), temp2.end(), back_inserter(temp3), [](char z)->bool{if(z==32)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),gro());
		temp3 = y.grouped;
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), xoring(key));
		out << temp3;

	}

	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}

		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xoring(key));
		out << temp2;
	}
};

//xor ecb packing
template <typename Group> 
class CryptoMachine <xorencrypt, ecb, Group, pack>
{
private:
	std::vector<char> key;

public:
	CryptoMachine(uint32_t keyIn)
	{
		std::bitset <32> bits(keyIn);
		std::bitset <8> bits2;
		for(int i =0;i<32;++i)
		{	
			bits2[i%8] = bits[i];
			if(i%8==7)
			{
				key.push_back(bits2.to_ulong());
			}
		}
	}

	void encode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}

		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		//pack
		std::transform(temp2.begin(), temp2.end(),temp2.begin(),[](char z)->char{if(z==32){return 26;};return z-65;});
		std::bitset<1000> bits;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((temp2.size()*5)/8+1);
		int count = 0;
		std::string temp3;
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), xoring(key));
		std::cout << temp3.length() <<std::endl;
		out << temp3;

	}

	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xoring(key));
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((temp2.size()*8)/5);
		int count = (temp2.size()*8)/5;
		std::string temp3;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==26){return 32;};return z+65;});
		out << temp3;
	}
};

//xor ecb packing and grouping
template <> 
class CryptoMachine <xorencrypt, ecb, group, pack>
{
private:
	std::vector<char> key;

public:
	CryptoMachine(uint32_t keyIn)
	{
		std::bitset <32> bits(keyIn);
		std::bitset <8> bits2;
		for(int i =0;i<32;++i)
		{	
			bits2[i%8] = bits[i];
			if(i%8==7)
			{
				key.push_back(bits2.to_ulong());
			}
		}
	}

	void encode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		std::string temp3;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}

		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		//Grouping part:
		//	removes spaces
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		std::copy_if(temp2.begin(), temp2.end(), back_inserter(temp3), [](char z)->bool{if(z==32)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),gro());
		temp2 = y.grouped;
		temp3 = "";

		//pack
		std::transform(temp2.begin(), temp2.end(),temp2.begin(),[](char z)->char{if(z==32){return 26;};return z-65;});
		std::bitset<1000> bits;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((temp2.size()*5)/8+1);
		int count = 0;
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), xoring(key));
		std::cout << temp3.length() <<std::endl;
		out << temp3;

	}

	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xoring(key));
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((temp2.size()*8)/5);
		int count = (temp2.size()*8)/5;
		std::string temp3;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==26){return 32;};return z+65;});
		out << temp3;
	}
};

//xor CBC
template <typename Group, typename Pack> 
class CryptoMachine <xorencrypt, cbc, Group, Pack>
{
private:
	std::vector<char> key;
	std::vector<char> IV;

public:
	CryptoMachine(uint32_t keyIn, uint32_t IVin)
	{
		std::bitset <32> bits(keyIn);
		std::bitset <32> bitsI(IVin);
		std::bitset <8> bits2;
		std::bitset <8> bits3;
		for(int i =0;i<32;++i)
		{	
			bits2[i%8] = bits[i];
			bits3[i%8] = bitsI[i];
			if(i%8==7)
			{
				key.push_back(bits2.to_ulong());
				IV.push_back(bits3.to_ulong());
			}
		}
	}

	void encode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});

		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xorcbcenc(key, IV));
		out << temp2;

	}

	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}

		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xorcbcdec(key, IV));
		out << temp2;
		encode(in, out);
	}
};

//xor CBC grouping
template <typename Pack> 
class CryptoMachine <xorencrypt, cbc, group, Pack>
{
private:
	std::vector<char> key;
	std::vector<char> IV;

public:
	CryptoMachine(uint32_t keyIn, uint32_t IVin)
	{
		std::bitset <32> bits(keyIn);
		std::bitset <32> bitsI(IVin);
		std::bitset <8> bits2;
		std::bitset <8> bits3;
		for(int i =0;i<32;++i)
		{	
			bits2[i%8] = bits[i];
			bits3[i%8] = bitsI[i];
			if(i%8==7)
			{
				key.push_back(bits2.to_ulong());
				IV.push_back(bits3.to_ulong());
			}
		}
	}

	void encode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		std::string temp3;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		//Grouping part:
		//	removes spaces
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		std::copy_if(temp2.begin(), temp2.end(), back_inserter(temp3), [](char z)->bool{if(z==32)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),gro());
		temp3 = y.grouped;
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), xorcbcenc(key, IV));
		out << temp3;

	}

	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}

		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xorcbcdec(key, IV));
		out << temp2;
		encode(in, out);
	}
};

//xor CBC packing
template <typename Group> 
class CryptoMachine <xorencrypt, cbc, Group, pack>
{
private:
	std::vector<char> key;
	std::vector<char> IV;

public:
	CryptoMachine(uint32_t keyIn, uint32_t IVin)
	{
		std::bitset <32> bits(keyIn);
		std::bitset <32> bitsI(IVin);
		std::bitset <8> bits2;
		std::bitset <8> bits3;
		for(int i =0;i<32;++i)
		{	
			bits2[i%8] = bits[i];
			bits3[i%8] = bitsI[i];
			if(i%8==7)
			{
				key.push_back(bits2.to_ulong());
				IV.push_back(bits3.to_ulong());
			}
		}
	}

	void encode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}

		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		//pack
		std::transform(temp2.begin(), temp2.end(),temp2.begin(),[](char z)->char{if(z==32){return 26;};return z-65;});
		std::bitset<1000> bits;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((temp2.size()*5)/8+1);
		int count = 0;
		std::string temp3;
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), xorcbcenc(key, IV));
		std::cout << temp3.length() <<std::endl;
		out << temp3;

	}

	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xorcbcdec(key, IV));
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((temp2.size()*8)/5);
		int count = (temp2.size()*8)/5;
		std::string temp3;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==26){return 32;};return z+65;});
		out << temp3;
	}
};

//xor CBC packing and grouping
template <> 
class CryptoMachine <xorencrypt, cbc, group, pack>
{
private:
	std::vector<char> key;
	std::vector<char> IV;

public:
	CryptoMachine(uint32_t keyIn, uint32_t IVin)
	{
		std::bitset <32> bits(keyIn);
		std::bitset <32> bitsI(IVin);
		std::bitset <8> bits2;
		std::bitset <8> bits3;
		for(int i =0;i<32;++i)
		{	
			bits2[i%8] = bits[i];
			bits3[i%8] = bitsI[i];
			if(i%8==7)
			{
				key.push_back(bits2.to_ulong());
				IV.push_back(bits3.to_ulong());
			}
		}
	}

	void encode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		std::string temp3;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}

		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		//Grouping part:
		//	removes spaces
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{return toupper(z);});
		std::copy_if(temp2.begin(), temp2.end(), back_inserter(temp3), [](char z)->bool{if(z==32)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),gro());
		temp2 = y.grouped;
		temp3 = "";

		//pack
		std::transform(temp2.begin(), temp2.end(),temp2.begin(),[](char z)->char{if(z==32){return 26;};return z-65;});
		std::bitset<1000> bits;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((temp2.size()*5)/8+1);
		int count = 0;
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), xorcbcenc(key, IV));
		std::cout << temp3.length() <<std::endl;
		out << temp3;

	}

	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), xorcbcdec(key, IV));
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((temp2.size()*8)/5);
		int count = (temp2.size()*8)/5;
		std::string temp3;
		std::for_each(temp2.begin(), temp2.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==26){return 32;};return z+65;});
		out << temp3;
	}
};

#endif