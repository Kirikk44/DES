#pragma once
#include "Cryptographer.h"
#include <istream>

class AbstractEncryptionMode
{
protected:
	const Cryptographer* cryptographer = nullptr;
	std::istream* input = nullptr;
	std::ostream* output = nullptr;

	std::vector<uint8_t> key;

public:
	virtual void encrypt() = 0;
	virtual void decrypt() = 0;
	AbstractEncryptionMode(const Cryptographer* cryptographer){
		this->cryptographer = cryptographer;
	}

	void setOutputStream(std::ostream* output)
	{
		this->output = output;
	}
	void setInputStream(std::istream* input)
	{
		this->input = input;
	}

	virtual void genKey(uint16_t key_size) {
		srand(time(nullptr));

		for (int i = 0; i <= key_size / 8; i++) {
			key.push_back(rand() % 255);
		}
	}

	virtual ~AbstractEncryptionMode() = default;
};

