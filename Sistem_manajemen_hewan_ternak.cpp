#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;

// STRUKTUR DATA
struct Akun {
    string username;
    string password;
    bool isAdmin = false;
};

struct Hewan {
    int id;
    string nama;
    int umur;
    double berat;
};

struct Pengajuan {
    int id;
    string namaHewan;
    int umur;
    double berat;
    string usernamePengaju;
    string status;
    string timestamp;
};

// FUNGSI UTILITY
void bersihkanLayar() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

string bersihkanString(string str) {
    str.erase(remove(str.begin(), str.end(), '\"'), str.end());
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.erase(remove(str.begin(), str.end(), ','), str.end());
    return str;
}

string ekstrakValue(string line) {
    size_t posColon = line.find(":");
    if (posColon == string::npos) return "";
    string rawValue = line.substr(posColon + 1);
    return bersihkanString(rawValue);
}

string getTimestamp() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string timestamp(dt);
    timestamp.erase(remove(timestamp.begin(), timestamp.end(), '\n'), timestamp.end());
    return timestamp;
}

// FILE HANDLING - USERS
void simpanUsersKeFile(vector<Akun>& users) {
    ofstream file("users.json");
    if (file.is_open()) {
        file << "[\n";
        for (size_t i = 0; i < users.size(); i++) {
            file << "  {\n";
            file << "    \"username\": \"" << users[i].username << "\",\n";
            file << "    \"password\": \"" << users[i].password << "\",\n";
            file << "    \"isAdmin\": " << (users[i].isAdmin ? "true" : "false") << "\n";
            file << "  }";
            if (i < users.size() - 1) file << ",";
            file << "\n";
        }
        file << "]\n";
        file.close();
    }
}

void bacaUsersDariFile(vector<Akun>& users) {
    ifstream file("users.json");
    if (file.is_open()) {
        users.clear();
        string line;
        Akun temp;
        bool isNewEntry = false;

        while (getline(file, line)) {
            if (line.find("{") != string::npos) {
                isNewEntry = true;
                temp.username = ""; temp.password = ""; temp.isAdmin = false;
            }
            if (line.find("\"username\"") != string::npos) temp.username = ekstrakValue(line);
            if (line.find("\"password\"") != string::npos) temp.password = ekstrakValue(line);
            if (line.find("\"isAdmin\"") != string::npos) {
                string val = ekstrakValue(line);
                temp.isAdmin = (val == "true");
                if (isNewEntry && !temp.username.empty()) {
                    users.push_back(temp);
                    isNewEntry = false;
                }
            }
        }
        file.close();
    } else {
        Akun adminDefault;
        adminDefault.username = "Aril";
        adminDefault.password = "119";
        adminDefault.isAdmin = true;
        users.push_back(adminDefault);
        simpanUsersKeFile(users);
    }
}

// FILE HANDLING - TERNAK
void simpanDataKeFile(Hewan ternak[], int jumlah) {
    ofstream file("data_ternak.json");
    if (file.is_open()) {
        file << "[\n";
        for (int i = 0; i < jumlah; i++) {
            file << "  {\n";
            file << "    \"id\": " << ternak[i].id << ",\n";
            file << "    \"nama\": \"" << ternak[i].nama << "\",\n";
            file << "    \"umur\": " << ternak[i].umur << ",\n";
            file << "    \"berat\": " << ternak[i].berat << "\n";
            file << "  }";
            if (i < jumlah - 1) file << ",";
            file << "\n";
        }
        file << "]\n";
        file.close();
    }
}

void bacaDataDariFile(Hewan ternak[], int *jumlah) {
    ifstream file("data_ternak.json");
    if (file.is_open()) {
        *jumlah = 0;
        string line;
        while (getline(file, line)) {
            if (line.find("\"id\"") != string::npos) {
                string val = ekstrakValue(line);
                if(!val.empty()) ternak[*jumlah].id = stoi(val);
            }
            if (line.find("\"nama\"") != string::npos) {
                size_t firstQuote = line.find("\"", line.find(":"));
                size_t lastQuote = line.rfind("\"");
                if (firstQuote != string::npos && lastQuote != string::npos && lastQuote > firstQuote) {
                    ternak[*jumlah].nama = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                }
            }
            if (line.find("\"umur\"") != string::npos) {
                string val = ekstrakValue(line);
                if(!val.empty()) ternak[*jumlah].umur = stoi(val);
            }
            if (line.find("\"berat\"") != string::npos) {
                string val = ekstrakValue(line);
                if(!val.empty()) ternak[*jumlah].berat = stod(val);
                (*jumlah)++;
            }
        }
        file.close();
    }
}

