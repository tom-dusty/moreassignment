// Cryptomachine class
// Thomas Dusterwald
// 19 April 2014

#ifndef _CRYPTOMACHINE_H
#define _CRYTOMACHINE_H

#include "types.h"
#include "CryptoMachine_traits.h"
#include "CryptoMachine_policies.h"
#include <algorithm>
#include <bitset>
#include <cstdint>

//Default template
template <typename Cipher, typename Mode, typename Group, typename Pack >
class CryptoMachine 
{

};

//vignere
template <typename Mode, typename Group, typename Pack >
class CryptoMachine <vignere, Mode, Group, Pack>
{
private:
	CryptoMachine_policies<vignere,Mode,Group,Pack> vignereP;
	std::vector<char> key;

public:
	//Constructor
	CryptoMachine(std::string keyIn)
	{
		std::transform(keyIn.begin(), keyIn.end(),std::back_inserter(key), [](char z) -> char{return toupper(z);});
	}

	//Encode data
	void encode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		std::transform(temp2.begin(), temp2.end(),temp2.begin(), [](char z) -> char{if(z==32){return 91;}return toupper(z);});
		vignereP.encode(temp2, out, key);
	}

	//Decode data
	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		vignereP.decode(temp2, out, key);
	}
};

//Xor
template <typename Mode, typename Group, typename Pack>
class CryptoMachine <xorencrypt, Mode, Group, Pack>
{
private:
	CryptoMachine_policies<xorencrypt,Mode,Group,Pack> xorP;
	std::vector<char> key;
	std::vector<char> IV;

public:
	//Constructor
	CryptoMachine(uint32_t keyIn, uint32_t IVin = 0)
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

	//Encode data
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
		xorP.encode(temp2, out, key,IV,CryptoMachine_Traits<xorencrypt,Mode,Group,Pack>::mode());
	}

	//Decode data
	void decode(std::istream & in, std::ostream & out)
	{
		std::string temp2;
		char temp = in.get();
		while(temp!=EOF)
		{
			temp2 += temp;
			temp = in.get();
		}
		xorP.decode(temp2, out, key,IV,CryptoMachine_Traits<xorencrypt,Mode,Group,Pack>::mode());
	}
};



#endif