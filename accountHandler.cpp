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
    accountName = accountName + ".enc";
    try {
        accounts.emplace_back(accountName, username, encrypt(password, accountName));
    }
    catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }
    cout << "Account created successfully." << endl;
}

void accountHandler::getAcct(string accountName) {
    accountName = accountName + ".enc";
    for (tuple<string, string, ByteArray> i : accounts){
        if (get<0>(i) == accountName){
            cout << "Username for " << accountName << " is: " << get<1>(i) << endl;
            cout << "Password for " << accountName << " is: " << decrypt(get<0>(i), get<2>(i)) << endl;
        }
    }
}

void accountHandler::changeUsername(string accountName, string newUsername) {
    accountName = accountName + ".enc";
}

void accountHandler::changePassword(string accountName, string newPassword) {
    accountName = accountName + ".enc";
}

void accountHandler::deleteAcct(string accountName) {
    accountName = accountName + ".enc";
}

// Private
ByteArray accountHandler::encrypt(string password, string accountName) {
    ByteArray key = keyGen(), plain, enc;
    string encString;
    // Convert password to byte array
    for (char i : password){
        plain.push_back(i);
    }
    // Encrypt password
    Aes256::encrypt(key, plain, enc);
    storeFile(accountName, enc);
    return key;
}

string accountHandler::decrypt(string accountName, ByteArray key) {
    ByteArray enc, dec;
    string password;
    // Read file into enc
    enc = getFile(accountName);
    Aes256::decrypt(key, enc, dec);
    // Return decrypted string
    for (unsigned char i : dec){
        password.push_back(i);
    }
    return password;
}

int accountHandler::storeFile(std::string filename, ByteArray data) {
    try {
        // Open/create file
        ofstream file;
        file.open(filename);
        // Write data to file
        for (unsigned char i : data){
            file << i;
        }
        // Close file
        file.close();
    }
    catch (exception e){
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}

ByteArray accountHandler::getFile(std::string filename) {
    ByteArray data;
    try {
        // Open file
        ifstream file;
        file.open(filename);
        // Read data from file
        char c;
        while (file.get(c)){
            data.push_back(c);
        }
        // Close file
        file.close();
        return data;
    }
    catch (exception e){
        cout << "Error: " << e.what() << endl;
        return data;
    }
}

int accountHandler::deleteFile(std::string filename) {
    try {
        // Delete file
        filesystem::remove(filename);
    }
    catch (exception e){
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
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

