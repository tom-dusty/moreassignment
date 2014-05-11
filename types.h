// Types to be used in CryptoMachine
// Thomas Dusterwald
// 9 May 2014
#ifndef _TYPES_H
#define _TYPES_H

class vignere{};
class xorencrypt{};
class ecb{};
class cbc{};
class group{};
class other{};
class pack{};

#endif

//Combos
/*
- vignere, ecb x 
- vignere, ecb, codeblocks x
- vignere, ecb, pack  x
- vignere, ecb, group, pack x
- xor, ecb x 
- xor, ecb, group x 
- xor ecb, pack
- xor, ecb, group, pack
- xor, cbc -- use IV = all 1s for ECB. Thus saving time
- xor, cbc, pack 
- xor, cbc, group
- xor, cbc, group, pack
- xor, cbc, pack
- xoe, e
*/
