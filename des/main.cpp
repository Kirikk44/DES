#include "ECB.h"

#include <iostream>
#include "DES.h"
#include "ECB.h"
#include <fstream>

void testECB();

int main()
{
	testECB();
}

void testECB() {
	DES des;
	ECB ecb(&des);
	std::ifstream in;
	std::ofstream out;
	in.open("file.txt", std::ifstream::binary);
	out.open("cipher.txt", std::ofstream::binary);

	ecb.setInputStream(&in);
	ecb.setOutputStream(&out);

	try {
		ecb.encrypt();
	}
	catch(std::exception ex){
		std::cout << ex.what() << std::endl;
	}

	in.close();
	out.close();

	in.open("cipher.txt", std::ifstream::binary);
	out.open("file_res.txt", std::ofstream::binary);
	ecb.setInputStream(&in);
	ecb.setOutputStream(&out);
	try {
		ecb.decrypt();
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
	}
	in.close();
	out.close();
}

//DES des;
//
//uint64_t t = 0xc1113cb0fafbfadf;
//uint64_t k = 0xc32930a0fffbffff;
//
//std::cout << std::bitset<64>(t) << std::endl;
//
//uint64_t res_c;
//uint64_t res_d;
//
//des.encrypt((uint8_t*)&t, (uint8_t*)&k, (uint8_t*)&res_c);
//des.decrypt((uint8_t*)&res_c, (uint8_t*)&k, (uint8_t*)&res_d);
//
//std::cout << std::bitset<64>(res_c) << std::endl;
//std::cout << std::bitset<64>(res_d) << std::endl;