// FILE HANDLING - PENGAJUAN
void simpanPengajuanKeFile(Pengajuan data[], int jumlah) {
    ofstream file("pengajuan.json");
    if (file.is_open()) {
        file << "[\n";
        for (int i = 0; i < jumlah; i++) {
            file << "  {\n";
            file << "    \"id\": " << data[i].id << ",\n";
            file << "    \"namaHewan\": \"" << data[i].namaHewan << "\",\n";
            file << "    \"umur\": " << data[i].umur << ",\n";
            file << "    \"berat\": " << fixed << setprecision(2) << data[i].berat << ",\n";
            file << "    \"usernamePengaju\": \"" << data[i].usernamePengaju << "\",\n";
            file << "    \"status\": \"" << data[i].status << "\",\n";
            file << "    \"timestamp\": \"" << data[i].timestamp << "\"\n";
            file << "  }";
            if (i < jumlah - 1) file << ",";
            file << "\n";
        }
        file << "]\n";
        file.close();
    }
}

void bacaPengajuanDariFile(Pengajuan data[], int *jumlah) {
    ifstream file("pengajuan.json");
    if (file.is_open()) {
        *jumlah = 0;
        string line;
        Pengajuan temp;
        bool isNewEntry = false;

        while (getline(file, line)) {
            if (line.find("{") != string::npos) {
                isNewEntry = true;
                temp = {};
                temp.status = "pending";
            }
            if (line.find("\"id\"") != string::npos && line.find("\"usernamePengaju\"") == string::npos) {
                string val = ekstrakValue(line);
                if(!val.empty()) temp.id = stoi(val);
            }
            if (line.find("\"namaHewan\"") != string::npos) {
                size_t firstQuote = line.find("\"", line.find(":"));
                size_t lastQuote = line.rfind("\"");
                if (firstQuote != string::npos && lastQuote != string::npos && lastQuote > firstQuote) {
                    temp.namaHewan = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                }
            }
            if (line.find("\"umur\"") != string::npos) {
                string val = ekstrakValue(line);
                if(!val.empty()) temp.umur = stoi(val);
            }
            if (line.find("\"berat\"") != string::npos && line.find("\"usernamePengaju\"") == string::npos) {
                string val = ekstrakValue(line);
                if(!val.empty()) temp.berat = stod(val);
            }
            if (line.find("\"usernamePengaju\"") != string::npos) {
                size_t firstQuote = line.find("\"", line.find(":"));
                size_t lastQuote = line.rfind("\"");
                if (firstQuote != string::npos && lastQuote != string::npos && lastQuote > firstQuote) {
                    temp.usernamePengaju = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                }
            }
            if (line.find("\"status\"") != string::npos) {
                size_t firstQuote = line.find("\"", line.find(":"));
                size_t lastQuote = line.rfind("\"");
                if (firstQuote != string::npos && lastQuote != string::npos && lastQuote > firstQuote) {
                    temp.status = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                }
            }
            if (line.find("\"timestamp\"") != string::npos) {
                size_t firstQuote = line.find("\"", line.find(":"));
                size_t lastQuote = line.rfind("\"");
                if (firstQuote != string::npos && lastQuote != string::npos && lastQuote > firstQuote) {
                    temp.timestamp = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                }
                if (isNewEntry) {
                    data[*jumlah] = temp;
                    (*jumlah)++;
                    isNewEntry = false;
                }
            }
        }
        file.close();
    }
}

