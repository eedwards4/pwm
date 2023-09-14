// Wrapper for encryption and decryption
// Created by Ethan Edwards on 9/11/2023.
//

#include "encryptionHandler.h"

#include <utility>

// Startup
encryptionHandler::encryptionHandler() {
    // Check for keys.txt and locks.txt
    if (filesystem::exists("keys.txt")) {
        getKeys();
    }
    if (filesystem::exists("locks.txt")) {
        getLocks();
    }
}

encryptionHandler::~encryptionHandler() {
    // Write keys.txt and locks.txt
    hideKeys();
    storeLocks();
}

// User-facing
void encryptionHandler::getAcct(string filename) {
    cout << "Username for " << filename << " is: " << getUser(filename) << endl;
    cout << "Password for " << filename << " is: " << decrypt(filename) << endl;
}

void encryptionHandler::makeAcct(string filename, string username, string password) {
    encrypt(password, filename);
    userChain.push_back(username);
}

// Encryption/Decryption
void encryptionHandler::encrypt(string password, string filename) {
    ByteArray key = keyGen(), plain, enc; // Generate key
    filename = filename + ".enc"; // Add .enc to filename
    // Read password string into plain
    for (unsigned char i : password) {
        plain.push_back(i);
    }
    Aes256::encrypt(key, plain, enc); // Encrypt file
    // Write enc to file
    ofstream encFile(filename);
    if (encFile.is_open()) {
        for (unsigned char i : enc) {
            encFile << i;
        }
    }
    // Store key and filename
    keyChain.push_back(key);
    lockChain.push_back(filename);
}

string encryptionHandler::decrypt(string filename) {
    filename = filename + ".enc"; // Add .enc to filename
    // Find key
    ByteArray key, enc, dec;
    for (int i = 0; i < lockChain.size(); i++) {
        if (lockChain[i] == filename) {
            key = keyChain[i];
            break;
        }
    }
    // Read file into enc
    ifstream inFile(filename);
    if (inFile.is_open()) {
        char c;
        while (inFile.get(c)) {
            enc.push_back(c);
        }
    }
    // Decrypt file
    Aes256::decrypt(key, enc, dec);
    // Return decrypted string
    string password;
    for (unsigned char i : dec) {
        password.push_back(i);
    }
    return password;
}

ByteArray encryptionHandler::keyGen() {
    ByteArray key;
    for (int i = 0; i < KEY_LEN; i++) {
        key.push_back(rand() % 256);
    }
    return key;
}

// Trinary encryption/decryption
string encryptionHandler::encryptTri(string filename){

}

string encryptionHandler::decryptTri(string filename) {

}

void encryptionHandler::hideKeys() {
    ofstream keyFile("keys.txt");
    if (keyFile.is_open()) { // Temp function to directly write keys
        for (ByteArray i : keyChain) {
            for (unsigned char j : i) {
                keyFile << j;
            }
            keyFile << endl;
        }
    }

    /*
    if (keyFile.is_open()) { // Write encrypted keys to file
        for (ByteArray i : keyChain) {
            string key;
            for (unsigned char j : i) {
                key.push_back(j);
            }
            key = encryptTri(key);
            for (unsigned char j : key) {
                keyFile << j;
            }
            keyFile << endl;
        }
    }
     */
}

void encryptionHandler::getKeys() {
    fstream keyFile("keys.txt");
    if (keyFile.is_open()) { // Temp function to directly read in keys
        string line;
        while (getline(keyFile, line)) {
            ByteArray key;
            for (unsigned char i : line) {
                key.push_back(i);
            }
            keyChain.push_back(key);
        }
    }

    /*
    if (keyFile.is_open()) { // Read in encrypted keys, decrypt and pass to keychain
        string line;
        while (getline(keyFile, line)) {
            string key;
            for (unsigned char i: line) {
                key.push_back(i);
            }
            ByteArray alsoKey;
            key = decryptTri(key);
            for (unsigned char i: key) {
                alsoKey.push_back(i);
            }
            keyChain.push_back(alsoKey);
        }
    }
     */
}

void encryptionHandler::getLocks() {
    fstream lockFile("locks.txt");
    if (lockFile.is_open()) {
        string line;
        while (getline(lockFile, line)) {
            lockChain.push_back(line);
        }
    }
    fstream userFile("users.txt");
    if (userFile.is_open()) {
        string line;
        while (getline(userFile, line)) {
            userChain.push_back(line);
        }
    }
}

void encryptionHandler::storeLocks() {
    ofstream lockFile("locks.txt");
    if (lockFile.is_open()) {
        for (string i : lockChain) {
            lockFile << i << endl;
        }
    }
    ofstream userFile("users.txt");
    if (userFile.is_open()) {
        for (string i : userChain) {
            userFile << i << endl;
        }
    }
}

// Management (deletion, password change, etc)
void encryptionHandler::changePassword(const string& filename, string newPassword) {
    // Delete old password
    deletePassword(filename);
    // Encrypt new password
    encrypt(std::move(newPassword), filename);
}

void encryptionHandler::deletePassword(string filename) {
    // Delete password file
    string path = filename + ".enc";
    remove(path.c_str());
    // Delete password from keyChain and lockChain
    for (int i = 0; i < lockChain.size(); i++) {
        if (lockChain[i] == filename) {
            keyChain.erase(keyChain.begin() + i);
            lockChain.erase(lockChain.begin() + i);
            userChain.erase(userChain.begin() + i);
            break;
        }
    }
}

string encryptionHandler::getUser(std::string filename) {
    filename = filename + ".enc";
    for (int i = 0; i < lockChain.size(); i++) {
        if (lockChain[i] == filename) {
            return userChain[i];
        }
    }
    return "Error: File not found";
}
