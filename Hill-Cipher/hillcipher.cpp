#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Fungsi untuk menghitung greatest common divisor (GCD)
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// Fungsi untuk mengenkripsi pesan
string encrypt(string plaintext, vector<vector<int>> keyMatrix) {
    string ciphertext = "";
    int keySize = keyMatrix.size();

    // Mengulangi pesan dengan langkah sesuai dengan ukuran kunci
    for (int i = 0; i < plaintext.length(); i += keySize) {
        string block = plaintext.substr(i, keySize);
        vector<int> blockVector(keySize);

        // Mengonversi blok pesan menjadi vektor bilangan bulat
        for (int j = 0; j < keySize; j++) {
            blockVector[j] = block[j] - 'A';
        }

        // Matriks perkalian dengan vektor blok pesan
        vector<int> result(keySize, 0);
        for (int j = 0; j < keySize; j++) {
            for (int k = 0; k < keySize; k++) {
                result[j] += keyMatrix[j][k] * blockVector[k];
            }
            result[j] %= 26;
        }

        // Mengonversi hasil perkalian kembali ke huruf
        for (int j = 0; j < keySize; j++) {
            ciphertext += static_cast<char>(result[j] + 'A');
        }
    }

    return ciphertext;
}

// Fungsi untuk mendekripsi pesan
string decrypt(string ciphertext, vector<vector<int>> keyMatrix) {
    string plaintext = "";
    int keySize = keyMatrix.size();

    // Menghitung invers modulo 26 dari determinan kunci matriks
    int determinant = 0;
    if (keySize == 2) {
        determinant = keyMatrix[0][0] * keyMatrix[1][1] - keyMatrix[0][1] * keyMatrix[1][0];
    } else if (keySize == 3) {
        determinant = keyMatrix[0][0] * (keyMatrix[1][1] * keyMatrix[2][2] - keyMatrix[1][2] * keyMatrix[2][1]) -
                     keyMatrix[0][1] * (keyMatrix[1][0] * keyMatrix[2][2] - keyMatrix[1][2] * keyMatrix[2][0]) +
                     keyMatrix[0][2] * (keyMatrix[1][0] * keyMatrix[2][1] - keyMatrix[1][1] * keyMatrix[2][0]);
    }

    int inverseDet = -1;
    for (int i = 1; i < 26; i++) {
        if ((determinant * i) % 26 == 1) {
            inverseDet = i;
            break;
        }
    }

    if (inverseDet == -1) {
        cout << "Determinan tidak memiliki invers modulo 26." << endl;
        return "";
    }

    // Menghitung matriks adjoint dan invers
    vector<vector<int>> adjoint(keySize, vector<int>(keySize));
    vector<vector<int>> inverse(keySize, vector<int>(keySize));

    for (int i = 0; i < keySize; i++) {
        for (int j = 0; j < keySize; j++) {
            int minor[2][2];
            int minorIdx = 0;

            for (int k = 0; k < keySize; k++) {
                for (int l = 0; l < keySize; l++) {
                    if (k != i && l != j) {
                        minor[minorIdx / 2][minorIdx % 2] = keyMatrix[k][l];
                        minorIdx++;
                    }
                }
            }

            int cofactor = minor[0][0] * minor[1][1] - minor[0][1] * minor[1][0];
            adjoint[j][i] = (cofactor * ((i + j) % 2 == 0 ? 1 : -1)) % 26;
            if (adjoint[j][i] < 0) {
                adjoint[j][i] += 26;
            }

            inverse[i][j] = (adjoint[j][i] * inverseDet) % 26;
            if (inverse[i][j] < 0) {
                inverse[i][j] += 26;
            }
        }
    }

    // Mengulangi pesan terenkripsi dengan langkah sesuai dengan ukuran kunci
    for (int i = 0; i < ciphertext.length(); i += keySize) {
        string block = ciphertext.substr(i, keySize);
        vector<int> blockVector(keySize);

        // Mengonversi blok pesan terenkripsi menjadi vektor bilangan bulat
        for (int j = 0; j < keySize; j++) {
            blockVector[j] = block[j] - 'A';
        }

        // Matriks perkalian dengan vektor blok pesan terenkripsi
        vector<int> result(keySize, 0);
        for (int j = 0; j < keySize; j++) {
            for (int k = 0; k < keySize; k++) {
                result[j] += inverse[j][k] * blockVector[k];
            }
            result[j] %= 26;
        }

        // Mengonversi hasil perkalian kembali ke huruf
        for (int j = 0; j < keySize; j++) {
            plaintext += static_cast<char>(result[j] + 'A');
        }
    }

    return plaintext;
}