// TAMPILAN DATA (TABEL RAPI)
void tampilDataTabel(Hewan ternak[], int jumlah) {
    if (jumlah == 0) {
        cout << "Belum ada data hewan ternak.\n";
        return;
    }
    cout << "\n+----+------------------------+------+------------+\n";
    cout << "| ID | Nama                   | Umur | Berat (kg) |\n";
    cout << "+----+------------------------+------+------------+\n";
    for (int i = 0; i < jumlah; i++) {
        cout << "| " << setw(2) << ternak[i].id << " | " 
             << left << setw(22) << ternak[i].nama << right
             << " | " << setw(4) << ternak[i].umur 
             << " | " << setw(10) << fixed << setprecision(2) << ternak[i].berat << " |\n";
    }
    cout << "+----+------------------------+------+------------+\n";
}

// CRUD TERNAK
void tambahData(Hewan ternak[], int *jumlah) {
    bersihkanLayar();
    cout << "\n=== Tambah Data Hewan ===\n";
    cout << "ID Hewan   : "; cin >> ternak[*jumlah].id;
    cout << "Nama Hewan : "; cin.ignore(); getline(cin, ternak[*jumlah].nama);
    cout << "Umur       : "; cin >> ternak[*jumlah].umur;
    cout << "Berat      : "; cin >> ternak[*jumlah].berat;
    (*jumlah)++;
    simpanDataKeFile(ternak, *jumlah);
    cout << "\nData berhasil ditambahkan!\n";
    cout << "Tekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void tampilData(Hewan ternak[], int jumlah) {
    bersihkanLayar();
    cout << "\nData Hewan Ternak\n";
    cout << "========================================\n";
    tampilDataTabel(ternak, jumlah);
    cout << "\nTekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void updateData(Hewan ternak[], int jumlah) {
    bersihkanLayar();
    int cari;
    cout << "Masukkan ID yang ingin diupdate: "; cin >> cari;
    for (int i = 0; i < jumlah; i++) {
        if (ternak[i].id == cari) {
            cout << "Nama baru : "; cin.ignore(); getline(cin, ternak[i].nama);
            cout << "Umur baru : "; cin >> ternak[i].umur;
            cout << "Berat baru: "; cin >> ternak[i].berat;
            simpanDataKeFile(ternak, jumlah);
            cout << "Data berhasil diupdate!\n";
            cout << "Tekan Enter untuk kembali..."; cin.ignore(); cin.get();
            bersihkanLayar();
            return;
        }
    }
    cout << "Data tidak ditemukan!\n";
    cout << "Tekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void hapusData(Hewan ternak[], int *jumlah) {
    bersihkanLayar();
    int cari;
    cout << "Masukkan ID yang ingin dihapus: "; cin >> cari;
    for (int i = 0; i < *jumlah; i++) {
        if (ternak[i].id == cari) {
            for (int j = i; j < *jumlah - 1; j++) ternak[j] = ternak[j + 1];
            (*jumlah)--;
            simpanDataKeFile(ternak, *jumlah);
            cout << "Data berhasil dihapus!\n";
            cout << "Tekan Enter untuk kembali..."; cin.ignore(); cin.get();
            bersihkanLayar();
            return;
        }
    }
    cout << "Data tidak ditemukan!\n";
    cout << "Tekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

// SORTING
void copyArray(Hewan src[], Hewan dest[], int n) {
    for(int i=0; i<n; i++) dest[i] = src[i];
}

void sortingNama(Hewan ternak[], int jumlah, bool ascend) {
    bersihkanLayar();
    Hewan temp[100]; copyArray(ternak, temp, jumlah);
    
    if(ascend) {
        sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.nama < b.nama; });
        cout << "Sorting: Nama (A-Z)\n\n";
    } else {
        sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.nama > b.nama; });
        cout << "Sorting: Nama (Z-A)\n\n";
    }
    tampilDataTabel(temp, jumlah);
    cout << "\nTekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void sortingUmur(Hewan ternak[], int jumlah, bool ascend) {
    bersihkanLayar();
    Hewan temp[100]; copyArray(ternak, temp, jumlah);
    
    if(ascend) {
        sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.umur < b.umur; });
        cout << "Sorting: Umur (Muda -> Tua)\n\n";
    } else {
        sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.umur > b.umur; });
        cout << "Sorting: Umur (Tua -> Muda)\n\n";
    }
    tampilDataTabel(temp, jumlah);
    cout << "\nTekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void sortingBerat(Hewan ternak[], int jumlah, bool ascend) {
    bersihkanLayar();
    Hewan temp[100]; copyArray(ternak, temp, jumlah);
    
    if(ascend) {
        sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.berat < b.berat; });
        cout << "Sorting: Berat (Ringan -> Berat)\n\n";
    } else {
        sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.berat > b.berat; });
        cout << "Sorting: Berat (Berat -> Ringan)\n\n";
    }
    tampilDataTabel(temp, jumlah);
    cout << "\nTekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void menuSorting(Hewan ternak[], int jumlah) {
    int pilih;
    do {
        bersihkanLayar();
        cout << "========================================\n";
        cout << "=        MENU SORTING DATA             =\n";
        cout << "========================================\n";
        cout << "1. Ascending  (A -> Z)\n";
        cout << "2. Descending (Z -> A)\n";
        cout << "3. Ascending  (Muda -> Tua)\n";
        cout << "4. Descending (Tua -> Muda)\n";
        cout << "5. Ascending  (Ringan -> Berat)\n";
        cout << "6. Descending (Berat -> Ringan)\n";
        cout << "7. Kembali\n";
        cout << "========================================\n";
        cout << "Pilih: "; cin >> pilih;

        if (pilih == 1) {
            sortingNama(ternak, jumlah, true);
        } else if (pilih == 2) {
            sortingNama(ternak, jumlah, false);
        } else if (pilih == 3) {
            sortingUmur(ternak, jumlah, true);
        } else if (pilih == 4) {
            sortingUmur(ternak, jumlah, false);
        } else if (pilih == 5) {
            sortingBerat(ternak, jumlah, true);
        } else if (pilih == 6) {
            sortingBerat(ternak, jumlah, false);
        } else if (pilih == 7) {
            return;
        } else {
            cout << "Pilihan tidak valid!\n";
            cin.ignore(); cin.get();
        }
    } while(true);
}

