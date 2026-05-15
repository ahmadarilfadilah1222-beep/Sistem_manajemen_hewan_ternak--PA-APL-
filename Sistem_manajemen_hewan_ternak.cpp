#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <ctime>
#include <limits>
#include <cctype>
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
    try {
        str.erase(remove(str.begin(), str.end(), '\"'), str.end());
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        str.erase(remove(str.begin(), str.end(), ','), str.end());
        return str;
    } catch (...) { return ""; }
}

string ekstrakValue(string line) {
    try {
        size_t posColon = line.find(":");
        if (posColon == string::npos) return "";
        string rawValue = line.substr(posColon + 1);
        return bersihkanString(rawValue);
    } catch (...) { return ""; }
}

string getTimestamp() {
    try {
        time_t now = time(0);
        char* dt = ctime(&now);
        string timestamp(dt);
        timestamp.erase(remove(timestamp.begin(), timestamp.end(), '\n'), timestamp.end());
        return timestamp;
    } catch (...) { return "Unknown"; }
}

bool isOnlyWhitespace(const string& str) {
    for (char c : str) {
        if (!isspace(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

// FUNGSI VALIDASI INPUT
bool inputIntValid(int &output, const string &pesan) {
    string input;
    while (true) {
        cout << pesan;
        if (!getline(cin, input)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (input.empty() || isOnlyWhitespace(input)) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        bool valid = true; size_t start = (input[0] == '-') ? 1 : 0;
        if (start == input.length()) valid = false;
        for (size_t i = start; i < input.length(); i++) if (!isdigit(static_cast<unsigned char>(input[i]))) { valid = false; break; }
        if (!valid) { cout << "Harap masukkan angka yang valid!\n"; continue; }
        try { output = stoi(input); return true; } catch (...) { cout << "Input tidak valid!\n"; }
    }
}

bool inputDoubleValid(double &output, const string &pesan) {
    string input;
    while (true) {
        cout << pesan;
        if (!getline(cin, input)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (input.empty() || isOnlyWhitespace(input)) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        try { output = stod(input); return true; } catch (...) { cout << "Harap masukkan angka desimal yang valid!\n"; }
    }
}

string inputStringNotEmpty(const string &pesan, int minLen = 1) {
    string input;
    while (true) {
        cout << pesan;
        if (!getline(cin, input)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (input.empty() || isOnlyWhitespace(input)) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        string trimmed = input;
        trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
        trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
        if ((int)trimmed.length() < minLen) {
            cout << "Input minimal " << minLen << " karakter!\n";
            continue;
        }
        replace(input.begin(), input.end(), '"', '\'');
        replace(input.begin(), input.end(), '\\', '/');
        return input;
    }
}

bool inputPilihanValid(int &output, const string &pesan, int min, int max) {
    string input;
    while (true) {
        cout << pesan;
        if (!getline(cin, input)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (input.empty() || isOnlyWhitespace(input)) {
            cout << "Pilihan tidak boleh kosong!\n";
            continue;
        }
        bool isNum = true;
        for (char c : input) if (!isdigit(static_cast<unsigned char>(c))) { isNum = false; break; }
        if (!isNum) { cout << "Harap masukkan angka antara " << min << "-" << max << "!\n"; continue; }
        try {
            output = stoi(input);
            if (output >= min && output <= max) return true;
            else cout << "Pilihan diluar range! Masukkan " << min << "-" << max << "\n";
        } catch (...) { cout << "Input tidak valid!\n"; }
    }
}

bool cekIdSudahAda(Hewan ternak[], int jumlah, int idCari) {
    for (int i = 0; i < jumlah; i++) if (ternak[i].id == idCari) return true;
    return false;
}

bool validasiRangeHewan(int umur, double berat, string &pesanError) {
    if (umur < 0 || umur > 50) { pesanError = "Umur tidak realistis (0-50 tahun)."; return false; }
    if (berat <= 0.0 || berat > 2000.0) { pesanError = "Berat tidak realistis (0.1-2000 kg)."; return false; }
    return true;
}

// FILE HANDLING - USERS
void simpanUsersKeFile(vector<Akun>& users) {
    ofstream file("users.json");
    if (!file.is_open()) { cout << "Gagal membuka file users.json!\n"; return; }
    try {
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
    } catch (const exception& e) { cout << "Gagal menyimpan data user.\n"; }
}

void bacaUsersDariFile(vector<Akun>& users) {
    ifstream file("users.json");
    users.clear();
    bool fileCorrupt = false;
    
    if (!file.is_open()) {
        cout << "File users.json tidak ditemukan. Membuat data default...\n";
        fileCorrupt = true;
    } else {
        try {
            string line; Akun temp; bool isNewEntry = false;
            while (getline(file, line)) {
                try {
                    if (line.find("{") != string::npos) { isNewEntry = true; temp = {}; }
                    if (line.find("\"username\"") != string::npos) temp.username = ekstrakValue(line);
                    if (line.find("\"password\"") != string::npos) temp.password = ekstrakValue(line);
                    if (line.find("\"isAdmin\"") != string::npos) {
                        string val = ekstrakValue(line);
                        temp.isAdmin = (val == "true");
                        if (isNewEntry && !temp.username.empty()) { users.push_back(temp); isNewEntry = false; }
                    }
                } catch (...) { fileCorrupt = true; continue; }
            }
        } catch (const exception& e) { cout << "Gagal membaca file users.json.\n"; fileCorrupt = true; }
        file.close();
    }
    
    if (fileCorrupt || users.empty()) {
        cout << "Data user tidak valid. Reset ke default...\n";
        users = {{"Aril", "119", true}, {"Bintang", "132", true}, {"Dirga", "115", true}, {"Dika", "130", true}};
        simpanUsersKeFile(users);
    }
}

// FILE HANDLING - TERNAK
bool simpanDataKeFile(Hewan ternak[], int jumlah) {
    ofstream file("data_ternak.json");
    if (!file.is_open()) { cout << "Gagal membuka file data_ternak.json!\n"; return false; }
    try {
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
        return true;
    } catch (const exception& e) { cout << "Gagal menyimpan data ternak.\n"; return false; }
}

void bacaDataDariFile(Hewan ternak[], int *jumlah) {
    *jumlah = 0;
    ifstream file("data_ternak.json");
    if (!file.is_open()) { cout << "File data_ternak.json tidak ditemukan.\n"; return; }
    try {
        string line;
        while (getline(file, line) && *jumlah < 100) {
            try {
                if (line.find("\"id\"") != string::npos) { string val = ekstrakValue(line); if(!val.empty()) ternak[*jumlah].id = stoi(val); }
                if (line.find("\"nama\"") != string::npos) { size_t f=line.find("\"",line.find(":")), l=line.rfind("\""); if(f!=string::npos && l!=string::npos && l>f) ternak[*jumlah].nama=line.substr(f+1,l-f-1); }
                if (line.find("\"umur\"") != string::npos) { string val = ekstrakValue(line); if(!val.empty()) ternak[*jumlah].umur = stoi(val); }
                if (line.find("\"berat\"") != string::npos) { string val = ekstrakValue(line); if(!val.empty()) ternak[*jumlah].berat = stod(val); (*jumlah)++; }
            } catch (...) { continue; }
        }
    } catch (const exception& e) { cout << "Gagal membaca data ternak.\n"; }
    file.close();
}

// FILE HANDLING - PENGAJUAN
bool simpanPengajuanKeFile(Pengajuan data[], int jumlah) {
    ofstream file("pengajuan.json");
    if (!file.is_open()) { cout << "Gagal membuka file pengajuan.json!\n"; return false; }
    try {
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
        return !file.fail();
    } catch (const exception& e) { cout << "Gagal menyimpan data pengajuan.\n"; return false; }
}

void bacaPengajuanDariFile(Pengajuan data[], int *jumlah) {
    *jumlah = 0;
    ifstream file("pengajuan.json");
    if (!file.is_open()) { cout << "File pengajuan.json tidak ditemukan.\n"; return; }
    try {
        string line; Pengajuan temp; bool isNewEntry = false;
        while (getline(file, line) && *jumlah < 100) {
            try {
                if (line.find("{") != string::npos) { isNewEntry = true; temp = {}; temp.status = "pending"; }
                if (line.find("\"id\"") != string::npos && line.find("\"usernamePengaju\"") == string::npos) { string val=ekstrakValue(line); if(!val.empty()) temp.id=stoi(val); }
                if (line.find("\"namaHewan\"") != string::npos) { size_t f=line.find("\"",line.find(":")), l=line.rfind("\""); if(f!=string::npos&&l!=string::npos&&l>f) temp.namaHewan=line.substr(f+1,l-f-1); }
                if (line.find("\"umur\"") != string::npos) { string val=ekstrakValue(line); if(!val.empty()) temp.umur=stoi(val); }
                if (line.find("\"berat\"") != string::npos && line.find("\"usernamePengaju\"") == string::npos) { string val=ekstrakValue(line); if(!val.empty()) temp.berat=stod(val); }
                if (line.find("\"usernamePengaju\"") != string::npos) { size_t f=line.find("\"",line.find(":")), l=line.rfind("\""); if(f!=string::npos&&l!=string::npos&&l>f) temp.usernamePengaju=line.substr(f+1,l-f-1); }
                if (line.find("\"status\"") != string::npos) { size_t f=line.find("\"",line.find(":")), l=line.rfind("\""); if(f!=string::npos&&l!=string::npos&&l>f) temp.status=line.substr(f+1,l-f-1); }
                if (line.find("\"timestamp\"") != string::npos) { size_t f=line.find("\"",line.find(":")), l=line.rfind("\""); if(f!=string::npos&&l!=string::npos&&l>f) temp.timestamp=line.substr(f+1,l-f-1); if(isNewEntry) { data[*jumlah]=temp; (*jumlah)++; isNewEntry=false; } }
            } catch (...) { continue; }
        }
    } catch (const exception& e) { cout << "Gagal membaca data pengajuan.\n"; }
    file.close();
}

// TAMPILAN DATA TABEL
void tampilDataTabel(Hewan ternak[], int jumlah) {
    if (jumlah == 0) { cout << "Belum ada data hewan ternak.\n"; return; }
    cout << "\n+----+------------------------+------+------------+\n";
    cout << "| ID | Nama                   | Umur | Berat (kg) |\n";
    cout << "+----+------------------------+------+------------+\n";
    for (int i = 0; i < jumlah; i++) {
        cout << "| " << setw(2) << ternak[i].id << " | " << left << setw(22) << ternak[i].nama << right
             << " | " << setw(4) << ternak[i].umur << " | " << setw(10) << fixed << setprecision(2) << ternak[i].berat << " |\n";
    }
    cout << "+----+------------------------+------+------------+\n";
}

// Helper untuk menampilkan tabel pengajuan user yang rapi
void tampilTabelPengajuanUser(Pengajuan data[], int jumlah, string usernameUser) {
    cout << "\n+----+------------------------+------+------------+----------+\n";
    cout << "| ID | Nama Hewan             | Umur | Berat (kg) | Status   |\n";
    cout << "+----+------------------------+------+------------+----------+\n";
    
    bool ada = false;
    for (int i = 0; i < jumlah; i++) {
        if (data[i].usernamePengaju == usernameUser) {
            ada = true;
            cout << "| " << setw(2) << data[i].id << " | " << left << setw(22) << data[i].namaHewan << right
                 << " | " << setw(4) << data[i].umur 
                 << " | " << setw(10) << fixed << setprecision(2) << data[i].berat
                 << " | " << setw(8) << data[i].status << " |\n";
        }
    }
    cout << "+----+------------------------+------+------------+----------+\n";
    
    if (!ada) {
        cout << "Anda belum memiliki pengajuan.\n";
    }
}

// CRUD TERNAK
void tambahData(Hewan ternak[], int *jumlah) {
    bersihkanLayar();
    cout << "\n========================================\n";
    cout << "=           TAMBAH DATA HEWAN          =\n";
    cout << "========================================\n";

    if (*jumlah >= 100) {
        cout << "Kapasitas data penuh (maks 100)!\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    // Input ID dengan validasi ketat & cek duplikat
    int idBaru;
    bool idValid = false;
    while (!idValid) {
        cout << "\nID Hewan   : ";
        string inputId;
        if (!getline(cin, inputId)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        
        if (inputId.empty() || isOnlyWhitespace(inputId)) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        
        bool isNum = true;
        for (char c : inputId) if (!isdigit(static_cast<unsigned char>(c))) { isNum = false; break; }
        if (!isNum) { cout << "Harap masukkan angka yang valid!\n"; continue; }
        
        try { idBaru = stoi(inputId); } catch (...) { cout << "Input tidak valid!\n"; continue; }

        if (cekIdSudahAda(ternak, *jumlah, idBaru)) {
            cout << "ID " << idBaru << " sudah terdaftar! Silakan masukkan ID lain.\n";
        } else {
            idValid = true;
        }
    }

    string namaBaru = inputStringNotEmpty("Nama Hewan : ");
    int umurBaru;
    inputIntValid(umurBaru, "Umur       : ");
    double beratBaru;
    inputDoubleValid(beratBaru, "Berat      : ");

    string err;
    if (!validasiRangeHewan(umurBaru, beratBaru, err)) {
        cout << "Gagal tambah data: " << err << "\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    cout << "\nRingkasan data yang akan ditambahkan:\n";
    cout << "ID    : " << idBaru << "\n";
    cout << "Nama  : " << namaBaru << "\n";
    cout << "Umur  : " << umurBaru << " tahun\n";
    cout << "Berat : " << beratBaru << " kg\n";

    string konfirmasi;
    while (true) {
        cout << "\nSimpan data ini? (y/n): ";
        if (!getline(cin, konfirmasi)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (konfirmasi.empty() || isOnlyWhitespace(konfirmasi)) {
            cout << "Input tidak boleh kosong!\n"; continue;
        }
        char firstChar = tolower(konfirmasi[0]);
        if (firstChar == 'y') {
            break;
        } else if (firstChar == 'n') {
            cout << "Penambahan data dibatalkan.\n";
            cout << "Tekan Enter..."; cin.ignore(); cin.get();
            bersihkanLayar();
            return;
        } else {
            cout << "Harap masukkan 'y' untuk ya atau 'n' untuk tidak!\n";
        }
    }

    ternak[*jumlah].id = idBaru;
    ternak[*jumlah].nama = namaBaru;
    ternak[*jumlah].umur = umurBaru;
    ternak[*jumlah].berat = beratBaru;
    
    (*jumlah)++;
    
    if (simpanDataKeFile(ternak, *jumlah)) {
        cout << "\nData berhasil ditambahkan!\n";
    } else {
        (*jumlah)--;
        cout << "\nGagal menyimpan data ke file!\n";
    }
    
    cout << "Tekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void tampilData(Hewan ternak[], int jumlah) {
    bersihkanLayar();
    cout << "\n===================================================\n";
    cout << "=                DATA HEWAN TERNAK                =\n";
    cout << "===================================================\n";
    
    if (jumlah == 0) {
        cout << "Belum ada data hewan untuk ditampilkan.\n";
    } else {
        tampilDataTabel(ternak, jumlah);
    }
    
    cout << "\nTekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void updateData(Hewan ternak[], int jumlah) {
    bersihkanLayar();
    cout << "\n========================================\n";
    cout << "=           UPDATE DATA HEWAN          =\n";
    cout << "========================================\n";

    if (jumlah == 0) {
        cout << "Belum ada data hewan untuk diupdate.\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    tampilDataTabel(ternak, jumlah);
    
    int cari;
    string inputId;
    bool validIdInput = false;
    while (!validIdInput) {
        cout << "\nID Hewan : ";
        if (!getline(cin, inputId)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (inputId.empty() || isOnlyWhitespace(inputId)) {
            cout << "Input tidak boleh kosong!\n"; continue;
        }
        bool isNum = true;
        for (char c : inputId) if (!isdigit(static_cast<unsigned char>(c))) { isNum = false; break; }
        if (!isNum) { cout << "Harap masukkan angka!\n"; continue; }
        try {
            cari = stoi(inputId);
            validIdInput = true;
        } catch (...) { cout << "Input tidak valid!\n"; }
    }

    int idx = -1;
    for (int i = 0; i < jumlah; i++) {
        if (ternak[i].id == cari) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "Data dengan ID " << cari << " tidak ditemukan!\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    cout << "\nData ditemukan:\n";
    cout << "ID    : " << ternak[idx].id << "\n";
    cout << "Nama  : " << ternak[idx].nama << "\n";
    cout << "Umur  : " << ternak[idx].umur << "\n";
    cout << "Berat : " << ternak[idx].berat << "\n";
    
    string konfirmasi;
    while (true) {
        cout << "\nLanjutkan update data ini? (y/n): ";
        if (!getline(cin, konfirmasi)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (konfirmasi.empty() || isOnlyWhitespace(konfirmasi)) {
            cout << "Input tidak boleh kosong!\n"; continue;
        }
        char firstChar = tolower(konfirmasi[0]);
        if (firstChar == 'y') {
            break;
        } else if (firstChar == 'n') {
            cout << "Update dibatalkan.\n";
            cout << "Tekan Enter..."; cin.ignore(); cin.get();
            bersihkanLayar();
            return;
        } else {
            cout << "Harap masukkan 'y' untuk ya atau 'n' untuk tidak!\n";
        }
    }

    cout << "\n--- Masukkan Data Baru ---\n";
    ternak[idx].nama = inputStringNotEmpty("Nama Baru   : ");
    inputIntValid(ternak[idx].umur, "Umur Baru   : ");
    inputDoubleValid(ternak[idx].berat, "Berat Baru  : ");

    string err;
    if (!validasiRangeHewan(ternak[idx].umur, ternak[idx].berat, err)) {
        cout << "Gagal update: " << err << "\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    if (simpanDataKeFile(ternak, jumlah)) {
        cout << "\nData berhasil diupdate!\n";
    } else {
        cout << "\nGagal menyimpan data ke file!\n";
    }
    cout << "Tekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void hapusData(Hewan ternak[], int *jumlah) {
    bersihkanLayar();
    cout << "\n========================================\n";
    cout << "=           HAPUS DATA HEWAN           =\n";
    cout << "========================================\n";

    if (*jumlah == 0) {
        cout << "Belum ada data hewan untuk dihapus.\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    tampilDataTabel(ternak, *jumlah);

    int cari;
    inputIntValid(cari, "\nMasukkan ID yang ingin dihapus: ");
    
    int idx = -1;
    for (int i = 0; i < *jumlah; i++) {
        if (ternak[i].id == cari) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "Data tidak ditemukan!\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    cout << "\nData yang akan dihapus:\n";
    cout << "ID    : " << ternak[idx].id << "\n";
    cout << "Nama  : " << ternak[idx].nama << "\n";
    
    string konfirmasi;
    while (true) {
        cout << "\nYakin ingin menghapus data ini? (y/n): ";
        if (!getline(cin, konfirmasi)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (konfirmasi.empty() || isOnlyWhitespace(konfirmasi)) {
            cout << "Input tidak boleh kosong!\n"; continue;
        }
        char firstChar = tolower(konfirmasi[0]);
        if (firstChar == 'y') {
            break;
        } else if (firstChar == 'n') {
            cout << "Penghapusan dibatalkan.\n";
            cout << "Tekan Enter..."; cin.ignore(); cin.get();
            bersihkanLayar();
            return;
        } else {
            cout << "Harap masukkan 'y' untuk ya atau 'n' untuk tidak!\n";
        }
    }

    for (int j = idx; j < *jumlah - 1; j++) ternak[j] = ternak[j + 1];
    (*jumlah)--;
    
    if(simpanDataKeFile(ternak, *jumlah)) cout << "Data berhasil dihapus!\n";
    else cout << "Gagal menyimpan perubahan!\n";
    
    cout << "Tekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

// SORTING
void copyArray(Hewan src[], Hewan dest[], int n) { for(int i=0; i<n; i++) dest[i] = src[i]; }

void sortingNama(Hewan ternak[], int jumlah, bool ascend) {
    bersihkanLayar(); Hewan temp[100]; copyArray(ternak, temp, jumlah);
    if(ascend) { sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.nama < b.nama; }); cout << "Sorting: Nama (A-Z)\n\n"; }
    else { sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.nama > b.nama; }); cout << "Sorting: Nama (Z-A)\n\n"; }
    tampilDataTabel(temp, jumlah);
    cout << "\nTekan Enter..."; cin.ignore(); cin.get(); bersihkanLayar();
}

void sortingUmur(Hewan ternak[], int jumlah, bool ascend) {
    bersihkanLayar(); Hewan temp[100]; copyArray(ternak, temp, jumlah);
    if(ascend) { sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.umur < b.umur; }); cout << "Sorting: Umur (Muda -> Tua)\n\n"; }
    else { sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.umur > b.umur; }); cout << "Sorting: Umur (Tua -> Muda)\n\n"; }
    tampilDataTabel(temp, jumlah);
    cout << "\nTekan Enter..."; cin.ignore(); cin.get(); bersihkanLayar();
}

void sortingBerat(Hewan ternak[], int jumlah, bool ascend) {
    bersihkanLayar(); Hewan temp[100]; copyArray(ternak, temp, jumlah);
    if(ascend) { sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.berat < b.berat; }); cout << "Sorting: Berat (Ringan -> Berat)\n\n"; }
    else { sort(temp, temp + jumlah, [](Hewan a, Hewan b) { return a.berat > b.berat; }); cout << "Sorting: Berat (Berat -> Ringan)\n\n"; }
    tampilDataTabel(temp, jumlah);
    cout << "\nTekan Enter..."; cin.ignore(); cin.get(); bersihkanLayar();
}

void menuSorting(Hewan ternak[], int jumlah) {
    int pilih;
    do {
        bersihkanLayar();
        cout << "\n========================================\n";
        cout << "=           MENU SORTING DATA          =\n";
        cout << "========================================\n";
        cout << "1. Ascending  (A -> Z)\n";
        cout << "2. Descending (Z -> A)\n";
        cout << "3. Ascending  (Muda -> Tua)\n";
        cout << "4. Descending (Tua -> Muda)\n";
        cout << "5. Ascending  (Ringan -> Berat)\n";
        cout << "6. Descending (Berat -> Ringan)\n";
        cout << "7. Kembali\n";
        cout << "========================================\n";
        if (!inputPilihanValid(pilih, "Pilih: ", 1, 7)) continue;

        if (pilih == 1) sortingNama(ternak, jumlah, true);
        else if (pilih == 2) sortingNama(ternak, jumlah, false);
        else if (pilih == 3) sortingUmur(ternak, jumlah, true);
        else if (pilih == 4) sortingUmur(ternak, jumlah, false);
        else if (pilih == 5) sortingBerat(ternak, jumlah, true);
        else if (pilih == 6) sortingBerat(ternak, jumlah, false);
        else if (pilih == 7) return;
        
    } while(true);
}

// SEARCHING
void cariData(Hewan ternak[], int jumlah) {
    bersihkanLayar();
    
    if (jumlah == 0) {
        cout << "Belum ada data hewan untuk dicari.\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    int cari;
    cout << "\n========================================\n";
    cout << "=            CARI DATA HEWAN           =\n";
    cout << "========================================\n";
    inputIntValid(cari, "Masukkan ID yang dicari: ");
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
    cout << "\nTekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

// CRUD PENGAJUAN - USER
void tambahPengajuan(Pengajuan data[], int *jumlah, string usernameUser) {
    bersihkanLayar();
    cout << "\n========================================\n";
    cout << "=           TAMBAH PENGAJUAN           =\n";
    cout << "========================================\n";

    if (*jumlah >= 100) {
        cout << "Kapasitas pengajuan penuh!\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int idBaru;
    bool idValid = false;
    while (!idValid) {
        inputIntValid(idBaru, "ID Pengajuan : ");
        
        bool sudahAda = false;
        for (int i = 0; i < *jumlah; i++) {
            if (data[i].id == idBaru) {
                sudahAda = true;
                break;
            }
        }
        
        if (sudahAda) {
            cout << "ID Pengajuan " << idBaru << " sudah terdaftar! Silakan masukkan ID lain.\n";
        } else {
            idValid = true;
        }
    }

    data[*jumlah].id = idBaru;
    data[*jumlah].namaHewan = inputStringNotEmpty("Nama Hewan   : ");
    inputIntValid(data[*jumlah].umur, "Umur Hewan   : ");
    inputDoubleValid(data[*jumlah].berat, "Berat Hewan  : ");

    string err;
    if (!validasiRangeHewan(data[*jumlah].umur, data[*jumlah].berat, err)) {
        cout << err << "\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    data[*jumlah].usernamePengaju = usernameUser;
    data[*jumlah].status = "pending";
    data[*jumlah].timestamp = getTimestamp();

    (*jumlah)++;
    if (!simpanPengajuanKeFile(data, *jumlah)) {
        (*jumlah)--;
        cout << "Gagal menyimpan pengajuan ke file!\n";
    } else {
        cout << "\nPengajuan berhasil dikirim!\n";
        cout << "Status: Menunggu persetujuan admin\n";
    }

    cout << "Tekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void tampilPengajuanUser(Pengajuan data[], int jumlah, string usernameUser) {
    bersihkanLayar();
    cout << "\n==============================================================\n";
    cout << "=                      PENGAJUAN ANDA                        =\n";
    cout << "==============================================================\n";
    
    tampilTabelPengajuanUser(data, jumlah, usernameUser);
    
    cout << "\nTekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void updatePengajuan(Pengajuan data[], int jumlah, string usernameUser) {
    bersihkanLayar();
    cout << "\n==============================================================\n";
    cout << "=                     UPDATE PENGAJUAN                       =\n";
    cout << "==============================================================\n";

    if (jumlah == 0) {
        cout << "Belum ada pengajuan.\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    cout << "\nDaftar Pengajuan Anda:\n";
    tampilTabelPengajuanUser(data, jumlah, usernameUser);

    int cari;
    string inputId; bool validId = false;
    while (!validId) {
        cout << "\nMasukkan ID Pengajuan yang ingin diupdate: ";
        if (!getline(cin, inputId)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (inputId.empty() || isOnlyWhitespace(inputId)) { cout << "Input tidak boleh kosong!\n"; continue; }
        bool isNum = true; for (char c : inputId) if (!isdigit(static_cast<unsigned char>(c))) { isNum = false; break; }
        if (!isNum) { cout << "Harap masukkan angka!\n"; continue; }
        try { cari = stoi(inputId); validId = true; } catch (...) { cout << "Input tidak valid!\n"; }
    }

    int idx = -1;
    for (int i = 0; i < jumlah; i++) {
        if (data[i].id == cari && data[i].usernamePengaju == usernameUser) { idx = i; break; }
    }

    if (idx == -1) {
        cout << "ID tidak ditemukan atau bukan milik Anda!\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    if (data[idx].status != "pending") {
        cout << "Pengajuan dengan status '" << data[idx].status << "' tidak dapat diubah.\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    cout << "\nData yang akan diupdate:\n";
    cout << "ID Pengajuan : " << data[idx].id << "\n";
    cout << "Nama Hewan   : " << data[idx].namaHewan << "\n";
    cout << "Status       : " << data[idx].status << "\n";

    string konfirmasi;
    while (true) {
        cout << "\nLanjutkan update data ini? (y/n): ";
        if (!getline(cin, konfirmasi)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (konfirmasi.empty() || isOnlyWhitespace(konfirmasi)) { cout << "Input tidak boleh kosong!\n"; continue; }
        char firstChar = tolower(konfirmasi[0]);
        if (firstChar == 'y') break;
        else if (firstChar == 'n') {
            cout << "Update dibatalkan.\n";
            cout << "Tekan Enter..."; cin.ignore(); cin.get();
            bersihkanLayar();
            return;
        } else {
            cout << "Harap masukkan 'y' untuk ya atau 'n' untuk tidak!\n";
        }
    }

    cout << "\n--- Masukkan Data Baru ---\n";
    data[idx].namaHewan = inputStringNotEmpty("Nama Hewan   : ");
    inputIntValid(data[idx].umur, "Umur Hewan   : ");
    inputDoubleValid(data[idx].berat, "Berat Hewan  : ");

    string err;
    if (!validasiRangeHewan(data[idx].umur, data[idx].berat, err)) {
        cout << err << "\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    data[idx].timestamp = getTimestamp();
    if (simpanPengajuanKeFile(data, jumlah)) {
        cout << "\nPengajuan berhasil diupdate!\n";
    } else {
        cout << "\nGagal menyimpan perubahan!\n";
    }
    cout << "Tekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void hapusPengajuan(Pengajuan data[], int *jumlah, string usernameUser) {
    bersihkanLayar();
    cout << "\n==============================================================\n";
    cout << "=                       HAPUS PENGAJUAN                     =\n";
    cout << "==============================================================\n";
    if (*jumlah == 0) {
        cout << "Belum ada pengajuan.\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    cout << "\nDaftar Pengajuan Anda:\n";
    tampilTabelPengajuanUser(data, *jumlah, usernameUser);

    int cari;
    string inputId; bool validId = false;
    while (!validId) {
        cout << "\nMasukkan ID Pengajuan yang ingin dihapus: ";
        if (!getline(cin, inputId)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (inputId.empty() || isOnlyWhitespace(inputId)) { cout << "Input tidak boleh kosong!\n"; continue; }
        bool isNum = true; for (char c : inputId) if (!isdigit(static_cast<unsigned char>(c))) { isNum = false; break; }
        if (!isNum) { cout << "Harap masukkan angka!\n"; continue; }
        try { cari = stoi(inputId); validId = true; } catch (...) { cout << "Input tidak valid!\n"; }
    }

    int idx = -1;
    for (int i = 0; i < *jumlah; i++) {
        if (data[i].id == cari && data[i].usernamePengaju == usernameUser) { idx = i; break; }
    }

    if (idx == -1) {
        cout << "ID tidak ditemukan atau bukan milik Anda!\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    if (data[idx].status != "pending") {
        cout << "Pengajuan dengan status '" << data[idx].status << "' tidak dapat dihapus.\n";
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        bersihkanLayar();
        return;
    }

    cout << "\nData yang akan dihapus:\n";
    cout << "ID Pengajuan : " << data[idx].id << "\n";
    cout << "Nama Hewan   : " << data[idx].namaHewan << "\n";

    string konfirmasi;
    while (true) {
        cout << "\nYakin ingin menghapus data ini? (y/n): ";
        if (!getline(cin, konfirmasi)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (konfirmasi.empty() || isOnlyWhitespace(konfirmasi)) { cout << "Input tidak boleh kosong!\n"; continue; }
        char firstChar = tolower(konfirmasi[0]);
        if (firstChar == 'y') break;
        else if (firstChar == 'n') {
            cout << "Penghapusan dibatalkan.\n";
            cout << "Tekan Enter..."; cin.ignore(); cin.get();
            bersihkanLayar();
            return;
        } else {
            cout << "Harap masukkan 'y' untuk ya atau 'n' untuk tidak!\n";
        }
    }

    for (int j = idx; j < *jumlah - 1; j++) data[j] = data[j + 1];
    (*jumlah)--;

    if (simpanPengajuanKeFile(data, *jumlah)) {
        cout << "Pengajuan berhasil dihapus!\n";
    } else {
        cout << "Gagal menyimpan perubahan!\n";
    }
    cout << "Tekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

// FUNGSI ADMIN: KELOLA PENGAJUAN
void kelolaPengajuan(Pengajuan data[], int *jumlah, Hewan ternak[], int *jumlahTernak) {
    bersihkanLayar();
    cout << "\n========================================\n";
    cout << "=      KELOLA PENGAJUAN MASUK          =\n";
    cout << "========================================\n\n";
    
    int countPending = 0;
    for (int i = 0; i < *jumlah; i++) {
        if (data[i].status == "pending") {
            countPending++;
            cout << "[" << countPending << "] ID Pengajuan: " << data[i].id << "\n";
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
    string prompt = "Masukkan nomor pengajuan yang ingin diproses (0 untuk batal): ";
    if (!inputPilihanValid(pilihan, prompt, 0, countPending)) { cout << "Tekan Enter..."; cin.ignore(); cin.get(); bersihkanLayar(); return; }
    if (pilihan == 0) { bersihkanLayar(); return; }
    
    int idx = -1, counter = 0;
    for (int i = 0; i < *jumlah; i++) {
        if (data[i].status == "pending") { counter++; if (counter == pilihan) { idx = i; break; } }
    }
    if (idx == -1) { cout << "Pilihan tidak valid!\n"; cout << "Tekan Enter..."; cin.ignore(); cin.get(); bersihkanLayar(); return; }
    
    int aksi;
    cout << "\nApa yang ingin Anda lakukan?\n";
    cout << "1. Terima Pengajuan\n";
    cout << "2. Tolak Pengajuan\n";
    if (!inputPilihanValid(aksi, "Pilih: ", 1, 2)) { cout << "Pilihan tidak valid!\n"; cout << "Tekan Enter..."; cin.ignore(); cin.get(); bersihkanLayar(); return; }
    
    if (aksi == 1) {
        if (*jumlahTernak >= 100) {
            cout << "Kapasitas data ternak penuh!\n";
        } else {
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
    } else if (aksi == 2) {
        data[idx].status = "ditolak";
        simpanPengajuanKeFile(data, *jumlah);
        cout << "\nPengajuan DITOLAK.\n";
    }
    cout << "Tekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

void tampilRiwayatPengajuan(Pengajuan data[], int jumlah) {
    bersihkanLayar();
    cout << "\n========================================\n";
    cout << "=      RIWAYAT SEMUA PENGAJUAN         =\n";
    cout << "========================================\n\n";
    if (jumlah == 0) cout << "Belum ada riwayat pengajuan.\n";
    else for (int i = 0; i < jumlah; i++) cout << "[" << data[i].status << "] ID Peng:" << data[i].id << " | " << data[i].namaHewan << " | Pengaju: " << data[i].usernamePengaju << " | " << data[i].timestamp << "\n";
    cout << "\nTekan Enter..."; cin.ignore(); cin.get();
    bersihkanLayar();
}

// LOGIN SYSTEM
void registerAkun(vector<Akun>& users) {
    bersihkanLayar();
    cout << "\n========================================\n";
    cout << "=           REGISTER (USER)            =\n";
    cout << "========================================\n";
    string username, password;
    
    while (true) {
        username = inputStringNotEmpty("Buat Username (min 3 karakter): ", 3);
        bool exists = false;
        for (const auto& user : users) {
            if (user.username == username) {
                cout << "Username '" << username << "' sudah digunakan!\n";
                exists = true;
                break;
            }
        }
        if (!exists) break;
    }
    
    password = inputStringNotEmpty("Buat Password (min 4 karakter): ", 4);
    
    try {
        users.push_back({username, password, false});
        simpanUsersKeFile(users);
        cout << "\nRegister berhasil! Silakan login.\n";
    } catch (...) { cout << "\nGagal menyimpan data user.\n"; users.pop_back(); }
    cout << "Tekan Enter..."; cin.ignore(); cin.get(); bersihkanLayar();
}

bool login(vector<Akun>& users, bool &isAdmin, string &loggedInUser) {
    bersihkanLayar();
    string u, p; 
    int kesempatan = 3;
    
    while (kesempatan > 0) {
        cout << "\n========================================\n";
        cout << "=                LOGIN                 =\n";
        cout << "========================================\n";
        cout << "Sisa percobaan: " << kesempatan << "\n\n";
        
        while(true) {
            cout << "Username : "; 
            if (!getline(cin, u)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
            if (u.empty() || isOnlyWhitespace(u)) {
                cout << "Username tidak boleh kosong!\n";
                continue;
            }
            break;
        }
        
        while(true) {
            cout << "Password : "; 
            if (!getline(cin, p)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
            if (p.empty() || isOnlyWhitespace(p)) {
                cout << "Password tidak boleh kosong!\n";
                continue;
            }
            break;
        }
        
        try {
            for (const auto& user : users) {
                if (user.username == u && user.password == p) {
                    cout << "\nLogin berhasil sebagai " << (user.isAdmin ? "ADMIN" : "USER") << "!\n";
                    cout << "Selamat datang, " << user.username << "!\n";
                    isAdmin = user.isAdmin; loggedInUser = user.username; 
                    cin.ignore(); cin.get(); 
                    return true;
                }
            }
        } catch (...) { cout << "Terjadi kesalahan sistem saat login.\n"; }
        
        kesempatan--;
        if (kesempatan > 0) {
            cout << "\nUsername atau password salah!\n";
            cout << "Tekan Enter untuk coba lagi..."; cin.ignore(); cin.get();
            bersihkanLayar();
        }
    }
    cout << "\nAkun dikunci sementara (3x gagal). Coba lagi nanti.\n";
    cout << "Tekan Enter..."; cin.ignore(); cin.get();
    return false;
}

// MAIN PROGRAM
int main() {
    bersihkanLayar();
    vector<Akun> users;
    Hewan ternak[100] = {};        
    Pengajuan pengajuan[100] = {}; 
    int jumlah = 0, jumlahPengajuan = 0;
    string currentUser = "";

    bacaUsersDariFile(users);
    bacaDataDariFile(ternak, &jumlah);
    bacaPengajuanDariFile(pengajuan, &jumlahPengajuan);

    int menuAwal; bool sudahLogin = false, isAdmin = false;

    do {
        bersihkanLayar();
        cout << "\n========================================\n";
        cout << "=    SISTEM MANAJEMEN HEWAN TERNAK     =\n";
        cout << "========================================\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Keluar\n";
        cout << "========================================\n";
        if (!inputPilihanValid(menuAwal, "Pilih: ", 1, 3)) continue;

        if (menuAwal == 1) {
            registerAkun(users);
        } else if (menuAwal == 2) {
            sudahLogin = login(users, isAdmin, currentUser);
            if (sudahLogin) {
                bersihkanLayar();
                if (isAdmin) {
                    int pilihan;
                    do {
                        bersihkanLayar();
                        cout << "\n========================================\n";
                        cout << "=              MENU ADMIN              =\n";
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
                        if (!inputPilihanValid(pilihan, "Pilih: ", 1, 9)) continue;

                        if (pilihan == 1) tambahData(ternak, &jumlah);
                        else if (pilihan == 2) tampilData(ternak, jumlah);
                        else if (pilihan == 3) updateData(ternak, jumlah);
                        else if (pilihan == 4) hapusData(ternak, &jumlah);
                        else if (pilihan == 5) cariData(ternak, jumlah);
                        else if (pilihan == 6) menuSorting(ternak, jumlah);
                        else if (pilihan == 7) kelolaPengajuan(pengajuan, &jumlahPengajuan, ternak, &jumlah);
                        else if (pilihan == 8) tampilRiwayatPengajuan(pengajuan, jumlahPengajuan);
                        else if (pilihan == 9) {
                            cout << "Logging out...\n";
                            bersihkanLayar();
                        }
                    } while (pilihan != 9);
                    bersihkanLayar();
                } else {
                    int pilihan;
                    do {
                        bersihkanLayar();
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
                        if (!inputPilihanValid(pilihan, "Pilih: ", 1, 6)) continue;

                        if (pilihan == 1) tambahPengajuan(pengajuan, &jumlahPengajuan, currentUser);
                        else if (pilihan == 2) tampilData(ternak, jumlah);
                        else if (pilihan == 3) tampilPengajuanUser(pengajuan, jumlahPengajuan, currentUser);
                        else if (pilihan == 4) updatePengajuan(pengajuan, jumlahPengajuan, currentUser);
                        else if (pilihan == 5) hapusPengajuan(pengajuan, &jumlahPengajuan, currentUser);
                        else if (pilihan == 6) {
                            cout << "Logging out...\n";
                            bersihkanLayar();
                        }
                    } while (pilihan != 6);
                    bersihkanLayar();
                }
            }
        } else if (menuAwal == 3) {
            simpanDataKeFile(ternak, jumlah);
            simpanPengajuanKeFile(pengajuan, jumlahPengajuan);
            bersihkanLayar();
            cout << "\nData tersimpan. Program selesai. Terima kasih!\n";
        }
    } while (menuAwal != 3);
    
    return 0;
}