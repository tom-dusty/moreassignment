// CryptoMachine_policies policies class
// Thomas Dusterwald
// 13 May 2014

#ifndef _CRYPTOMACHINE_POLICIES_H
#define _CRYPTOMACHINE_POLICIES_H

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
class xorenc{
public:
	int count;
	std::vector<char> key;
	std::vector<char> IV;
	std::string mode;
	xorenc(std::vector<char> keyIn,std::vector<char> IVin, std::string modeIn) : key(keyIn), IV(IVin), mode(modeIn), count(0){};
	char operator()(char in)
	{
		char tbr = in xor key[count];
		if(mode == "CBC")
			tbr = tbr xor IV[count];
		if(tbr == EOF)
			tbr = in;
		IV[count] = tbr;
		count = (count+1)%4;
		return tbr;
	}
};

//xor cbc decoder functor
class xordec {
public:
	int count;
	std::vector<char> key;
	std::vector<char> IV;
	std::string mode;
	xordec(std::vector<char> keyIn,std::vector<char> IVin, std::string modeIn) : key(keyIn), IV(IVin), mode(modeIn), count(0){};
	char operator()(char in)
	{
		char tbr = in xor key[count];
		if(mode == "CBC")
			tbr = tbr xor IV[count];		
		if(tbr == EOF)
			tbr = in;
		IV[count] = in;
		count = (count+1)%4;
		return tbr;
	}
};

// Templated with four types that modify functionality

template <typename Cipher, typename Mode, typename Group, typename Pack>
class CryptoMachine_policies 
{
public:

	void encode(std::string in, std::ostream & out, std::vector<char> key);
	void decode(std::string in, std::ostream & out, std::vector<char> key);
};

//CryptoMachine_policies templated with vignere and ECB options
template <typename Group, typename Pack> 
class CryptoMachine_policies <vignere, ecb, Group, Pack>
{
public:

	void encode(std::string in, std::ostream & out, std::vector<char> key)
	{
		vig translater(key);
		std::transform(in.begin(), in.end(), in.begin(), translater);
		out << in;
	}

	void decode(std::string in, std::ostream & out, std::vector<char> key)
	{
		vigdec translater(key);
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(in.begin(), in.end(), in.begin(), translater);
		out << in;
	}
};

//vignere ecb pack
template <typename Group> 
class CryptoMachine_policies <vignere, ecb, Group, pack>
{
public:

	void encode(std::string in, std::ostream & out, std::vector<char> key)
	{
		vig translater(key);
		//encode using vignere
		std::transform(in.begin(), in.end(), in.begin(), translater);
		//pack
		std::transform(in.begin(), in.end(),in.begin(),[](char z)->char{if(z==32){return 27;};return z-64;});
		std::bitset<1000> bits;
		std::for_each(in.begin(), in.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((in.size()*5)/8+1);
		int count = 0;
		std::string temp3;
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		out << temp3;

	}

	void decode(std::string in, std::ostream & out, std::vector<char> key)
	{
		vigdec translater(key);
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((in.size()*8)/5);
		int count = 0;
		std::string temp3;
		std::string temp4;
		bool odd = false;
		std::for_each(in.begin(), in.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits,&odd](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;if(bitty.to_ulong()==0)odd=true;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());
		if(odd)
		{
			std::copy_if(temp3.begin(),temp3.end(),std::back_inserter(temp4),[&](char in)->bool{return !(in==0);});
			temp3 = temp4;
		}

		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==27){return 32;};return z+64;});
		std::transform(temp3.begin(), temp3.end(),temp3.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), translater);

		out << temp3;
	}
};

