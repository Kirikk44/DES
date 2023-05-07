#pragma once
#include "AbstractEncryptionMode.h"

class ECB: public AbstractEncryptionMode
{
public:
	ECB(Cryptographer* cryptographer) : AbstractEncryptionMode(cryptographer) {

	}
	void encrypt() override;
	void decrypt() override;
};

