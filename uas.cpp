#include <iostream>
#include <string>
#include <queue>
#include <iomanip>

using namespace std;

// ==========================================
// 0. FUNGSI BANTUAN UNTUK VALIDASI INPUT
// ==========================================


// Fungsi untuk memastikan input integer valid (Anti-Crash)
int getValidInt(string prompt) {
    string input;
    int value;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (input.empty()) {
            cout << "[-] Input tidak boleh kosong!\n";
            continue;
        }
        
        bool isValid = true;
        for (char c : input) {
            if (!isdigit(c)) { isValid = false; break; }
        }
        
        if (isValid) {
            try { return stoi(input); } catch (...) { cout << "[-] Angka terlalu besar!\n"; }
        } else {
            cout << "[-] Input harus berupa angka bulat!\n";
        }
    }
}

// Fungsi untuk memastikan input string tidak kosong
string getValidString(string prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty() && input.find_first_not_of(' ') != string::npos) {
            return input;
        }
        cout << "[-] Input tidak boleh kosong atau hanya spasi!\n";
    }
}

// Fungsi khusus validasi (y/n)
bool getValidYN(string prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (input.empty()) {
            cout << "[-] Input tidak boleh kosong!\n";
            continue;
        }

        if (input == "y" || input == "Y") return true;
        if (input == "n" || input == "N") return false;

        cout << "[-] Input tidak valid! Masukkan 'y' untuk ya atau 'n' untuk tidak.\n";
    }
}


// ==========================================
// 1. STRUCT DEFINITIONS (Materi Percobaan II)
// ==========================================

// Node untuk Priority Queue per Poli (Materi Percobaan VI & VIII)
struct PasienPoli {
    int noAntrean;         
    string nama;
    string nrm;
    string jenisKelamin;   
    int prioritas;         // 1: Lansia/Difabel, 2: Anak, 3: Umum
    string kategori;
    string keluhan;
    string ruanganPoli; 
    string pembiayaan;            
    PasienPoli* next;
};

// Node untuk Stack Riwayat (Materi Percobaan V)
struct RiwayatPoli {
    string nama;
    string nrm;
    string ruanganPoli;
    RiwayatPoli* next;
};

// Node untuk Binary Search Tree Rekam Medis (Materi Percobaan IX)
struct RekamMedis {
    string nrm;            
    string nama;
    string jenisKelamin;   
    int umur;
    string riwayatPenyakit;
    RekamMedis* left;
    RekamMedis* right;
};

// Struct untuk Array Penampung Sorting (Materi Percobaan I & IV)
struct DataPasien {
    string nrm;            
    string nama;
};


// ==========================================
// 2. CLASS DEFINITIONS (Materi Percobaan III)
// ==========================================

class SistemPendaftaranPoli {
private:
    PasienPoli* frontQueue[13];  // Array of Linked List (Antrean terpisah per Poli)
    RiwayatPoli* topStack;       // Stack Riwayat Panggilan
    RekamMedis* rootBST;         // Root BST Rekam Medis
    
    // Array Statis nama lokasi (Materi Percobaan I)
    string lokasiPoli[13] = {
        "Loket Pendaftaran", "Ruang Tunggu Utama", "Lorong Blok A", "Lorong Blok B", 
        "Poli Umum", "Poli Anak", "Poli Kandungan", "Poli Bedah", 
        "Poli Penyakit Dalam", "Poli Gigi", "Poli Mata", "Poli THT", "Poli Saraf"
    };
    
    DataPasien arrPasien[100];
    int jumlahPasienTerdaftar;
    int tiketCounter;
    int rmCounter; 

public:
    SistemPendaftaranPoli() {
        for(int i = 0; i < 13; i++) {
            frontQueue[i] = NULL;
        }
        topStack = NULL;
        rootBST = NULL;
        jumlahPasienTerdaftar = 0;
        tiketCounter = 1; 
        rmCounter = 1; 
    }

