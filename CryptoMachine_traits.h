// CryptoMachine traits class
// Thomas Dusterwald
// 13 May 2014

#ifndef _CRYPTOMACHINE_TRAITS_H
#define _CRYPTOMACHINE_TRAITS_H

#include <string>
#include "types.h"


// Templated with four types that modify functionality

template <typename Cipher, typename Mode, typename Group, typename Pack>
class CryptoMachine 
{
private:
	std::vector<char> key;
public:
	std::string cypher_type = "unimplemented";
	std::string mode = "unimplemented";
	bool group = false;
	bool pack = false;
};

//Cryptomachine templated with vignere and ECB options
template <typename Group, typename Pack> 
class CryptoMachine <vignere, ecb, Group, Pack>
{
private:
	std::vector<char> key;
public:
	std::string cypher_type = "vignere";
	std::string mode = "ECB";
	bool group = false;
	bool pack = false;
};

//vignere ecb pack
template <typename Group> 
class CryptoMachine <vignere, ecb, Group, pack>
{
private:
	std::vector<char> key;
public:
	std::string cypher_type = "vignere";
	std::string mode = "ECB";
	bool group = false;
	bool pack = true;
};

//Vignere with Grouping option
template <typename Pack> 
class CryptoMachine <vignere, ecb, group, Pack>
{
private:
	std::vector<char> key;
public:
	std::string cypher_type = "vignere";
	std::string mode = "ECB";
	bool group = true;
	bool pack = false;
};

//vignere ecb pack and group
template <> 
class CryptoMachine <vignere, ecb, group, pack>
{
private:
	std::vector<char> key;
public:
	std::string cypher_type = "vignere";
	std::string mode = "ECB";
	bool group = true;
	bool pack = true;
};


//xor ecb
template <typename Group, typename Pack> 
class CryptoMachine <xorencrypt, ecb, Group, Pack>
{
private:
	std::vector<char> key;
	const std::vector<char> IV(4,0);
public:
	std::string cypher_type = "xor";
	std::string mode = "ECB";
	bool group = false;
	bool pack = false;
};

//xor ecb grouping
template <typename Pack> 
class CryptoMachine <xorencrypt, ecb, group, Pack>
{
private:
	std::vector<char> key;
	const std::vector<char> IV(4,0);
public:
	std::string cypher_type = "xor";
	std::string mode = "ECB";
	bool group = true;
	bool pack = false;
};

//xor ecb packing
template <typename Group> 
class CryptoMachine <xorencrypt, ecb, Group, pack>
{
private:
	std::vector<char> key;
	const std::vector<char> IV(4,0);
public:
	std::string cypher_type = "xor";
	std::string mode = "ECB";
	bool group = false;
	bool pack = true;
};

//xor ecb packing and grouping
template <> 
class CryptoMachine <xorencrypt, ecb, group, pack>
{
private:
	std::vector<char> key;
	const std::vector<char> IV(4,0);
public:
	std::string cypher_type = "xor";
	std::string mode = "ECB";
	bool group = true;
	bool pack = true;
};

//xor CBC
template <typename Group, typename Pack> 
class CryptoMachine <xorencrypt, cbc, Group, Pack>
{
private:
	std::vector<char> key;
	std::vector<char> IV;
public:
	std::string cypher_type = "xor";
	std::string mode = "CBC";
	bool group = false;
	bool pack = false;
};

//xor CBC grouping
template <typename Pack> 
class CryptoMachine <xorencrypt, cbc, group, Pack>
{
private:
	std::vector<char> key;
	std::vector<char> IV;
public:
	std::string cypher_type = "xor";
	std::string mode = "CBC";
	bool group = true;
	bool pack = false;
};

//xor CBC packing
template <typename Group> 
class CryptoMachine <xorencrypt, cbc, Group, pack>
{
private:
	std::vector<char> key;
	std::vector<char> IV;
public:
	std::string cypher_type = "xor";
	std::string mode = "CBC";
	bool group = false;
	bool pack = true;
};

//xor CBC packing and grouping
template <> 
class CryptoMachine <xorencrypt, cbc, group, pack>
{
private:
	std::vector<char> key;
	std::vector<char> IV;
public:
	std::string cypher_type = "xor";
	std::string mode = "CBC";
	bool group = true;
	bool pack = true;
};

#endif