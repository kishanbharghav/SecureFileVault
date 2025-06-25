#include "encryption.h"

std::string xorEncryptDecrypt(const std::string& data, const std::string& key) {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= key[i % key.length()];
    }
    return result;
}