    // Mendapatkan indeks poli berdasarkan nama ruangan
    int getPoliIndex(string namaPoli) {
        for (int i = 4; i <= 12; i++) {
            if (lokasiPoli[i] == namaPoli) return i;
        }
        return 4; 
    }

    // Membuat NRM otomatis untuk pasien baru
    string generateNRM() {
        string nrm = "RM-";
        if (rmCounter < 10) nrm += "00" + to_string(rmCounter);
        else if (rmCounter < 100) nrm += "0" + to_string(rmCounter);
        else nrm += to_string(rmCounter);
        rmCounter++;
        return nrm;
    }

    // Alur pendaftaran utama (Create)
    void prosesPendaftaran() {
        cout << "\n-- FORM REGISTRASI POLIKLINIK --\n";
        cout << "1. Pasien Baru\n";
        cout << "2. Pasien Lama\n";
        int jenisPasien;
        while(true) {
            jenisPasien = getValidInt("Pilih status pasien (1/2): ");
            if (jenisPasien == 1 || jenisPasien == 2) break;
            cout << "[-] Pilihan tidak valid!\n";
        }

        string nama, nrm, jk, keluhan, pembiayaanStr;
        int umur;
        bool kondisiKhusus = false;

        if (jenisPasien == 1) { 
            cout << "\n[+] Formulir Pasien Baru (Mohon serahkan KTP)\n";
            nama = getValidString("Nama Lengkap   : ");
            
            while (true) {
                jk = getValidString("Jenis Kelamin (L/P): ");
                if (jk == "L" || jk == "l" || jk == "P" || jk == "p") break;
                cout << "[-] Masukkan 'L' untuk Laki-laki atau 'P' untuk Perempuan.\n";
            }
            jk = (jk == "l" || jk == "L") ? "L" : "P";

            umur = getValidInt("Usia           : ");
            nrm = generateNRM();
            cout << "[!] Sistem telah membuatkan Nomor RM Baru: " << nrm << "\n";
            rootBST = insertBST(rootBST, nrm, nama, umur, jk, "Belum ada riwayat (Pasien Baru)");

        } else { 
            cout << "\n[+] Data Pasien Lama (Mohon serahkan Kartu Berobat/KTP)\n";
            nrm = getValidString("Masukkan Nomor Rekam Medis (Cth: RM-001): ");
            
            RekamMedis* dataPasien = searchBST(rootBST, nrm);
            if (dataPasien == NULL) {
                cout << "[-] Data dengan NRM " << nrm << " tidak ditemukan! Silakan daftar sebagai pasien baru.\n";
                return;
            } else {
                cout << "[+] Data Ditemukan!\n";
                cout << "    Nama: " << dataPasien->nama 
                     << " | JK: " << dataPasien->jenisKelamin 
                     << " | Usia: " << dataPasien->umur << " tahun\n";
                nama = dataPasien->nama;
                umur = dataPasien->umur;
                jk = dataPasien->jenisKelamin;
            }
        }

        if (umur > 12 && umur < 60) {
            string prompt = (jk == "L") ? "Apakah pasien penyandang Disabilitas? (y/n): " : "Apakah pasien Ibu Hamil / Disabilitas? (y/n): ";
            kondisiKhusus = getValidYN(prompt);
        }

        keluhan = getValidString("Keluhan Utama (Cth: sakit gigi, mata buram, hamil, maag): ");
        
        cout << "\n-- VERIFIKASI PEMBIAYAAN --\n";
        cout << "1. Pasien Umum (Bayar Tunai/Pribadi)\n";
        cout << "2. BPJS Kesehatan\n";
        cout << "3. Asuransi Swasta\n";
        int pembiayaan;
        while(true) {
            pembiayaan = getValidInt("Pilih metode pembiayaan (1-3): ");
            if (pembiayaan >= 1 && pembiayaan <= 3) break;
            cout << "[-] Pilihan tidak valid!\n";
        }

        if (pembiayaan == 1) {
            pembiayaanStr = "UMUM";
            cout << "[INFO] Pasien terdaftar sebagai Pasien Umum.\n";
        } else if (pembiayaan == 2) {
            bool punyaRujukan = getValidYN("Apakah membawa Surat Rujukan Faskes 1? (y/n): ");
            if (punyaRujukan) {
                pembiayaanStr = "BPJS";
                cout << "[VERIFIKASI BPJS BERHASIL] Surat Eligibilitas Peserta (SEP) sedang dicetak...\n";
            } else {
                // LOGIKA BATAL PADA PENDAFTARAN BPJS
                cout << "\n[-] Pasien BPJS WAJIB membawa surat rujukan!\n";
                cout << "1. Lanjut pendaftaran menggunakan Biaya Umum\n";
                cout << "2. Batal Antre (Pendaftaran Dibatalkan)\n";
                int pilihanBatal;
                while(true) {
                    pilihanBatal = getValidInt("Pilih tindakan (1-2): ");
                    if (pilihanBatal == 1 || pilihanBatal == 2) break;
                    cout << "[-] Pilihan tidak valid!\n";
                }

                if (pilihanBatal == 1) {
                    pembiayaanStr = "UMUM";
                    cout << "[INFO] Dialihkan ke Pasien Umum.\n";
                } else {
                    cout << "[INFO] Pendaftaran pasien " << nama << " telah dibatalkan oleh sistem.\n";
                    return; // Menghentikan fungsi secara total
                }
            }
        } else {
            pembiayaanStr = "ASURANSI";
            getValidString("Masukkan Nomor Polis Asuransi: ");
            cout << "[VERIFIKASI ASURANSI BERHASIL] Menunggu approval pihak asuransi...\n";
        }

        tambahAntrean(nama, nrm, umur, jk, kondisiKhusus, keluhan, pembiayaanStr);
    }

