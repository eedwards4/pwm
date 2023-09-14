// Wrapper for encryption and decryption
// Created by Ethan Edwards on 9/11/2023.
//

#ifndef PWM_ENCRYPTIONHANDLER_H
#define PWM_ENCRYPTIONHANDLER_H

#define KEY_LEN 32
#define TEXT_SIZE 100
#define ENC_SIZE 113

#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

#include "modules/utils/aes256.hpp"

using namespace std;

class encryptionHandler {
public:
    encryptionHandler();
    ~encryptionHandler();
    // Encryption/Decryption
    void getAcct(string filename);
    void makeAcct(string filename, string username, string password);

    // Management (deletion, password change, etc)
    void changePassword(const string& filename, string newPassword);
    void deletePassword(string filename);

private:
    vector<string> userChain;
    vector<ByteArray> keyChain;
    vector<string> lockChain;

    ByteArray keyGen();
    void encrypt(string password, string filename); // Encrypt a new password
    string decrypt(string filename); // Decrypt a password
    string decryptTri(string filename); // Decrypts keys from trinary file into keyChain
    string encryptTri(string filename); // Writes + encrypts keys to trinary file

    string getUser(string filename); // Gets username from file

    void hideKeys(); // Writes + encrypts keys to file (keys.txt)
    void getKeys(); // Decrypts keys from file into keyChain
    void storeLocks(); // Writes lockChain to file (locks.txt)
    void getLocks(); // Reads the filenames of all encrypted files into lockChain

};


#endif //PWM_ENCRYPTIONHANDLER_H
