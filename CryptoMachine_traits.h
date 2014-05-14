// CryptoMachine_Traits traits class
// Thomas Dusterwald
// 13 May 2014

#ifndef _CRYPTOMACHINE_TRAITS_H
#define _CRYPTOMACHINE_TRAITS_H

#include <string>
#include "types.h"


// Templated with four types that modify functionality

template <typename Cipher, typename Mode, typename Group, typename Pack>
class CryptoMachine_Traits 
{
public:
	static std::string cypher_type(void){return "unimplemented";}
	static std::string mode(void){return "unimplemented";}
	static const bool group = false;
	static const bool pack = false;
};

//CryptoMachine_Traits templated with vignere and ECB options
template <typename Group, typename Pack> 
class CryptoMachine_Traits <vignere, ecb, Group, Pack>
{
public:
	static std::string cypher_type(void){return "vignere";}
	static std::string mode(void){return "ECB";}
	static const bool group = false;
	static const bool pack = false;
};

//vignere ecb pack
template <typename Group> 
class CryptoMachine_Traits <vignere, ecb, Group, pack>
{
public:
	static std::string cypher_type(void){return "vignere";}
	static std::string mode(void){return "ECB";}
	static const bool group = false;
	static const bool pack = true;
};

//Vignere with Grouping option
template <typename Pack> 
class CryptoMachine_Traits <vignere, ecb, group, Pack>
{
public:
	static std::string cypher_type(void){return "vignere";}
	static std::string mode(void){return "ECB";}
	static const bool group = true;
	static const bool pack = false;
};

//vignere ecb pack and group
template <> 
class CryptoMachine_Traits <vignere, ecb, group, pack>
{
public:
	static std::string cypher_type(void){return "vignere";}
	static std::string mode(void){return "ECB";}
	static const bool group = true;
	static const bool pack = true;
};

//xor
template <typename Group, typename Mode, typename Pack> 
class CryptoMachine_Traits <xorencrypt, Mode, Group, Pack>
{
public:
	static std::string cypher_type(void){return "xor";}
	static std::string mode(void){return "?";}
	static const bool group = false;
	static const bool pack = false;
};

//xor ecb
template <typename Group, typename Pack> 
class CryptoMachine_Traits <xorencrypt, ecb, Group, Pack>
{
public:
	static std::string cypher_type(void){return "xor";}
	static std::string mode(void){return "ECB";}
	static const bool group = false;
	static const bool pack = false;
};

//xor ecb grouping
template <typename Pack> 
class CryptoMachine_Traits <xorencrypt, ecb, group, Pack>
{
public:
	static std::string cypher_type(void){return "xor";}
	static std::string mode(void){return "ECB";}
	static const bool group = true;
	static const bool pack = false;
};

//xor ecb packing
template <typename Group> 
class CryptoMachine_Traits <xorencrypt, ecb, Group, pack>
{
public:
	static std::string cypher_type(void){return "xor";}
	static std::string mode(void){return "ECB";}
	static const bool group = false;
	static const bool pack = true;
};

//xor ecb packing and grouping
template <> 
class CryptoMachine_Traits <xorencrypt, ecb, group, pack>
{
public:
	static std::string cypher_type(void){return "xor";}
	static std::string mode(void){return "ECB";}
	static const bool group = true;
	static const bool pack = true;
};

//xor CBC
template <typename Group, typename Pack> 
class CryptoMachine_Traits <xorencrypt, cbc, Group, Pack>
{
public:
	static std::string cypher_type(void){return "xor";}
	static std::string mode(void){return "CBC";}
	static const bool group = false;
	static const bool pack = false;
};

//xor CBC grouping
template <typename Pack> 
class CryptoMachine_Traits <xorencrypt, cbc, group, Pack>
{
public:
	static std::string cypher_type(void){return "xor";}
	static std::string mode(void){return "CBC";}
	static const bool group = true;
	static const bool pack = false;
};

//xor CBC packing
template <typename Group> 
class CryptoMachine_Traits <xorencrypt, cbc, Group, pack>
{
public:
	static std::string cypher_type(void){return "xor";}
	static std::string mode(void){return "CBC";}
	static const bool group = false;
	static const bool pack = true;
};

//xor CBC packing and grouping
template <> 
class CryptoMachine_Traits <xorencrypt, cbc, group, pack>
{
public:
	static std::string cypher_type(void){return "xor";}
	static std::string mode(void){return "CBC";}
	static const bool group = true;
	static const bool pack = true;
};

#endif