    void cetakStruk(PasienPoli* p) {
        cout << "\n========================================\n";
        cout << "       STRUK PENDAFTARAN POLIKLINIK     \n";
        cout << "========================================\n";
        cout << "No. Antrean      : " << p->noAntrean << "\n";
        cout << "Nama Pasien      : " << p->nama << " (NRM: " << p->nrm << ")\n";
        cout << "Jenis Kelamin    : " << (p->jenisKelamin == "L" ? "Laki-laki" : "Perempuan") << "\n";
        cout << "Kategori Pasien  : " << p->kategori << " (Prioritas " << p->prioritas << ")\n";
        cout << "Keluhan Utama    : " << p->keluhan << "\n";
        cout << "Tujuan Poli      : " << p->ruanganPoli << "\n";
        cout << "Pembiayaan       : " << p->pembiayaan << "\n";
        if(p->pembiayaan == "BPJS") {
            cout << "----------------------------------------\n";
            cout << "          SURAT ELIGIBILITAS PESERTA    \n";
            cout << "            (SEP) BPJS KESEHATAN        \n";
        }
        cout << "========================================\n";
        cout << "Mohon tunggu panggilan di Ruang Tunggu Utama.\n\n";
    }

    void tambahAntrean(string nama, string nrm, int umur, string jk, bool kondisiKhusus, string keluhan, string pembiayaan) {
        int prioritas = 3;
        string kategori = "Reguler (Dewasa)";
        string ruangan = "Poli Umum";
        
        if (umur >= 60 || kondisiKhusus) {
            prioritas = 1;
            kategori = (jk == "L") ? "Prioritas Utama (Lansia/Difabel)" : "Prioritas Utama (Lansia/Bumil/Difabel)";
        } else if (umur <= 12) {
            prioritas = 2;
            kategori = "Prioritas Menengah (Anak-anak)";
        }

        string kelLower = keluhan;
        for (char &c : kelLower) c = tolower(c);

        if (kelLower.find("anak") != string::npos || kelLower.find("bayi") != string::npos || umur <= 12) {
            ruangan = "Poli Anak";
        } else if (kelLower.find("hamil") != string::npos || kelLower.find("kandungan") != string::npos || kelLower.find("janin") != string::npos) {
            ruangan = "Poli Kandungan";
        } else if (kelLower.find("operasi") != string::npos || kelLower.find("tumor") != string::npos || kelLower.find("luka") != string::npos) {
            ruangan = "Poli Bedah";
        } else if (kelLower.find("lambung") != string::npos || kelLower.find("maag") != string::npos || kelLower.find("diabetes") != string::npos) {
            ruangan = "Poli Penyakit Dalam";
        } else if (kelLower.find("gigi") != string::npos || kelLower.find("gusi") != string::npos) {
            ruangan = "Poli Gigi";
        } else if (kelLower.find("mata") != string::npos || kelLower.find("rabun") != string::npos) {
            ruangan = "Poli Mata";
        } else if (kelLower.find("telinga") != string::npos || kelLower.find("hidung") != string::npos || kelLower.find("tenggorokan") != string::npos) {
            ruangan = "Poli THT";
        } else if (kelLower.find("saraf") != string::npos || kelLower.find("lumpuh") != string::npos || kelLower.find("tremor") != string::npos) {
            ruangan = "Poli Saraf";
        } else {
            ruangan = "Poli Umum";
        }

        PasienPoli* pasienBaru = new PasienPoli;
        pasienBaru->noAntrean = tiketCounter++; 
        pasienBaru->nama = nama;
        pasienBaru->nrm = nrm;
        pasienBaru->jenisKelamin = jk;
        pasienBaru->prioritas = prioritas;
        pasienBaru->kategori = kategori;
        pasienBaru->keluhan = keluhan;
        pasienBaru->ruanganPoli = ruangan;
        pasienBaru->pembiayaan = pembiayaan;
        pasienBaru->next = NULL;

        int poliIdx = getPoliIndex(ruangan);

        if (frontQueue[poliIdx] == NULL || prioritas < frontQueue[poliIdx]->prioritas) {
            pasienBaru->next = frontQueue[poliIdx];
            frontQueue[poliIdx] = pasienBaru;
        } else {
            PasienPoli* temp = frontQueue[poliIdx];
            while (temp->next != NULL && temp->next->prioritas <= prioritas) {
                temp = temp->next;
            }
            pasienBaru->next = temp->next;
            temp->next = pasienBaru;
        }

        arrPasien[jumlahPasienTerdaftar].nrm = nrm;
        arrPasien[jumlahPasienTerdaftar].nama = nama;
        jumlahPasienTerdaftar++;
        
        RekamMedis* rm = searchBST(rootBST, nrm);
        if(rm != NULL) {
            if(rm->riwayatPenyakit == "Belum ada riwayat (Pasien Baru)") rm->riwayatPenyakit = keluhan;
            else rm->riwayatPenyakit += ", " + keluhan;
        }

        cout << "[+] Pendaftaran poli berhasil diproses!\n";
        cetakStruk(pasienBaru);
    }

