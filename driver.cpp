//Driver 
//Thomas Dusterwald
//11 May 2014
#include <iostream>	
#include <fstream>	
#include <string>
#include "cmdline_parser.h"
#include "CryptoMachine.h"
#include "types.h"

int main(int argc, char* argv[])
{
	cmdline_parser parser;
	int switchsum = 0;
	std::string keyvig;
	int keyxor;
	int IV;
	
		if(parser.should_print_help())
		{ parser.print_help(std::cout);	}
	if(!parser.process_cmdline(argc, argv))
	{
		std::cerr << "Couldn't process command line arguments" << std::endl;
		return 1;
	}	

	//Initialize keys
	keyvig = parser.get_vignere_key();
	keyxor = parser.get_xor_key();
	IV = parser.get_IV();

	// Files to read from and write to.
	std::ifstream fin(parser.get_input_filename());
	std::ofstream fout(parser.get_output_filename());
	if(!fin.is_open()and!fout.is_open())
	{
		std::cerr << "Invalid file names" << std::endl;
		return 1;
	}

	if(parser.should_encode())
		switchsum+=4;
	if(parser.should_group())
		switchsum+=2;
	if(parser.should_pack())
		switchsum+=1;
	if(parser.vignere_or_xor())
		switchsum += 8;
	switch(switchsum)
	{
		case 0: // decode with xor -- no packing or grouping
			if(parser.get_mode()=="ECB")
			{
				CryptoMachine<xorencrypt,ecb, other, other> encrypter(keyxor);
				encrypter.decode(fin, fout);
			}
			else
			{
				CryptoMachine<xorencrypt,cbc, other, other> encrypter(keyxor,IV);
				encrypter.decode(fin, fout);
			}
			break;

		case 1: // decode with xor -- packing, no grouping
			if(parser.get_mode()=="ECB")
			{
				CryptoMachine<xorencrypt,ecb, other, pack> encrypter(keyxor);
				encrypter.decode(fin, fout);
			}
			else
			{
				CryptoMachine<xorencrypt,cbc, other, pack> encrypter(keyxor, IV);
				encrypter.decode(fin, fout);
			}
			break;

		case 2: //decode with xor -- grouping, no packing
			if(parser.get_mode()=="ECB")
			{
				CryptoMachine<xorencrypt,ecb, group, other> encrypter(keyxor);
				encrypter.decode(fin, fout);
			}
			else
			{
				CryptoMachine<xorencrypt,cbc, group, other> encrypter(keyxor, IV);
				encrypter.decode(fin, fout);
			}
			break;

		case 3: //decode with xor -- packing and grouping
			if(parser.get_mode()=="ECB")
			{
				CryptoMachine<xorencrypt,ecb, group, pack> encrypter(keyxor);
				encrypter.decode(fin, fout);
			}
			else
			{
				CryptoMachine<xorencrypt,cbc, group, pack> encrypter(keyxor, IV);
				encrypter.decode(fin, fout);
			}
			break;

		case 4: // encode with xor -- no packing or grouping
			if(parser.get_mode()=="ECB")
			{
				CryptoMachine<xorencrypt,ecb, other, other> encrypter(keyxor);
				encrypter.encode(fin, fout);
			}
			else
			{
				CryptoMachine<xorencrypt,cbc, other, other> encrypter(keyxor,IV);
				encrypter.encode(fin, fout);
			}
			break;

		case 5: // encode with xor -- packing, no grouping
			if(parser.get_mode()=="ECB")
			{
				CryptoMachine<xorencrypt,ecb, other, pack> encrypter(keyxor);
				encrypter.encode(fin, fout);
			}
			else
			{
				CryptoMachine<xorencrypt,cbc, other, pack> encrypter(keyxor, IV);
				encrypter.encode(fin, fout);
			}
			break;

		case 6: //encode with xor -- grouping, no packing
			if(parser.get_mode()=="ECB")
			{
				CryptoMachine<xorencrypt,ecb, group, other> encrypter(keyxor);
				encrypter.encode(fin, fout);
			}
			else
			{
				CryptoMachine<xorencrypt,cbc, group, other> encrypter(keyxor, IV);
				encrypter.encode(fin, fout);
			}
			break;

		case 7: //encode with xor -- packing and grouping
			if(parser.get_mode()=="ECB")
			{
				CryptoMachine<xorencrypt,ecb, group, pack> encrypter(keyxor);
				encrypter.encode(fin, fout);
			}
			else
			{
				CryptoMachine<xorencrypt,cbc, group, pack> encrypter(keyxor, IV);
				encrypter.encode(fin, fout);
			}
			break;

		case 8: //decode with vignere -- no packing or grouping
				{
					CryptoMachine<vignere,ecb, other, other> encrypter(keyvig);
					encrypter.decode(fin, fout);
					break;
				}


		case 9: //decode with vignere -- packing, no grouping
			{
				CryptoMachine<vignere,ecb, other, pack> encrypter(keyvig);
				encrypter.decode(fin, fout);
				break;
			}


		case 10: //decode with vignere -- grouping, no packing
			{
				CryptoMachine<vignere,ecb, group, other> encrypter(keyvig);
				encrypter.decode(fin, fout);
				break;
			}

		case 11: //decode with vignere -- grouping and packing
			{
				CryptoMachine<vignere,ecb, group, pack> encrypter(keyvig);
				encrypter.decode(fin, fout);
				break;
			}

		case 12: //encode with vignere -- no packing or grouping
			{
				CryptoMachine<vignere,ecb, other, other> encrypter(keyvig);
				encrypter.encode(fin, fout);
				break;
			}


		case 13: //encode with vignere -- packing, no grouping
			{
				CryptoMachine<vignere,ecb, other, pack> encrypter(keyvig);
				encrypter.encode(fin, fout);
				break;
			}

		case 14: //encode with vignere -- grouping, no packing
			{
				CryptoMachine<vignere,ecb, group, other> encrypter(keyvig);
				encrypter.encode(fin, fout);
				break;
			}

		case 15: //encode with vignere -- grouping and packing
			{
				CryptoMachine<vignere,ecb, group, pack> encrypter(keyvig);
				encrypter.encode(fin, fout);
				break;
			}
	}

	return 0;
}