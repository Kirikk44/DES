#include "ECB.h"
#include <iostream>

void padding(uint8_t* buf, int n) {
	if (n == 0) {
		return;
	}

	int diff = 8 - n;

	for (int i = n; i < 8; i++) {
		buf[i] = diff;
	}
}

int paddingRev(uint8_t* buf) {
	int n = buf[7];

	if (n > 7)
		return 8;
	for (int i = 7; i > 8 - n; i--) {
		if (buf[i] != n) {
			return 8;
		}
	}
	return 8 - n;
}

void ECB::encrypt()
{
	if (input == nullptr || output == nullptr)
		throw std::exception("file did not open");
	if (cryptographer == nullptr)
		throw std::exception("cryptographer null");
	uint8_t buf[BUFSIZ];
	uint8_t res[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	int blockSize = cryptographer->getBlockSize() / 8;
	if (key.empty())
		genKey(blockSize);

	std::streamsize n = 8;
	while (!input->read((char*)buf, blockSize).eof())
	{
		cryptographer->encrypt(buf, key.data(), res);
		output->write((char*)res, blockSize);
		std::cout << input->gcount() << std::endl;
	}
	std::cout << input->gcount() << std::endl;
	if (input->gcount()) {
		padding(buf, input->gcount());
		cryptographer->encrypt(buf,key.data(), res);
		output->write((char*)res, blockSize);
	}
}

void ECB::decrypt()
{
	uint8_t buf[BUFSIZ];
	uint8_t res[BUFSIZ];
	int blockSize = cryptographer->getBlockSize() / 8;
	if (key.empty())
		genKey(blockSize);

	input->seekg(0, std::ios_base::end);

	long long end = input->tellg();

	input->seekg(0, std::ios_base::beg);
	while (true)
	{
		input->read((char*)buf, blockSize);
		if (input->tellg() == end) {
			cryptographer->decrypt(buf, key.data(), res);
			blockSize = paddingRev(res);
			output->write((char*)res, blockSize);
			return;
		}
		cryptographer->decrypt(buf, key.data(), res);
		output->write((char*)res, blockSize);
	}
}