    void tampilAntrean() {
        cout << "\n=== DAFTAR ANTREAN PER POLIKLINIK ===\n";
        bool antreanKosongSemua = true;
        for (int i = 4; i <= 12; i++) {
            if (frontQueue[i] != NULL) {
                antreanKosongSemua = false;
                cout << "\n[" << lokasiPoli[i] << "]\n";
                PasienPoli* temp = frontQueue[i];
                int no = 1;
                while (temp != NULL) {
                    cout << "   " << no++ << ". NRM: " << temp->nrm 
                         << " | Nama: " << temp->nama << " (" << temp->jenisKelamin << ")"
                         << "\n      [Prio " << temp->prioritas << " - " << temp->kategori << "] -> Pembiayaan: " << temp->pembiayaan << "\n";
                    temp = temp->next;
                }
            }
        }
        if (antreanKosongSemua) cout << "[-] Seluruh antrean poliklinik saat ini masih kosong.\n";
    }

    void panggilPasien() {
        cout << "\n-- PANGGIL PASIEN BERDASARKAN POLI --\n";
        for (int i = 4; i <= 12; i++) {
            cout << i - 3 << ". " << lokasiPoli[i] << "\n";
        }
        cout << "0. Batal\n"; // Tombol Batal
        
        int pilihanPoli;
        while(true) {
            pilihanPoli = getValidInt("Pilih Poli yang akan memanggil pasien (0-9): ");
            if(pilihanPoli >= 0 && pilihanPoli <= 9) break;
            cout << "[-] Pilihan tidak valid!\n";
        }
        
        // LOGIKA BATAL PADA PEMANGGILAN
        if (pilihanPoli == 0) {
            cout << "[INFO] Pemanggilan dibatalkan. Kembali ke menu utama.\n";
            return;
        }
        
        int poliIdx = pilihanPoli + 3; 
        if (frontQueue[poliIdx] == NULL) {
            cout << "[-] Tidak ada antrean pasien di " << lokasiPoli[poliIdx] << " saat ini.\n";
            return;
        }
        
        PasienPoli* temp = frontQueue[poliIdx];
        frontQueue[poliIdx] = frontQueue[poliIdx]->next; 
        cout << "\n[PANGGILAN POLI] Nomor Antrean " << temp->noAntrean << ", Pasien a/n " << temp->nama << "!\n";
        cout << "                 Silakan masuk ke ruangan " << temp->ruanganPoli << ".\n";
        pushRiwayat(temp->nama, temp->nrm, temp->ruanganPoli);
        delete temp;
    }

