#include <iostream>
#include <string>
using namespace std;

struct Akun {
    string username;
    string password;
};

struct Hewan {
    int id;
    string nama;
    int umur;
    double berat;
};

void tambahData(Hewan ternak[], int *jumlah) {
    cout << "\n=== Tambah Data Hewan ===" << endl;
    cout << "ID Hewan   : ";
    cin >> ternak[*jumlah].id;

    cout << "Nama Hewan : ";
    cin.ignore();
    getline(cin, ternak[*jumlah].nama);

    cout << "Umur (tahun): ";
    cin >> ternak[*jumlah].umur;

    cout << "Berat (kg) : ";
    cin >> ternak[*jumlah].berat;

    (*jumlah)++;
    cout << "Data berhasil ditambahkan!\n";
}

void tampilData(Hewan ternak[], int index, int *jumlah) {
    if (index == *jumlah) return;

    cout << "\nData ke-" << index + 1 << endl;
    cout << "ID Hewan   : " << ternak[index].id << endl;
    cout << "Nama Hewan : " << ternak[index].nama << endl;
    cout << "Umur       : " << ternak[index].umur << " tahun" << endl;
    cout << "Berat      : " << ternak[index].berat << " kg" << endl;

    tampilData(ternak, index + 1, jumlah);
}

void updateData(Hewan ternak[], int *jumlah) {
    int idCari;
    bool ditemukan = false;

    cout << "\nMasukkan ID hewan yang ingin diupdate: ";
    cin >> idCari;

    for (int i = 0; i < *jumlah; i++) {
        if (ternak[i].id == idCari) {
            cout << "Nama baru : ";
            cin.ignore();
            getline(cin, ternak[i].nama);

            cout << "Umur baru : ";
            cin >> ternak[i].umur;

            cout << "Berat baru: ";
            cin >> ternak[i].berat;

            cout << "Data berhasil diupdate!\n";
            ditemukan = true;
        }
    }

    if (!ditemukan) cout << "Data tidak ditemukan!\n";
}

void hapusData(Hewan ternak[], int *jumlah) {
    int idHapus;
    bool ditemukan = false;

    cout << "\nMasukkan ID hewan yang mati atau terjual: ";
    cin >> idHapus;

    for (int i = 0; i < *jumlah; i++) {
        if (ternak[i].id == idHapus) {
            for (int j = i; j < *jumlah - 1; j++) {
                ternak[j] = ternak[j + 1];
            }
            (*jumlah)--;
            ditemukan = true;

            cout << "Data berhasil dihapus!\n";
            break;
        }
    }

    if (!ditemukan) cout << "Data tidak ditemukan!\n";
}

// Linear Search (ID)
void cariID(Hewan ternak[], int *jumlah) {
    int idCari;
    bool ditemukan = false;

    cout << "\nMasukkan ID yang dicari: ";
    cin >> idCari;

    for (int i = 0; i < *jumlah; i++) {
        if (ternak[i].id == idCari) {
            cout << "\nData ditemukan!\n";
            cout << "ID Hewan   : " << ternak[i].id << endl;
            cout << "Nama Hewan : " << ternak[i].nama << endl;
            cout << "Umur       : " << ternak[i].umur << " tahun" << endl;
            cout << "Berat      : " << ternak[i].berat << " kg" << endl;
            ditemukan = true;
            break;
        }
    }

    if (!ditemukan) cout << "Data tidak ditemukan!\n";
}

// Sort Nama Ascending
void sortNamaAscending(Hewan ternak[], int jumlah) {
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (ternak[j].nama > ternak[j + 1].nama) {
                Hewan temp = ternak[j];
                ternak[j] = ternak[j + 1];
                ternak[j + 1] = temp;
            }
        }
    }
}

// Binary Search (Nama)
void cariNama(Hewan ternak[], int jumlah) {
    string namaCari;
    cout << "\nMasukkan nama yang dicari: ";
    cin.ignore();
    getline(cin, namaCari);

    int kiri = 0, kanan = jumlah - 1;
    bool ditemukan = false;

    while (kiri <= kanan) {
        int tengah = (kiri + kanan) / 2;

        if (ternak[tengah].nama == namaCari) {
            cout << "\nData ditemukan!\n";
            cout << "ID Hewan   : " << ternak[tengah].id << endl;
            cout << "Nama Hewan : " << ternak[tengah].nama << endl;
            cout << "Umur       : " << ternak[tengah].umur << " tahun" << endl;
            cout << "Berat      : " << ternak[tengah].berat << " kg" << endl;
            ditemukan = true;
            break;
        }
        else if (ternak[tengah].nama < namaCari)
            kiri = tengah + 1;
        else
            kanan = tengah - 1;
    }

    if (!ditemukan) cout << "Data tidak ditemukan!\n";
}

