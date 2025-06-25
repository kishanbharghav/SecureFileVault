#include <iostream>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include "encryption.h"

const std::string VAULT_DIR = "vault/";
const std::string RETRIEVED_DIR = "retrieved/";
const std::string INDEX_FILE = "vault_index.txt";
const std::string ENCRYPTION_KEY = "mySecret123";

void printBanner() {
    std::cout << "\033[1;36m\n========================\n";
    std::cout << "   🔐 Secure File Vault  \n";
    std::cout << "========================\033[0m\n";
}

void printMenu() {
    std::cout << "\033[1;33m\nChoose an option:\033[0m\n";
    std::cout << "\033[1;32m1.\033[0m Encrypt and Store File\n";
    std::cout << "\033[1;32m2.\033[0m Decrypt and Retrieve File\n";
    std::cout << "\033[1;32m3.\033[0m List Stored Files\n";
    std::cout << "\033[1;32m4.\033[0m Delete File\n";
    std::cout << "\033[1;32m5.\033[0m Exit\n";
    std::cout << "\033[1;34m> \033[0m";
}

void ensureDirectories() {
    std::filesystem::create_directory(VAULT_DIR);
    std::filesystem::create_directory(RETRIEVED_DIR);
}

void updateIndex(const std::string& original, const std::string& encryptedName, bool append = true) {
    std::ofstream out(INDEX_FILE, append ? std::ios::app : std::ios::trunc);
    if (out)
        out << original << " " << encryptedName << "\n";
}

std::unordered_map<std::string, std::string> loadIndex() {
    std::unordered_map<std::string, std::string> index;
    std::ifstream in(INDEX_FILE);
    std::string orig, enc;
    while (in >> orig >> enc) {
        index[orig] = enc;
    }
    return index;
}

void encryptFile() {
    std::string filePath;
    std::cout << "Enter path of file to encrypt: ";
    std::cin >> filePath;

    std::ifstream in(filePath, std::ios::binary);
    if (!in) {
        std::cerr << "\033[1;31mFile not found!\033[0m\n";
        return;
    }

    std::ostringstream ss;
    ss << in.rdbuf();
    std::string content = ss.str();
    in.close();

    std::string encrypted = xorEncryptDecrypt(content, ENCRYPTION_KEY);
    std::string hashName = std::to_string(std::hash<std::string>{}(filePath + std::to_string(time(0))));
    std::ofstream out(VAULT_DIR + hashName, std::ios::binary);
    out << encrypted;
    out.close();

    updateIndex(filePath, hashName);
    std::cout << "\033[1;32mFile encrypted and stored as: " << hashName << "\033[0m\n";
}

void decryptFile() {
    auto index = loadIndex();
    std::string fileName;
    std::cout << "Enter original filename to retrieve: ";
    std::cin >> fileName;

    if (index.find(fileName) == index.end()) {
        std::cerr << "\033[1;31mFile not found in vault.\033[0m\n";
        return;
    }

    std::ifstream in(VAULT_DIR + index[fileName], std::ios::binary);
    if (!in) {
        std::cerr << "\033[1;31mEncrypted file missing.\033[0m\n";
        return;
    }

    std::ostringstream ss;
    ss << in.rdbuf();
    std::string encrypted = ss.str();
    std::string decrypted = xorEncryptDecrypt(encrypted, ENCRYPTION_KEY);

    std::string outputFile = RETRIEVED_DIR + std::filesystem::path(fileName).filename().string();
    std::ofstream out(outputFile, std::ios::binary);
    out << decrypted;
    out.close();

    std::cout << "\033[1;32mFile decrypted and saved as " << outputFile << "\033[0m\n";
}

void listFiles() {
    auto index = loadIndex();
    std::cout << "\033[1;36m\nStored Files:\033[0m\n";
    int count = 0;
    for (const auto& [original, enc] : index) {
        std::cout << "• " << std::setw(30) << original << " → " << enc << "\n";
        ++count;
    }
    if (count == 0) std::cout << "Vault is empty.\n";
}

void deleteFile() {
    auto index = loadIndex();
    std::string fileName;
    std::cout << "Enter original filename to delete: ";
    std::cin >> fileName;

    if (index.find(fileName) == index.end()) {
        std::cerr << "\033[1;31mFile not found.\033[0m\n";
        return;
    }

    std::string encryptedName = index[fileName];
    std::filesystem::remove(VAULT_DIR + encryptedName);

    index.erase(fileName);
    std::ofstream out(INDEX_FILE, std::ios::trunc);
    for (const auto& [orig, enc] : index)
        out << orig << " " << enc << "\n";

    std::cout << "\033[1;32mFile deleted from vault.\033[0m\n";
}

int main() {
    ensureDirectories();
    int choice;

    while (true) {
        printBanner();
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: encryptFile(); break;
            case 2: decryptFile(); break;
            case 3: listFiles(); break;
            case 4: deleteFile(); break;
            case 5: std::cout << "Exiting...\n"; return 0;
            default: std::cout << "\033[1;31mInvalid choice.\033[0m\n";
        }

        std::cout << "\n\033[1;90mPress Enter to continue...\033[0m";
        std::cin.ignore(); std::cin.get();
        system("clear"); // Use "cls" for Windows
    }

    return 0;
}
