//
// Created by Ethan Edwards on 9/13/2023.
//

#include "accountHandler.h"

// setup/shutdown
accountHandler::accountHandler() {
    getKeys();
    getAccounts();
    // TODO: Check if master password exists, if not, create it
    /*
    if (masterPassCheck()){
        getKeys();
        getAccounts();
    }
    else { // Force exit if master password is incorrect
        exit(1);
    }*/
}

accountHandler::~accountHandler() {
    storeAccounts();
    hideKeys();
}

// Public
void accountHandler::makeAcct(string accountName, string username, string password) {
    accounts.emplace_back(accountName, username, encrypt(password, accountName));
    cout << "Account created successfully." << endl;
}

void accountHandler::getAcct(string accountName) {
    for (tuple<string, string, ByteArray> i : accounts){
        if (get<0>(i) == accountName){
            cout << "Username for " << accountName << " is: " << get<1>(i) << endl;
            cout << "Password for " << accountName << " is: " << decrypt(get<0>(i), get<2>(i)) << endl;
        }
    }
}

void accountHandler::changeUsername(string accountName, string newUsername) {

}

void accountHandler::changePassword(string accountName, string newPassword) {

}

void accountHandler::deleteAcct(string accountName) {

}

// Private
ByteArray accountHandler::encrypt(string password, string accountName) {
    ByteArray key = keyGen(), plain, enc;
    for (unsigned char i : password) {
        plain.push_back(i);
    }
    // Encrypt password
    Aes256::encrypt(plain, key, enc);
    // Store encrypted password as file
    ofstream encFile(accountName + ".enc");
    if (encFile.is_open()) {
        for (unsigned char i : enc) {
            encFile << i;
        }
        encFile.close();
    }
    // Return key
    return key;
}

string accountHandler::decrypt(string accountName, ByteArray key) {
    fstream encFile(accountName + ".enc");
    ByteArray enc, dec;
    if (encFile.is_open()) {
        char c;
        while (encFile.get(c)) {
            enc.push_back(c);
        }
        encFile.close();
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

ByteArray accountHandler::keyGen() {
    ByteArray key;
    for (int i = 0; i < KEY_LEN; i++) {
        key.push_back(rand() % 256);
    }
    return key;
}

void accountHandler::hideKeys() {

}

void accountHandler::getKeys() {

}

void accountHandler::storeAccounts() {

}

void accountHandler::getAccounts() {

}

// Master
void accountHandler::masterEncrypt(string masterPass) {

}

string accountHandler::masterDecrypt() {

}

bool accountHandler::masterPassCheck(int check) {
    string masterPass, input;
    // CLI
    // Check for master.txt (first run check)
    if (!filesystem::exists("master.crypt")){
        cout << "No master password found. Please enter a new master password: ";
        cin >> masterPass;
        // Encrypt master password
        masterEncrypt(masterPass);
        return true;
    }
    else {
        // Decrypt master password
        masterPass = masterDecrypt();
    }
    cout << "Enter master password: ";
    cin >> input;
    if (input == masterPass){
        return true;
    }
    else if (check < 5){
        cout << "Incorrect password. " << 5 - check << " attempts remaining." << endl;
        masterPassCheck(check + 1);
    }
    else {
        cout << "Too many attempts. Shutting down..." << endl;
        return false;
    }
}