    void hapusPasien(string nrmHapus) {
        bool found = false;
        for (int i = 4; i <= 12; i++) {
            if (frontQueue[i] == NULL) continue;
            PasienPoli* temp = frontQueue[i];
            PasienPoli* prev = NULL;
            if (temp != NULL && temp->nrm == nrmHapus) {
                frontQueue[i] = temp->next;
                cout << "[-] Pendaftaran pasien " << temp->nama << " dari antrean " << lokasiPoli[i] << " telah dibatalkan.\n";
                delete temp; found = true; break;
            }
            while (temp != NULL && temp->nrm != nrmHapus) {
                prev = temp; temp = temp->next;
            }
            if (temp != NULL) {
                prev->next = temp->next;
                cout << "[-] Pendaftaran pasien " << temp->nama << " dari antrean " << lokasiPoli[i] << " telah dibatalkan.\n";
                delete temp; found = true; break;
            }
        }
        if (!found) cout << "[-] NRM " << nrmHapus << " tidak ditemukan di antrean poli mana pun.\n";
    }

    // Stack Riwayat (Percobaan V)
    void pushRiwayat(string nama, string nrm, string poli) {
        RiwayatPoli* nodeBaru = new RiwayatPoli;
        nodeBaru->nama = nama; nodeBaru->nrm = nrm; nodeBaru->ruanganPoli = poli;
        nodeBaru->next = topStack; topStack = nodeBaru;
    }

    void tampilRiwayat() {
        if (topStack == NULL) { cout << "[-] Belum ada pasien yang dipanggil ke Poli.\n"; return; }
        RiwayatPoli* temp = topStack;
        cout << "\n=== RIWAYAT PEMANGGILAN POLI (LIFO - Terbaru ke Terlama) ===\n";
        while (temp != NULL) {
            cout << "- " << temp->nama << " (NRM: " << temp->nrm << ") -> Masuk " << temp->ruanganPoli << "\n";
            temp = temp->next;
        }
    }

    // Binary Search Tree (Percobaan IX - Non-Linear)
    RekamMedis* insertBST(RekamMedis* root, string nrm, string nama, int umur, string jk, string riwayat) {
        if (root == NULL) {
            RekamMedis* nodeBaru = new RekamMedis;
            nodeBaru->nrm = nrm; nodeBaru->nama = nama; nodeBaru->umur = umur; nodeBaru->jenisKelamin = jk;
            nodeBaru->riwayatPenyakit = riwayat; nodeBaru->left = nodeBaru->right = NULL;
            return nodeBaru;
        }
        if (nrm < root->nrm) root->left = insertBST(root->left, nrm, nama, umur, jk, riwayat);
        else if (nrm > root->nrm) root->right = insertBST(root->right, nrm, nama, umur, jk, riwayat);
        return root;
    }