void sortNamaDescending(Hewan ternak[], int jumlah) {
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (ternak[j].nama < ternak[j + 1].nama) {
                Hewan temp = ternak[j];
                ternak[j] = ternak[j + 1];
                ternak[j + 1] = temp;
            }
        }
    }
    cout << "Sorting nama (Z-A) selesai!\n";
}

void sortUmurAscending(Hewan ternak[], int jumlah) {
    for (int i = 0; i < jumlah - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < jumlah; j++) {
            if (ternak[j].umur < ternak[minIndex].umur) {
                minIndex = j;
            }
        }
        Hewan temp = ternak[i];
        ternak[i] = ternak[minIndex];
        ternak[minIndex] = temp;
    }
    cout << "Sorting umur (Ascending) selesai!\n";
}

void sortBeratDescending(Hewan ternak[], int jumlah) {
    for (int i = 1; i < jumlah; i++) {
        Hewan key = ternak[i];
        int j = i - 1;

        while (j >= 0 && ternak[j].berat < key.berat) {
            ternak[j + 1] = ternak[j];
            j--;
        }
        ternak[j + 1] = key;
    }
    cout << "Sorting berat (Descending) selesai!\n";
}

int main() {
    Akun user;
    Hewan ternak[100];
    int jumlah = 0;

    string username, password;
    int kesempatan = 3;
    int pilihan;

    cout << "=== REGISTRASI AKUN ===" << endl;
    cout << "Buat Username            : ";
    cin >> user.username;
    cout << "Buat Password (3 digit NIM): ";
    cin >> user.password;

    cout << "\nAkun berhasil dibuat!\n\n";

    while (kesempatan > 0) {
        cout << "=== LOGIN ===" << endl;
        cout << "Username : ";
        cin >> username;
        cout << "Password : ";
        cin >> password;

        if (username == user.username && password == user.password) {
            cout << "\nLogin berhasil!\n";
            break;
        } else {
            kesempatan--;
            cout << "\nLogin gagal! Sisa percobaan: " << kesempatan << endl;
        }

        if (kesempatan == 0) {
            cout << "\nProgram berhenti.\n";
            return 0;
        }
    }

    do {
        cout << "\n=== MENU DATA HEWAN TERNAK ===" << endl;
        cout << "1. Tambah Data" << endl;
        cout << "2. Tampilkan Data" << endl;
        cout << "3. Update Data" << endl;
        cout << "4. Hapus Data" << endl;
        cout << "5. Sorting Nama (Z-A)" << endl;
        cout << "6. Sorting Umur (Ascending)" << endl;
        cout << "7. Sorting Berat (Descending)" << endl;
        cout << "8. Cari ID (Linear Search)" << endl;
        cout << "9. Cari Nama (Binary Search)" << endl;
        cout << "10. Keluar" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        if (pilihan == 1) tambahData(ternak, &jumlah);
        else if (pilihan == 2) {
            if (jumlah == 0) cout << "Belum ada data.\n";
            else tampilData(ternak, 0, &jumlah);
        }
        else if (pilihan == 3) updateData(ternak, &jumlah);
        else if (pilihan == 4) hapusData(ternak, &jumlah);
        else if (pilihan == 5) sortNamaDescending(ternak, jumlah);
        else if (pilihan == 6) sortUmurAscending(ternak, jumlah);
        else if (pilihan == 7) sortBeratDescending(ternak, jumlah);
        else if (pilihan == 8) cariID(ternak, &jumlah);
        else if (pilihan == 9) {
            sortNamaAscending(ternak, jumlah);
            cariNama(ternak, jumlah);
        }
        else if (pilihan == 10)
            cout << "\nTerima kasih!\n";
        else
            cout << "Pilihan tidak tersedia!\n";

    } while (pilihan != 10);

    return 0;
}