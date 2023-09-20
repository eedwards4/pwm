//
// Created by Ethan Edwards on 9/13/2023.
//

#ifndef PWM_ACCOUNTHANDLER_H
#define PWM_ACCOUNTHANDLER_H

#define KEY_LEN 256

#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

#include "modules/utils/aes256.hpp"

using namespace std;

class accountHandler {
public:
    accountHandler();
    ~accountHandler();

    void makeAcct(string accountName, string username, string password);
    void getAcct(string accountName);
    void changeUsername(string accountName, string newUsername);
    void changePassword(string accountName, string newPassword);
    void deleteAcct(string accountName);


private:
    vector<tuple<string, string, ByteArray>> accounts; // Account name, username, encryption key

    // Key stuff
    ByteArray encrypt(string password, string accountName); // Encrypt a password, store it in a file, and add the key to the keychain
    string decrypt(string accountName, ByteArray key); // Decrypt a password by account and return it as a string
    ByteArray keyGen();

    // Management
    void hideKeys();
    void getKeys();
    void storeAccounts();
    void getAccounts();
    int storeFile(string filename, ByteArray data);
    ByteArray getFile(string filename);
    int deleteFile(string filename);

    // Master
    void masterEncrypt(string masterPass);
    string masterDecrypt();
    bool masterPassCheck(int check = 0);

};


#endif //PWM_ACCOUNTHANDLER_H