    RekamMedis* searchBST(RekamMedis* root, string nrm) { 
        if (root == NULL || root->nrm == nrm) return root;
        if (root->nrm < nrm) return searchBST(root->right, nrm);
        return searchBST(root->left, nrm);
    }

    void cariRekamMedis(string nrm) {
        RekamMedis* hasil = searchBST(rootBST, nrm);
        if (hasil != NULL) {
            cout << "\n--- REKAM MEDIS DITEMUKAN (Searching BST) ---\n";
            cout << "NRM          : " << hasil->nrm << "\n";
            cout << "Nama         : " << hasil->nama << "\n";
            cout << "Jenis Kelamin: " << (hasil->jenisKelamin == "L" ? "Laki-laki" : "Perempuan") << "\n";
            cout << "Usia         : " << hasil->umur << " tahun\n";
            cout << "Riwayat Poli : " << hasil->riwayatPenyakit << "\n";
        } else cout << "[-] Rekam Medis dengan NRM " << nrm << " tidak ditemukan.\n";
    }

    // Selection Sort (Percobaan IV)
    void urutkanPasienBerdasarkanNama() {
        if (jumlahPasienTerdaftar == 0) { cout << "[-] Belum ada data pasien terdaftar.\n"; return; }
        for (int i = 0; i < jumlahPasienTerdaftar - 1; i++) {
            int min_idx = i;
            for (int j = i + 1; j < jumlahPasienTerdaftar; j++) {
                if (arrPasien[j].nama < arrPasien[min_idx].nama) min_idx = j;
            }
            DataPasien temp = arrPasien[min_idx];
            arrPasien[min_idx] = arrPasien[i];
            arrPasien[i] = temp;
        }
        cout << "\n=== DAFTAR SELURUH PASIEN (Selection Sort A-Z) ===\n";
        for (int i = 0; i < jumlahPasienTerdaftar; i++) cout << i + 1 << ". " << arrPasien[i].nama << " (NRM: " << arrPasien[i].nrm << ")\n";
    }
};

int main() {
    SistemPendaftaranPoli poli;
    int pilihan; string nrm;
    do {
        cout << "\n================================================\n";
        cout << "    SISTEM PENDAFTARAN POLIKLINIK TERPADU       \n";
        cout << "================================================\n";
        cout << "[1] Registrasi Pasien Poli (Priority Queue)\n";
        cout << "[2] Panggil Pasien ke Ruang Poli (Dequeue)\n";
        cout << "[3] Tampilkan Antrean Ruang Tunggu\n";
        cout << "[4] Cari Rekam Medis (Searching BST)\n";
        cout << "[5] Batal Antre / Hapus Pendaftaran (Delete)\n";
        cout << "------------------------------------------------\n";
        cout << "[6] Lihat Riwayat Panggilan Poli (Stack)\n";
        cout << "[7] Cetak Daftar Semua Pasien (Sorting A-Z)\n";
        cout << "[0] Keluar\n";
        pilihan = getValidInt("Pilih menu: ");
        switch (pilihan) {
            case 1: poli.prosesPendaftaran(); break;
            case 2: poli.panggilPasien(); break;
            case 3: poli.tampilAntrean(); break;
            case 4: cout << "\n"; nrm = getValidString("Masukkan NRM untuk dicari: "); poli.cariRekamMedis(nrm); break;
            case 5: cout << "\n"; nrm = getValidString("Masukkan NRM pasien yang batal antre: "); poli.hapusPasien(nrm); break;
            case 6: poli.tampilRiwayat(); break;
            case 7: poli.urutkanPasienBerdasarkanNama(); break;
            case 0: cout << "\nMenutup program. Terima kasih!\n"; break;
            default: cout << "[-] Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);
    return 0;
}