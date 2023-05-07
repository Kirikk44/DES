#include "DES.h"

uint64_t DES::funcF(uint64_t R, uint64_t ki) const {

    uint64_t buf = 0;
    buf = permutation(R, EBOX);
    buf ^= ki;
    buf = permutationS(buf, 8);
    buf = permutation(buf, PBOX);
    return 0;
}

uint64_t DES::funcFReverse(uint64_t R, uint64_t ki) const {

    uint64_t buf = 0;
    buf = permutation(R, EBOX);
    buf ^= ki;
    buf = permutationS(buf, 8);
    buf = permutation(buf, PBOX);
    return 0;
}

uint64_t DES::funcE(uint64_t t) const  {
    uint64_t newT = 0;
    for (int i = 0; i < 48; i++) {
        newT |= ((t >> (EBOX[i] - 1)) & 1) << i;
    }
    return newT;
}

uint64_t DES::permutation(uint64_t t, const std::vector<uint8_t> vec) const {
    uint64_t newT = 0;
    for (int i = 0; i < vec.size(); i++) {
        newT |= ((t >> (vec[i] - 1)) & 1) << i;
    }
    return newT;
}

uint64_t DES::shortenKey(uint64_t key) const 
{
    uint64_t res;
    for (int i = 0, j = 0; i < 64; ++i, ++j)
    {
        if ((i + 1) % 8 == 0)
            ++i;
        if (i < 64)
            res |= ((key >> i & 1) << j);
    }
    return res;
}

uint64_t DES::permutationS(uint64_t t, int k) const{
        uint64_t newT = 0;
        for (int i = 0; i < k; i++) {
            newT |= (SBOX[i][((t & 1) | ((t & 32) >> 4)) * ((t & 30) >> 1)]) << i * 4;
            t = t >> 6;
        }
        return newT;
 }

uint64_t DES::genKey(uint64_t key, uint8_t roundN) const {
    uint32_t mask = (1 << 28) - 1;
    uint64_t left = key >> 28;
    uint64_t right = key & (1 << 28 - 1);
    uint64_t res = 0;

    left = (left >> keyShift[roundN]) | (left << 28 - keyShift[roundN]) & mask;
    right = (right >> keyShift[roundN]) | (right << 28 - keyShift[roundN]) & mask;

    uint64_t shiftedKey = 0;
    //shiftedKey |= left;
    shiftedKey = (left << 28) | right;

    uint8_t buf;
    for (int i = 0; i < 48; ++i)
    {
        buf = (shiftedKey >> ((KEYGENBOX[i] - 1) - 1)) & 1; // получаем нужный бит
        res |= (buf << i);  // добавляем его в нужное место
    }

    return res;
}

void DES::encrypt(uint8_t* t, uint8_t* key,  uint8_t* res) const {
    encrypt(*(uint64_t*)t, *(uint64_t*)key, res);
}

void DES::decrypt(uint8_t* t, uint8_t* key, uint8_t* res) const
{
    return decrypt(*(uint64_t*)t, *(uint64_t*)key, res);
}

void DES::encrypt(uint64_t t, uint64_t key, uint8_t* res) const
{
    uint64_t buf = 0;
    uint64_t newL = 0;
    uint64_t newR = 0;
    uint64_t L = 0;
    uint64_t R = 0;

    buf = permutation(t, IPBOX);

    std::cout << "1 permutation:" << std::bitset<64>(buf) << std::endl;

    L = buf >> 32;
    R = buf & 0x00000000ffffffff;

    uint64_t ki = 0;
    key = shortenKey(key);

    for (int i = 0; i < roundNum; i++) {
        ki = genKey(key, i);
        newR = L ^ funcF(R, ki);

        newL = R;
    }
    buf = (newL << 32) | newR;
    buf = permutation(buf, FP);
    //std::cout << std::bitset<32>(buf) << std::endl;
    
    memcpy(res, &buf, 8);
}

void DES::decrypt(uint64_t t, uint64_t key, uint8_t* res) const
{
    uint64_t newL = 0;
    uint64_t newR = 0;
    uint64_t L = 0;
    uint64_t R = 0;
    uint64_t buf;

    buf = permutation(t, IPBOX);

    L = buf >> 32;
    R = buf & 0x00000000ffffffff;

    uint64_t ki = 0;

    key = shortenKey(key);

    for (int i = roundNum - 1; i  >= 0; i--) {
        ki = genKey(key, i);

        newR = L;
        newL = R ^ funcF(R, ki);
    }
    buf = (newL << 32) | newR;
    buf = permutation(buf, FP);
    //std::cout << std::bitset<32>(buf) << std::endl;

    memcpy(res, &buf, 8);
}

