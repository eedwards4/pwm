// Created by Ethan Edwards on 9/11/23
// Password manager

#include <iostream>
#include <fstream>

#include "accountHandler.h"

void encryptCLI(accountHandler handler);
void decryptCLI(accountHandler handler);
void changePasswordCLI(accountHandler handler);
void deletePasswordCLI(accountHandler handler);

int main(){
    accountHandler handler;
    cout << "Welcome to PWM!" << endl;
    cout << "What would you like to do?" << endl;
    cout << "1. Add an account" << endl;
    cout << "2. Decrypt an account" << endl;
    cout << "3. Change a password" << endl;
    cout << "4. Delete an account" << endl;
    cout << "5. Exit" << endl;
    int choice = 0;
    while (choice != 5){
        cout << "Input your choice: ";
        cin >> choice;
        if (choice == 1){
            encryptCLI(handler);
        }
        else if (choice == 2){
            decryptCLI(handler);
        }
        else if (choice == 3){
            changePasswordCLI(handler);
        }
        else if (choice == 4){
            deletePasswordCLI(handler);
        }
        else if (choice == 5){
            cout << "Shutting down..." << endl;
        }
        else {
            cout << "Invalid choice." << endl;
        }
    }
    return 0;
}

void encryptCLI(accountHandler handler){
    cout << "Input an account: " << endl;
    string acctName;
    cin >> acctName;
    cout << "Input the username: " << endl;
    string user;
    cin >> user;
    cout << "Input the password: " << endl;
    string pass;
    cin >> pass;
    handler.makeAcct(acctName, user, pass);
}

void decryptCLI(accountHandler handler){
    cout << "Input an account: " << endl;
    string acctName;
    cin >> acctName;
    handler.getAcct(acctName);
}

void changePasswordCLI(accountHandler handler){
    cout << "Input an account: " << endl;
    string filename;
    cin >> filename;
    cout << "Input the new password: " << endl;
    string pass;
    cin >> pass;
    handler.changePassword(filename, pass);
}

void deletePasswordCLI(accountHandler handler){
    cout << "Input an account: " << endl;
    string filename;
    cin >> filename;
    handler.deleteAcct(filename);
}
