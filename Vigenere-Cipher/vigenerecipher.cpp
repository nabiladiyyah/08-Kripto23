#include <iostream>
#include <string>

using namespace std;

string encryptVigenere(string plaintext, string key) {
    string ciphertext = "";
    int keyLength = key.length();

    for (int i = 0; i < plaintext.length(); i++) {
        char plainChar = plaintext[i];
        char keyChar = key[i % keyLength];
        char encryptedChar;

        if (isalpha(plainChar)) {
            char base = isupper(plainChar) ? 'A' : 'a';
            encryptedChar = ((plainChar - base + keyChar - 'a') % 26) + base;
        } else {
            encryptedChar = plainChar;
        }

        ciphertext += encryptedChar;
    }

    return ciphertext;
}

string decryptVigenere(string ciphertext, string key) {
    string plaintext = "";
    int keyLength = key.length();

    for (int i = 0; i < ciphertext.length(); i++) {
        char cipherChar = ciphertext[i];
        char keyChar = key[i % keyLength];
        char decryptedChar;

        if (isalpha(cipherChar)) {
            char base = isupper(cipherChar) ? 'A' : 'a';
            decryptedChar = ((cipherChar - base - (keyChar - 'a') + 26) % 26) + base;
        } else {
            decryptedChar = cipherChar;
        }

        plaintext += decryptedChar;
    }

    return plaintext;
}

int main() {
    string plaintext, key;
    char choice;
    
    do {
        cout << "Enkripsi & Dekripsi Vigenere Cipher" << endl;

        cout << "Masukkan plaintext: ";
        getline(cin, plaintext);

        cout << "Masukkan kunci: ";
        cin >> key;

        cin.ignore(); // Membersihkan newline dari buffer input

        string ciphertext = encryptVigenere(plaintext, key);
        string decryptedText = decryptVigenere(ciphertext, key);

        cout << "Hasil " << endl;
        cout << "Teks Terenkripsi: " << ciphertext << endl;
        cout << "Teks Terdekripsi: " << decryptedText << endl;

        cout << "Apakah Anda ingin melanjutkan (y/n)? ";
        cin >> choice;
        cin.ignore(); // Membersihkan newline dari buffer input
    } while (choice == 'y' || choice == 'Y');

    return 0;
}