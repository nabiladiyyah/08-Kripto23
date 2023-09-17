#include <iostream>
#include <string>

using namespace std;

int inverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if (((a % m) * (x % m)) % m == 1) {
            return x;
        }
    }
    return -1; // Inverse tidak ditemukan
}

string enkripsi(string pesan, int a, int b) {
    string enkrip = "";
    int m = 26; 

    for (char &c : pesan) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            int x = c - base;
            int enkripHuruf = (a * x + b) % m;
            if (enkripHuruf < 0) {
                enkripHuruf += m; 
            }
            enkrip += (char)(enkripHuruf + base);
        } else {
            enkrip += c;
        }
    }
    return enkrip;
}

string dekripsi(string pesan, int a, int b) {
    string dekrip = "";
    int m = 26; 
    int aInverse = inverse(a, m);

    if (aInverse == -1) {
        cout << "Inverse modular tidak ditemukan. Dekripsi tidak mungkin." << endl;
        return dekrip;
    }

    for (char &c : pesan) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            int y = c - base;
            int dekripChar = (aInverse * (y - b)) % m;
            if (dekripChar < 0) {
                dekripChar += m; 
            }
            dekrip += (char)(dekripChar + base);
        } else {
            dekrip += c;
        }
    }
    return dekrip;
}

int main() {
    char pilihan = 'y';
    
    while (pilihan == 'y' || pilihan == 'Y') {
        string pesan;
        int a, b;

        cout << "Masukkan pesan yang ingin dienkripsi: ";
        cin.ignore();
        getline(cin, pesan);

        cout << "Masukkan nilai a: ";
        cin >> a;

        cout << "Masukkan nilai b: ";
        cin >> b;

        string pesanTerenkripsi = enkripsi(pesan, a, b);
        cout << "Pesan terenkripsi: " << pesanTerenkripsi << endl;

        string pesanTerdekripsi = dekripsi(pesanTerenkripsi, a, b);
        cout << "Pesan terdekripsi: " << pesanTerdekripsi << endl;

        cout << "Apakah Anda ingin melanjutkan? (y/n): ";
        cin >> pilihan;
    }

    return 0;
}