// Fungsi untuk mencari kunci matriks
vector<vector<int>> findKeyMatrix(string plaintext, string ciphertext) {
    vector<vector<int>> keyMatrix;
    int keySize = 0;

    // Cek apakah panjang pesan cocok dengan ukuran kunci
    if (plaintext.length() == ciphertext.length() && plaintext.length() > 0) {
        keySize = plaintext.length();
    } else {
        cout << "Panjang pesan dan pesan terenkripsi harus sama dan lebih dari 0." << endl;
        return keyMatrix;
    }

    // Mengulangi setiap kemungkinan ukuran matriks
    for (int size = 2; size <= 3; size++) {
        if (keySize % size == 0) {
            keyMatrix.resize(size, vector<int>(size, 0));
            int blockCount = keySize / size;

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    int sum = 0;
                    for (int k = 0; k < blockCount; k++) {
                        sum += (plaintext[i * blockCount + k] - 'A') * (ciphertext[j * blockCount + k] - 'A');
                    }
                    keyMatrix[i][j] = sum % 26;
                }
            }

            // Mengecek apakah determinan kunci matriks adalah coprime dengan 26
            int determinant = 0;
            if (size == 2) {
                determinant = keyMatrix[0][0] * keyMatrix[1][1] - keyMatrix[0][1] * keyMatrix[1][0];
            } else if (size == 3) {
                determinant = keyMatrix[0][0] * (keyMatrix[1][1] * keyMatrix[2][2] - keyMatrix[1][2] * keyMatrix[2][1]) -
                             keyMatrix[0][1] * (keyMatrix[1][0] * keyMatrix[2][2] - keyMatrix[1][2] * keyMatrix[2][0]) +
                             keyMatrix[0][2] * (keyMatrix[1][0] * keyMatrix[2][1] - keyMatrix[1][1] * keyMatrix[2][0]);
            }

            if (determinant != 0 && gcd(determinant, 26) == 1) {
                return keyMatrix;
            }
        }
    }

    cout << "Tidak dapat menemukan kunci matriks yang valid." << endl;
    return keyMatrix;
}

int main() {
    string plaintext, ciphertext;
    int choice;
    char repeat;

    do {
        cout << "Hill Cipher - Program Enkripsi, Dekripsi, dan Pencarian Kunci Matriks" << endl;
        cout << "1. Enkripsi" << endl;
        cout << "2. Dekripsi" << endl;
        cout << "3. Cari Kunci Matriks" << endl;
        cout << "Pilih tindakan (1/2/3): ";
        cin >> choice;

        if (choice == 1) {
            cout << "Masukkan pesan yang akan dienkripsi (huruf kapital tanpa spasi): ";
            cin >> plaintext;
            cout << "Masukkan kunci matriks (misalnya, untuk matriks 2x2: a b c d): ";
            vector<vector<int>> keyMatrix(2, vector<int>(2));
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    cin >> keyMatrix[i][j];
                }
            }
            ciphertext = encrypt(plaintext, keyMatrix);
            cout << "Hasil enkripsi: " << ciphertext << endl;
        } else if (choice == 2) {
            cout << "Masukkan pesan yang akan didekripsi (huruf kapital tanpa spasi): ";
            cin >> ciphertext;
            cout << "Masukkan kunci matriks (misalnya, untuk matriks 2x2: a b c d): ";
            vector<vector<int>> keyMatrix(2, vector<int>(2));
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    cin >> keyMatrix[i][j];
                }
            }
            plaintext = decrypt(ciphertext, keyMatrix);
            cout << "Hasil dekripsi: " << plaintext << endl;
        } else if (choice == 3) {
            cout << "Masukkan pesan asli (huruf kapital tanpa spasi): ";
            cin >> plaintext;
            cout << "Masukkan pesan terenkripsi (huruf kapital tanpa spasi): ";
            cin >> ciphertext;
            vector<vector<int>> keyMatrix = findKeyMatrix(plaintext, ciphertext);
            if (!keyMatrix.empty()) {
                cout << "Kunci matriks yang mungkin:" << endl;
                for (int i = 0; i < keyMatrix.size(); i++) {
                    for (int j = 0; j < keyMatrix[i].size(); j++) {
                        cout << keyMatrix[i][j] << " ";
                    }
                    cout << endl;
                }
            }
        } else {
            cout << "Pilihan tidak valid." << endl;
        }

        cout << "Lakukan operasi lain? (y/n): ";
        cin >> repeat;
    } while (repeat == 'y' || repeat == 'Y');

    return 0;
}
