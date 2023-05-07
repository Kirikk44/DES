#pragma once
#include <cstdint>
#include <vector>

class Cryptographer {
	
public: 
	virtual void encrypt(uint8_t* t, uint8_t* key, uint8_t* res) const = 0;
	virtual void decrypt(uint8_t* t, uint8_t* key, uint8_t* res) const = 0;

	virtual uint16_t getBlockSize() const = 0;
	virtual uint16_t getKeySize() const = 0;

	virtual ~Cryptographer() = default;


protected:
	std::vector<uint8_t> key;
};