// SEARCHING
void cariData(Hewan ternak[], int jumlah) {
    bersihkanLayar();
    int cari;
    cout << "Masukkan ID yang dicari: "; cin >> cari;
    bool ditemukan = false;
    for (int i = 0; i < jumlah; i++) {
        if (ternak[i].id == cari) {
            cout << "\nData Ditemukan:\n";
            cout << "   ID    : " << ternak[i].id << endl;
            cout << "   Nama  : " << ternak[i].nama << endl;
            cout << "   Umur  : " << ternak[i].umur << " tahun" << endl;
            cout << "   Berat : " << ternak[i].berat << " kg" << endl;
            ditemukan = true;
            break;
        }
    }
    if (!ditemukan) cout << "Data dengan ID " << cari << " tidak ditemukan!\n";
    cout << "\nTekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

// CRUD PENGAJUAN - USER
void tambahPengajuan(Pengajuan data[], int *jumlah, string usernameUser) {
    bersihkanLayar();
    cout << "\n=== Tambah Pengajuan ===\n";
    cout << "ID Hewan   : "; cin >> data[*jumlah].id;
    cout << "Nama Hewan : "; cin.ignore(); getline(cin, data[*jumlah].namaHewan);
    cout << "Umur       : "; cin >> data[*jumlah].umur;
    cout << "Berat      : "; cin >> data[*jumlah].berat;
    
    data[*jumlah].usernamePengaju = usernameUser;
    data[*jumlah].status = "pending";
    data[*jumlah].timestamp = getTimestamp();
    
    (*jumlah)++;
    simpanPengajuanKeFile(data, *jumlah);
    
    cout << "\nPengajuan berhasil dikirim!\n";
    cout << "Status: Menunggu persetujuan admin\n";
    cout << "Tekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void tampilPengajuanUser(Pengajuan data[], int jumlah, string usernameUser) {
    bersihkanLayar();
    if (jumlah == 0) { 
        cout << "Belum ada pengajuan.\n"; 
        cout << "Tekan Enter..."; cin.ignore(); cin.get(); 
        bersihkanLayar(); 
        return; 
    }
    
    bool ada = false;
    cout << "\nData Pengajuan Anda:\n\n";
    for (int i = 0; i < jumlah; i++) {
        if (data[i].usernamePengaju == usernameUser) {
            ada = true;
            cout << "+---------------------+\n";
            cout << "| ID    : " << setw(5) << data[i].id << "        |\n";
            cout << "| Nama  : " << left << setw(15) << data[i].namaHewan << " |\n";
            cout << "| Umur  : " << data[i].umur << " tahun           |\n";
            cout << "| Berat : " << data[i].berat << " kg            |\n";
            cout << "| Status: " << data[i].status << "              |\n";
            cout << "+---------------------+\n";
        }
    }
    if (!ada) cout << "Anda belum memiliki pengajuan.\n";
    cout << "\nTekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void updatePengajuan(Pengajuan data[], int jumlah, string usernameUser) {
    bersihkanLayar();
    int cari; 
    cout << "Masukkan ID pengajuan: "; cin >> cari;
    for (int i = 0; i < jumlah; i++) {
        if (data[i].id == cari && data[i].usernamePengaju == usernameUser) {
            if (data[i].status != "pending") {
                cout << "Pengajuan dengan status '" << data[i].status << "' tidak dapat diubah.\n";
                cout << "Tekan Enter..."; cin.ignore(); cin.get(); 
                bersihkanLayar();
                return;
            }
            cout << "Nama baru : "; cin.ignore(); getline(cin, data[i].namaHewan);
            cout << "Umur baru : "; cin >> data[i].umur;
            cout << "Berat baru: "; cin >> data[i].berat;
            data[i].timestamp = getTimestamp();
            simpanPengajuanKeFile(data, jumlah);
            cout << "Berhasil diupdate!\n";
            cout << "Tekan Enter..."; cin.ignore(); cin.get(); 
            bersihkanLayar(); 
            return;
        }
    }
    cout << "Data tidak ditemukan!\n"; 
    cout << "Tekan Enter..."; cin.ignore(); cin.get(); 
    bersihkanLayar();
}

void hapusPengajuan(Pengajuan data[], int *jumlah, string usernameUser) {
    bersihkanLayar();
    int cari; 
    cout << "Masukkan ID: "; cin >> cari;
    for (int i = 0; i < *jumlah; i++) {
        if (data[i].id == cari && data[i].usernamePengaju == usernameUser) {
            if (data[i].status != "pending") {
                cout << "Pengajuan dengan status '" << data[i].status << "' tidak dapat dihapus.\n";
                cout << "Tekan Enter..."; cin.ignore(); cin.get(); 
                bersihkanLayar();
                return;
            }
            for (int j = i; j < *jumlah - 1; j++) data[j] = data[j + 1];
            (*jumlah)--;
            simpanPengajuanKeFile(data, *jumlah);
            cout << "Pengajuan dihapus!\n";
            cout << "Tekan Enter..."; cin.ignore(); cin.get(); 
            bersihkanLayar(); 
            return;
        }
    }
    cout << "Data tidak ditemukan!\n"; 
    cout << "Tekan Enter..."; cin.ignore(); cin.get(); 
    bersihkanLayar();
}

// FUNGSI ADMIN: KELOLA PENGAJUAN
void kelolaPengajuan(Pengajuan data[], int *jumlah, Hewan ternak[], int *jumlahTernak) {
    bersihkanLayar();
    cout << "\n=== Kelola Pengajuan Masuk ===\n\n";
    
    int countPending = 0;
    for (int i = 0; i < *jumlah; i++) {
        if (data[i].status == "pending") {
            countPending++;
            cout << "[" << countPending << "] ID: " << data[i].id << "\n";
            cout << "    Hewan   : " << data[i].namaHewan << "\n";
            cout << "    Umur    : " << data[i].umur << " tahun\n";
            cout << "    Berat   : " << data[i].berat << " kg\n";
            cout << "    Pengaju : " << data[i].usernamePengaju << "\n";
            cout << "    Waktu   : " << data[i].timestamp << "\n";
            cout << "    --------------------------\n";
        }
    }
    
    if (countPending == 0) {
        cout << "Tidak ada pengajuan pending.\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }
    
    int pilihan;
    cout << "\nMasukkan nomor pengajuan yang ingin diproses (0 untuk batal): ";
    cin >> pilihan;
    
    if (pilihan == 0) {
        bersihkanLayar();
        return;
    }
    
    if (pilihan < 1 || pilihan > countPending) {
        cout << "Pilihan tidak valid!\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }
    
    int idx = -1, counter = 0;
    for (int i = 0; i < *jumlah; i++) {
        if (data[i].status == "pending") {
            counter++;
            if (counter == pilihan) {
                idx = i;
                break;
            }
        }
    }
    
    if (idx == -1) return;
    
    int aksi;
    cout << "\nApa yang ingin Anda lakukan?\n";
    cout << "1. Terima Pengajuan\n";
    cout << "2. Tolak Pengajuan\n";
    cout << "Pilih: "; cin >> aksi;
    
    if (aksi == 1) {
        ternak[*jumlahTernak].id = data[idx].id;
        ternak[*jumlahTernak].nama = data[idx].namaHewan;
        ternak[*jumlahTernak].umur = data[idx].umur;
        ternak[*jumlahTernak].berat = data[idx].berat;
        (*jumlahTernak)++;
        
        data[idx].status = "diterima";
        simpanDataKeFile(ternak, *jumlahTernak);
        simpanPengajuanKeFile(data, *jumlah);
        
        cout << "\nPengajuan DITERIMA! Data hewan telah ditambahkan.\n";
    } 
    else if (aksi == 2) {
        data[idx].status = "ditolak";
        simpanPengajuanKeFile(data, *jumlah);
        cout << "\nPengajuan DITOLAK.\n";
    } 
    else {
        cout << "Pilihan tidak valid!\n";
    }
    
    cout << "Tekan Enter untuk kembali..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void tampilRiwayatPengajuan(Pengajuan data[], int jumlah) {
    bersihkanLayar();
    cout << "\n=== Riwayat Semua Pengajuan ===\n\n";
    if (jumlah == 0) {
        cout << "Belum ada riwayat pengajuan.\n";
    } else {
        for (int i = 0; i < jumlah; i++) {
            cout << "[" << data[i].status << "] ID:" << data[i].id 
                 << " | " << data[i].namaHewan 
                 << " | Pengaju: " << data[i].usernamePengaju 
                 << " | " << data[i].timestamp << "\n";
        }
    }
    cout << "\nTekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

// LOGIN SYSTEM
void registerAkun(vector<Akun>& users) {
    bersihkanLayar();
    cout << "\n=== REGISTER (USER) ===\n";
    string username, password;
    cout << "Buat Username : "; cin >> username;
    for (const auto& user : users) {
        if (user.username == username) {
            cout << "Username sudah digunakan!\n";
            cout << "Tekan Enter..."; cin.ignore(); cin.get(); bersihkanLayar(); return;
        }
    }
    cout << "Buat Password : "; cin >> password;
    Akun newUser;
    newUser.username = username; newUser.password = password; newUser.isAdmin = false;
    users.push_back(newUser);
    simpanUsersKeFile(users);
    cout << "Register berhasil!\n";
    cout << "Tekan Enter..."; cin.ignore(); cin.get(); bersihkanLayar();
}

bool login(vector<Akun>& users, bool &isAdmin, string &loggedInUser) {
    bersihkanLayar();
    string u, p; int kesempatan = 3;
    while (kesempatan > 0) {
        cout << "\n=== LOGIN ===\n";
        cout << "Username : "; cin >> u;
        cout << "Password : "; cin >> p;
        for (const auto& user : users) {
            if (user.username == u && user.password == p) {
                cout << "Login berhasil sebagai " << (user.isAdmin ? "ADMIN" : "USER") << "!\n";
                isAdmin = user.isAdmin;
                loggedInUser = user.username;
                return true;
            }
        }
        kesempatan--;
        cout << "Salah! Sisa kesempatan: " << kesempatan << endl;
    }
    return false;
}

// MAIN
int main() {
    bersihkanLayar();
    vector<Akun> users;
    Hewan ternak[100];
    Pengajuan pengajuan[100];
    int jumlah = 0, jumlahPengajuan = 0;
    string currentUser = "";

    bacaUsersDariFile(users);
    bacaDataDariFile(ternak, &jumlah);
    bacaPengajuanDariFile(pengajuan, &jumlahPengajuan);

    int menuAwal; bool sudahLogin = false, isAdmin = false;

    do {
        cout << "\n========================================\n";
        cout << "=    SISTEM MANAJEMEN HEWAN TERNAK     =\n";
        cout << "========================================\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Keluar\n";
        cout << "========================================\n";
        cout << "Pilih: "; cin >> menuAwal; cin.ignore();

        if (menuAwal == 1) {
            registerAkun(users);
        } else if (menuAwal == 2) {
            sudahLogin = login(users, isAdmin, currentUser);
            if (sudahLogin) {
                bersihkanLayar();
                if (isAdmin) {
                    int pilihan;
                    do {
                        cout << "\n========================================\n";
                        cout << "=             MENU ADMIN               =\n";
                        cout << "========================================\n";
                        cout << "1. Tambah Data hewan\n";
                        cout << "2. Tampilkan Data hewan\n";
                        cout << "3. Update Data hewan\n";
                        cout << "4. Hapus Data hewan\n";
                        cout << "5. Cari Data (ID) hewan\n";
                        cout << "6. Menu Sorting (Nama/Umur/Berat) hewan\n";
                        cout << "7. Kelola Pengajuan (Terima/Tolak)\n";
                        cout << "8. Lihat Riwayat Pengajuan\n";
                        cout << "9. Logout\n";
                        cout << "========================================\n";
                        cout << "Pilih: "; cin >> pilihan;

                        if (pilihan == 1) tambahData(ternak, &jumlah);
                        else if (pilihan == 2) tampilData(ternak, jumlah);
                        else if (pilihan == 3) updateData(ternak, jumlah);
                        else if (pilihan == 4) hapusData(ternak, &jumlah);
                        else if (pilihan == 5) cariData(ternak, jumlah);
                        else if (pilihan == 6) menuSorting(ternak, jumlah);
                        else if (pilihan == 7) kelolaPengajuan(pengajuan, &jumlahPengajuan, ternak, &jumlah);
                        else if (pilihan == 8) tampilRiwayatPengajuan(pengajuan, jumlahPengajuan);
                        else if (pilihan == 9) cout << "Logout...\n";
                        else { cout << "Pilihan salah!\n"; cin.ignore(); cin.get(); }
                    } while (pilihan != 9);
                    bersihkanLayar();
                } else {
                    int pilihan;
                    do {
                        cout << "\n========================================\n";
                        cout << "=               MENU USER              =\n";
                        cout << "========================================\n";
                        cout << "1. Tambah Pengajuan\n";
                        cout << "2. Lihat Data Ternak\n";
                        cout << "3. Lihat Pengajuan Saya\n";
                        cout << "4. Update Pengajuan\n";
                        cout << "5. Hapus Pengajuan\n";
                        cout << "6. Logout\n";
                        cout << "========================================\n";
                        cout << "Pilih: "; cin >> pilihan;

                        if (pilihan == 1) tambahPengajuan(pengajuan, &jumlahPengajuan, currentUser);
                        else if (pilihan == 2) tampilData(ternak, jumlah);
                        else if (pilihan == 3) tampilPengajuanUser(pengajuan, jumlahPengajuan, currentUser);
                        else if (pilihan == 4) updatePengajuan(pengajuan, jumlahPengajuan, currentUser);
                        else if (pilihan == 5) hapusPengajuan(pengajuan, &jumlahPengajuan, currentUser);
                        else if (pilihan == 6) cout << "Logout...\n";
                        else { cout << "Pilihan salah!\n"; cin.ignore(); cin.get(); }
                    } while (pilihan != 6);
                    bersihkanLayar();
                }
            }
        } else if (menuAwal == 3) {
            bersihkanLayar();
            cout << "Program selesai. Terima kasih!\n";
        } else {
            cout << "Pilihan tidak tersedia!\n"; cin.ignore(); cin.get();
        }
    } while (menuAwal != 3);
    return 0;
}