//Vignere with Grouping option
template <typename Pack> 
class CryptoMachine_policies <vignere, ecb, group, Pack>
{
public:
	void encode(std::string in, std::ostream & out, std::vector<char> key)
	{
		vig translater(key);
		std::string temp3;
		//Grouping part:
		//	removes spaces
		std::copy_if(in.begin(), in.end(), back_inserter(temp3), [](char z)->bool{if(z==91)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),gro());
		temp3 = y.grouped;
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), translater);
		out << temp3;
	}

	void decode(std::string in, std::ostream & out, std::vector<char> key)
	{
		vigdec translater(key);
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(in.begin(), in.end(), in.begin(), translater);
		out << in;
	}
};

//vignere ecb pack and group
template <> 
class CryptoMachine_policies <vignere, ecb, group, pack>
{
public:
	void encode(std::string in, std::ostream & out, std::vector<char> key)
	{
		vig translater(key);
		std::string temp3;
		//Group
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{return toupper(z);});
		std::copy_if(in.begin(), in.end(), back_inserter(temp3), [](char z)->bool{if(z==32)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),gro());
		in = y.grouped;
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(in.begin(), in.end(), in.begin(), translater);
		//pack
		std::transform(in.begin(), in.end(),in.begin(),[](char z)->char{if(z==32){return 27;};return z-64;});
		std::bitset<1000> bits;
		std::for_each(in.begin(), in.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((in.size()*5)/8+1);
		int count = 0;
		temp3 = "";
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		out << temp3;
	}

	void decode(std::string in, std::ostream & out, std::vector<char> key)
	{
		vigdec translater(key);
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((in.size()*8)/5);
		int count = 0;
		std::string temp3;
		std::string temp4;
		bool odd = false;
		std::for_each(in.begin(), in.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits,&odd](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;if(bitty.to_ulong()==0)odd=true;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());
		if(odd)
		{
			std::copy_if(temp3.begin(),temp3.end(),std::back_inserter(temp4),[&](char in)->bool{return !(in==0);});
			temp3 = temp4;
		}
		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==27){return 32;};return z+64;});
		std::transform(temp3.begin(), temp3.end(),temp3.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), translater);
		out << temp3;
	}
};


//xor ecb/cbc
template <typename Mode, typename Group, typename Pack> 
class CryptoMachine_policies <xorencrypt, Mode, Group, Pack>
{
public:
	void encode(std::string in, std::ostream & out, std::vector<char> key,std::vector<char> IV, std::string mode)
	{
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{return toupper(z);});
		std::transform(in.begin(), in.end(), in.begin(), xorenc(key,IV,mode));
		out << in;

	}

	void decode(std::string in, std::ostream & out, std::vector<char> key,std::vector<char> IV, std::string mode)
	{
		std::transform(in.begin(), in.end(), in.begin(), xordec(key,IV,mode));
		out << in;
	}
};

//xor ecb/cbc grouping
template <typename Mode, typename Pack>
class CryptoMachine_policies <xorencrypt, Mode, group, Pack>
{
public:
	void encode(std::string in, std::ostream & out, std::vector<char> key,std::vector<char> IV, std::string mode)
	{
		std::string temp3;
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{return toupper(z);});

		//Grouping part:
		//	removes spaces
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{return toupper(z);});
		std::copy_if(in.begin(), in.end(), back_inserter(temp3), [](char z)->bool{if(z==32)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),gro());
		temp3 = y.grouped;
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), xorenc(key,IV,mode));
		out << temp3;

	}

	void decode(std::string in, std::ostream & out, std::vector<char> key,std::vector<char> IV, std::string mode)
	{
		std::transform(in.begin(), in.end(), in.begin(), xordec(key,IV,mode));
		out << in;
	}
};

//xor ecb packing
template <typename Mode, typename Group> 
class CryptoMachine_policies <xorencrypt, Mode, Group, pack>
{
public:
	void encode(std::string in, std::ostream & out, std::vector<char> key,std::vector<char> IV, std::string mode)
	{
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{return toupper(z);});
		//pack
		std::transform(in.begin(), in.end(),in.begin(),[](char z)->char{if(z==32){return 27;};return z-64;});
		std::bitset<1000> bits;
		std::for_each(in.begin(), in.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((in.size()*5)/8+1);
		int count = 0;
		std::string temp3;
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), xorenc(key,IV,mode));
		out << temp3;

	}

	void decode(std::string in, std::ostream & out, std::vector<char> key,std::vector<char> IV, std::string mode)
	{
		std::transform(in.begin(), in.end(), in.begin(), xordec(key,IV,mode));
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((in.size()*8)/5);
		int count = 0;
		std::string temp3;
		std::string temp4;
		bool odd = false;
		std::for_each(in.begin(), in.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits,&odd](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;if(bitty.to_ulong()==0)odd=true;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());
		if(odd)
		{
			std::copy_if(temp3.begin(),temp3.end(),std::back_inserter(temp4),[&](char in)->bool{return !(in==0);});
			temp3 = temp4;
		}

		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==27){return 32;};return z+64;});
		out << temp3;
	}
};

//xor ecb/ecb packing and grouping
template <typename Mode> 
class CryptoMachine_policies <xorencrypt, Mode, group, pack>
{
public:
	void encode(std::string in, std::ostream & out, std::vector<char> key,std::vector<char> IV, std::string mode)
	{
		std::string temp3;
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{return toupper(z);});
		//Grouping part:
		//	removes spaces
		std::transform(in.begin(), in.end(),in.begin(), [](char z) -> char{return toupper(z);});
		std::copy_if(in.begin(), in.end(), back_inserter(temp3), [](char z)->bool{if(z==32)return false;return true;});
		// 	insert spaces every 5
		gro y = std::for_each(temp3.begin(),temp3.end(),gro());
		in = y.grouped;
		temp3 = "";

		//pack
		std::transform(in.begin(), in.end(),in.begin(),[](char z)->char{if(z==32){return 27;};return z-64;});
		std::bitset<1000> bits;
		std::for_each(in.begin(), in.end(),[&bits](char z){bits = bits<<5;bits|=(std::bitset<1000>(z));});
		//chop off bits that we want into char again
		std::vector<bool> squishy((in.size()*5)/8+1);
		int count = 0;
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(8);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>8;return bitty.to_ulong();});
		std::reverse(temp3.begin(), temp3.end());
		std::transform(temp3.begin(), temp3.end(), temp3.begin(), xorenc(key,IV,mode));
		out << temp3;

	};
	void decode(std::string in, std::ostream & out, std::vector<char> key,std::vector<char> IV, std::string mode)
	{
		std::transform(in.begin(), in.end(), in.begin(), xordec(key,IV,mode));
		//unpack
		std::bitset<1000> bits;
		std::vector<bool> squishy((in.size()*8)/5);
		int count = 0;
		std::string temp3;
		std::string temp4;
		bool odd = false;
		std::for_each(in.begin(), in.end(),[&bits](char z){bits = bits<<8;bits|=(std::bitset<1000>(std::bitset<8>(z).to_string()));});
		std::transform(squishy.begin(), squishy.end(),std::back_inserter(temp3),[&count, &bits,&odd](bool n)->char{count = 0;char ret;std::vector<bool> squishy2(5);std::bitset<8> bitty;std::for_each(squishy2.begin(),squishy2.end(),[&count, &bits, &bitty](bool meh){bitty[count]=bits[count];count++;});bits = bits>>5;if(bitty.to_ulong()==0)odd=true;return bitty.to_ulong();});		//chop off bits that we want into char again
		std::reverse(temp3.begin(), temp3.end());
		if(odd)
		{
			std::copy_if(temp3.begin(),temp3.end(),std::back_inserter(temp4),[&](char in)->bool{return !(in==0);});
			temp3 = temp4;
		}
		std::transform(temp3.begin(), temp3.end(),temp3.begin(),[](char z)->char{if(z==27){return 32;};return z+64;});
		out << temp3;
	}
};

#endif
