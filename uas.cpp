#include <iostream>
#include <string>
#include <queue>
#include <iomanip>
#include <vector>
#include <map>

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
            try { 
                long long checkValue = stoll(input);
                if (checkValue > 2147483647) throw 1; // Cek batas max int
                return (int)checkValue; 
            } catch (...) { 
                cout << "[-] Angka terlalu besar atau tidak valid!\n"; 
            }
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
// 1. STRUCT DEFINITIONS
// ==========================================

struct PasienPoli {
    int noAntrean;         
    string nama;
    string nrm;
    string jenisKelamin;
    string alamat;         
    string noTelp;         
    int prioritas;         
    string kategori;
    string keluhan;
    string ruanganPoli; 
    string pembiayaan;            
    PasienPoli* next;
};

struct RiwayatPoli {
    string nama;
    string nrm;
    string ruanganPoli;
    RiwayatPoli* next;
};

struct RekamMedis {
    string nrm;            
    string nama;
    string jenisKelamin;
    string alamat;         
    string noTelp;         
    int umur;
    string riwayatPenyakit;
    RekamMedis* left;
    RekamMedis* right;
};

struct DataPasien {
    string nrm;            
    string nama;
};


// ==========================================
// 2. CLASS DEFINITIONS
// ==========================================

class SistemPendaftaranPoli {
private:
    PasienPoli* frontQueue[13];  
    RiwayatPoli* topStack;       
    RekamMedis* rootBST;         
    
    // Penambahan Struktur Graph (Adjacency List dengan Map & Vector)
    map<string, vector<string>> graphKontak;
    
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

    int getPoliIndex(string namaPoli) {
        for (int i = 4; i <= 12; i++) {
            if (lokasiPoli[i] == namaPoli) return i;
        }
        return 4; 
    }

    string generateNRM() {
        string nrm = "RM-";
        if (rmCounter < 10) nrm += "00" + to_string(rmCounter);
        else if (rmCounter < 100) nrm += "0" + to_string(rmCounter);
        else nrm += to_string(rmCounter);
        rmCounter++;
        return nrm;
    }

    string verifikasiPembiayaanFlow(string namaPasien) {
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
            cout << "[INFO] Pasien terdaftar sebagai Pasien Umum.\n";
            return "UMUM";
        } else if (pembiayaan == 2) {
            bool punyaRujukan = getValidYN("Apakah membawa Surat Rujukan Faskes 1? (y/n): ");
            if (punyaRujukan) {
                cout << "[VERIFIKASI BPJS BERHASIL] Surat Eligibilitas Peserta (SEP) siap dicetak.\n";
                return "BPJS";
            } else {
                cout << "\n[-] Pasien BPJS WAJIB membawa surat rujukan!\n";
                cout << "1. Lanjut pendaftaran menggunakan Biaya Umum\n";
                cout << "2. Batal (Kembali pilih pembiayaan)\n";
                int pilihanBatal = getValidInt("Pilih tindakan (1-2): ");
                if (pilihanBatal == 1) return "UMUM";
                else return verifikasiPembiayaanFlow(namaPasien); 
            }
        } else {
            getValidString("Masukkan Nomor Polis Asuransi: ");
            cout << "[VERIFIKASI ASURANSI BERHASIL] Menunggu approval pihak asuransi...\n";
            return "ASURANSI";
        }
    }

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

        string nama, nrm, jk, alamat, noTelp, keluhan, pembiayaanStr;
        int umur;
        bool kondisiKhusus = false;

        if (jenisPasien == 1) { 
            cout << "\n[+] Formulir Pasien Baru\n";
            nama = getValidString("Nama Lengkap    : ");
            while (true) {
                jk = getValidString("Jenis Kelamin (L/P): ");
                if (jk == "L" || jk == "l" || jk == "P" || jk == "p") break;
                cout << "[-] Masukkan 'L' atau 'P'.\n";
            }
            jk = (jk == "l" || jk == "L") ? "L" : "P";
            umur = getValidInt("Usia            : ");
            alamat = getValidString("Alamat Lengkap  : ");
            noTelp = getValidString("Nomor Telepon   : ");
            nrm = generateNRM();
            rootBST = insertBST(rootBST, nrm, nama, umur, jk, alamat, noTelp, "Belum ada riwayat");
        } else { 
            cout << "\n[+] Data Pasien Lama\n";
            nrm = getValidString("Masukkan Nomor Rekam Medis (Cth: RM-001): ");
            RekamMedis* dataPasien = searchBST(rootBST, nrm);
            if (dataPasien == NULL) {
                cout << "[-] Data tidak ditemukan! Silakan daftar sebagai pasien baru.\n";
                return;
            }
            nama = dataPasien->nama; umur = dataPasien->umur; jk = dataPasien->jenisKelamin;
            alamat = dataPasien->alamat; noTelp = dataPasien->noTelp;
        }

        if (umur > 12 && umur < 60) {
            string pr = (jk == "L") ? "Apakah pasien penyandang Disabilitas? (y/n): " : "Ibu Hamil / Disabilitas? (y/n): ";
            kondisiKhusus = getValidYN(pr);
        }
        keluhan = getValidString("Keluhan Utama (Cth: sakit gigi, hamil, maag): ");
        pembiayaanStr = verifikasiPembiayaanFlow(nama);

        bool dataSudahBenar = false;
        while (!dataSudahBenar) {
            cout << "\n========================================\n";
            cout << "       REVIEW DATA PENDAFTARAN          \n";
            cout << "========================================\n";
            cout << "1. Nama Pasien   : " << nama << "\n";
            cout << "2. Keluhan       : " << keluhan << "\n";
            cout << "3. Pembiayaan    : " << pembiayaanStr << "\n";
            cout << "   (Data Pendukung: NRM " << nrm << ", Usia " << umur << ")\n";
            cout << "========================================\n";
            
            bool konfirmasi = getValidYN("Apakah data di atas sudah benar? (y/n): ");
            if (konfirmasi) {
                dataSudahBenar = true;
            } else {
                cout << "\n--- EDIT DATA PENDAFTARAN ---\n";
                cout << "1. Edit Nama\n";
                cout << "2. Edit Keluhan\n";
                cout << "3. Edit Pembiayaan\n";
                cout << "0. Batalkan Pendaftaran Seluruhnya\n";
                int pilihanEdit = getValidInt("Pilih data yang ingin diubah (0-3): ");
                
                if (pilihanEdit == 1) nama = getValidString("Masukkan Nama Baru: ");
                else if (pilihanEdit == 2) keluhan = getValidString("Masukkan Keluhan Baru: ");
                else if (pilihanEdit == 3) pembiayaanStr = verifikasiPembiayaanFlow(nama);
                else if (pilihanEdit == 0) {
                    cout << "[INFO] Pendaftaran dibatalkan.\n";
                    return;
                }
            }
        }

        tambahAntrean(nama, nrm, umur, jk, alamat, noTelp, kondisiKhusus, keluhan, pembiayaanStr);
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
        cout << "========================================\n\n";
    }

    void tambahAntrean(string nama, string nrm, int umur, string jk, string alamat, string noTelp, bool kondisiKhusus, string keluhan, string pembiayaan) {
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

        if (kelLower.find("anak") != string::npos || kelLower.find("bayi") != string::npos || umur <= 12) ruangan = "Poli Anak";
        else if (kelLower.find("hamil") != string::npos || kelLower.find("kandungan") != string::npos) ruangan = "Poli Kandungan";
        else if (kelLower.find("operasi") != string::npos || kelLower.find("tumor") != string::npos) ruangan = "Poli Bedah";
        else if (kelLower.find("lambung") != string::npos || kelLower.find("maag") != string::npos) ruangan = "Poli Penyakit Dalam";
        else if (kelLower.find("gigi") != string::npos) ruangan = "Poli Gigi";
        else if (kelLower.find("mata") != string::npos) ruangan = "Poli Mata";
        else if (kelLower.find("telinga") != string::npos || kelLower.find("hidung") != string::npos) ruangan = "Poli THT";
        else if (kelLower.find("saraf") != string::npos) ruangan = "Poli Saraf";

        PasienPoli* pasienBaru = new PasienPoli;
        pasienBaru->noAntrean = tiketCounter++; 
        pasienBaru->nama = nama; pasienBaru->nrm = nrm; pasienBaru->jenisKelamin = jk;
        pasienBaru->alamat = alamat; pasienBaru->noTelp = noTelp;
        pasienBaru->prioritas = prioritas; pasienBaru->kategori = kategori;
        pasienBaru->keluhan = keluhan; pasienBaru->ruanganPoli = ruangan;
        pasienBaru->pembiayaan = pembiayaan; pasienBaru->next = NULL;

        int poliIdx = getPoliIndex(ruangan);
        if (frontQueue[poliIdx] == NULL || prioritas < frontQueue[poliIdx]->prioritas) {
            pasienBaru->next = frontQueue[poliIdx];
            frontQueue[poliIdx] = pasienBaru;
        } else {
            PasienPoli* temp = frontQueue[poliIdx];
            while (temp->next != NULL && temp->next->prioritas <= prioritas) temp = temp->next;
            pasienBaru->next = temp->next;
            temp->next = pasienBaru;
        }

        arrPasien[jumlahPasienTerdaftar].nrm = nrm;
        arrPasien[jumlahPasienTerdaftar].nama = nama;
        jumlahPasienTerdaftar++;
        
        RekamMedis* rm = searchBST(rootBST, nrm);
        if(rm != NULL) {
            if(rm->riwayatPenyakit == "Belum ada riwayat") rm->riwayatPenyakit = keluhan;
            else rm->riwayatPenyakit += ", " + keluhan;
        }

        cout << "[+] Pasien berhasil masuk antrean poliklinik!\n";
        cetakStruk(pasienBaru);
    }

    void tampilAntrean() {
        cout << "\n=== DAFTAR ANTREAN PER POLIKLINIK ===\n";
        bool k = true;
        for (int i = 4; i <= 12; i++) {
            if (frontQueue[i] != NULL) {
                k = false;
                cout << "\n[" << lokasiPoli[i] << "]\n";
                PasienPoli* temp = frontQueue[i];
                int n = 1;
                while (temp != NULL) {
                    cout << "   " << n++ << ". NRM: " << temp->nrm << " | Nama: " << temp->nama << " (" << temp->jenisKelamin << ")\n";
                    temp = temp->next;
                }
            }
        }
        if (k) cout << "[-] Antrean kosong.\n";
    }

    void panggilPasien() {
        while (true) {
            cout << "\n-- PANGGIL PASIEN BERDASARKAN POLI --\n";
            for (int i = 4; i <= 12; i++) cout << i - 3 << ". " << lokasiPoli[i] << "\n";
            cout << "0. Batal\n";
            
            int p = getValidInt("Pilih Poli (0-9): ");
            
            if (p == 0) {
                cout << "[INFO] Pemanggilan dibatalkan.\n";
                return;
            }
            
            if (p >= 1 && p <= 9) {
                int idx = p + 3; 
                if (frontQueue[idx] == NULL) { 
                    cout << "[-] Tidak ada antrean di poli tersebut.\n"; 
                    return; 
                }
                PasienPoli* t = frontQueue[idx];
                frontQueue[idx] = frontQueue[idx]->next; 
                cout << "\n[PANGGILAN] Antrean " << t->noAntrean << ", a/n " << t->nama << " ke " << t->ruanganPoli << "!\n";
                pushRiwayat(t->nama, t->nrm, t->ruanganPoli);
                delete t;
                return;
            } else {
                cout << "[-] Pilihan diluar rentang (0-9). Silakan ulangi.\n";
            }
        }
    }

    void hapusPasien(string nrmHapus) {
        bool found = false;
        for (int i = 4; i <= 12; i++) {
            PasienPoli *t = frontQueue[i], *p = NULL;
            if (t != NULL && t->nrm == nrmHapus) {
                frontQueue[i] = t->next; delete t; found = true; break;
            }
            while (t != NULL && t->nrm != nrmHapus) { p = t; t = t->next; }
            if (t != NULL) { p->next = t->next; delete t; found = true; break; }
        }
        if (!found) cout << "[-] NRM tidak ditemukan di antrean.\n";
        else cout << "[+] Pendaftaran dibatalkan.\n";
    }

    void updateDataPasien() {
        if (rootBST == NULL) { cout << "[-] Database kosong.\n"; return; }
        string nrmUpdate = getValidString("Masukkan NRM: ");
        RekamMedis* p = searchBST(rootBST, nrmUpdate);
        if (p == NULL) { cout << "[-] Tidak ditemukan.\n"; return; }
        p->nama = getValidString("Nama Baru: ");
        while (true) {
            p->jenisKelamin = getValidString("JK Baru (L/P): ");
            if (p->jenisKelamin == "L" || p->jenisKelamin == "l" || p->jenisKelamin == "P" || p->jenisKelamin == "p") break;
        }
        p->jenisKelamin = (p->jenisKelamin == "l" || p->jenisKelamin == "L") ? "L" : "P";
        p->umur = getValidInt("Usia Baru: ");
        p->alamat = getValidString("Alamat Baru: ");
        p->noTelp = getValidString("No. Telp Baru: ");
        for (int i = 0; i < jumlahPasienTerdaftar; i++) if (arrPasien[i].nrm == nrmUpdate) arrPasien[i].nama = p->nama;
        cout << "[+] Data diperbarui!\n";
    }

    // ==========================================
    // IMPLEMENTASI GRAPH (Relasi Keluarga/Kontak)
    // ==========================================
    void tambahRelasi(string nrm1, string nrm2) {
        // Cek apakah NRM ada di database BST
        if (searchBST(rootBST, nrm1) == NULL || searchBST(rootBST, nrm2) == NULL) {
            cout << "[-] GAGAL: Salah satu atau kedua NRM tidak ditemukan di rekam medis.\n";
            return;
        }

        // Cek agar tidak mendaftarkan relasi yang sama (Duplikasi)
        for (string tetangga : graphKontak[nrm1]) {
            if (tetangga == nrm2) {
                cout << "[-] Relasi antara " << nrm1 << " dan " << nrm2 << " sudah ada sebelumnya.\n";
                return;
            }
        }

        // Graph Undirected (Hubungan Dua Arah)
        graphKontak[nrm1].push_back(nrm2);
        graphKontak[nrm2].push_back(nrm1);
        cout << "[+] BERHASIL: Relasi kontak antara " << nrm1 << " dan " << nrm2 << " berhasil ditambahkan!\n";
    }

    void tampilkanGraph() {
        cout << "\n=== JARINGAN KONTAK PASIEN (GRAPH ADJACENCY LIST) ===\n";
        if (graphKontak.empty()) {
            cout << "[-] Belum ada data relasi kontak antar pasien yang terdaftar.\n";
            return;
        }

        // Menampilkan seluruh relasi yang ada di dalam Map
        for (auto const& pair : graphKontak) {
            string nrmNode = pair.first;
            RekamMedis* rm = searchBST(rootBST, nrmNode);
            string namaNode = (rm != NULL) ? rm->nama : "Unknown";

            cout << "[" << nrmNode << " - " << namaNode << "] terhubung/kontak dengan:\n";
            
            // Loop vektor tetangga
            for (string tetangga : pair.second) {
                RekamMedis* rmTetangga = searchBST(rootBST, tetangga);
                string namaTetangga = (rmTetangga != NULL) ? rmTetangga->nama : "Unknown";
                cout << "   -> " << tetangga << " (" << namaTetangga << ")\n";
            }
            cout << "\n";
        }
    }
    // ==========================================

    void pushRiwayat(string nama, string nrm, string poli) {
        RiwayatPoli* n = new RiwayatPoli;
        n->nama = nama; n->nrm = nrm; n->ruanganPoli = poli;
        n->next = topStack; topStack = n;
    }

    void tampilRiwayat() {
        if (topStack == NULL) { cout << "[-] Riwayat kosong.\n"; return; }
        RiwayatPoli* t = topStack;
        cout << "\n=== RIWAYAT PEMANGGILAN (LIFO) ===\n";
        while (t != NULL) { cout << "- " << t->nama << " -> " << t->ruanganPoli << "\n"; t = t->next; }
    }

    RekamMedis* insertBST(RekamMedis* r, string n, string nm, int u, string j, string a, string t, string riw) {
        if (r == NULL) {
            RekamMedis* nb = new RekamMedis;
            nb->nrm = n; nb->nama = nm; nb->umur = u; nb->jenisKelamin = j;
            nb->alamat = a; nb->noTelp = t; nb->riwayatPenyakit = riw;
            nb->left = nb->right = NULL; return nb;
        }
        if (n < r->nrm) r->left = insertBST(r->left, n, nm, u, j, a, t, riw);
        else if (n > r->nrm) r->right = insertBST(r->right, n, nm, u, j, a, t, riw);
        return r;
    }

    RekamMedis* searchBST(RekamMedis* r, string n) {
        if (r == NULL || r->nrm == n) return r;
        if (r->nrm < n) return searchBST(r->right, n);
        return searchBST(r->left, n);
    }

    void cariRekamMedis(string n) {
        RekamMedis* h = searchBST(rootBST, n);
        if (h != NULL) {
            cout << "\n--- REKAM MEDIS ---\nNRM: " << h->nrm << "\nNama: " << h->nama << "\nJK: " << h->jenisKelamin << "\nAlamat: " << h->alamat << "\nRiwayat: " << h->riwayatPenyakit << "\n";
        } else cout << "[-] Tidak ditemukan.\n";
    }

    void urutkanPasienBerdasarkanNama() {
        if (jumlahPasienTerdaftar == 0) {
            cout << "[-] Belum ada data pasien yang terdaftar.\n";
            return;
        }
        for (int i = 0; i < jumlahPasienTerdaftar - 1; i++) {
            int m = i;
            for (int j = i + 1; j < jumlahPasienTerdaftar; j++) if (arrPasien[j].nama < arrPasien[m].nama) m = j;
            DataPasien t = arrPasien[m]; arrPasien[m] = arrPasien[i]; arrPasien[i] = t;
        }
        cout << "\n=== DAFTAR PASIEN (Sorting A-Z) ===\n";
        for (int i = 0; i < jumlahPasienTerdaftar; i++) cout << i+1 << ". " << arrPasien[i].nama << " (NRM: " << arrPasien[i].nrm << ")\n";
    }
};

int main() {
    SistemPendaftaranPoli p;
    int c; string n, n2;
    do {
        cout << "\n============================================\n";
        cout << "   SISTEM PENDAFTARAN POLIKLINIK TERPADU    \n";
        cout << "============================================\n";
        cout << "[1] Registrasi Pasien Poli\n[2] Panggil Pasien Pemeriksaan\n[3] Tampilkan Antrean\n[4] Cari Rekam Medis (BST)\n[5] Batal Antre / Hapus\n[6] Lihat Riwayat (Stack)\n[7] Cetak Semua Pasien (Sorting)\n[8] Update Data Pasien\n";
        cout << "[9] Tambah Relasi Kontak Keluarga (Graph)\n[10] Lihat Jaringan Kontak (Graph)\n";
        cout << "[0] Keluar\n";
        c = getValidInt("Pilih: ");
        switch (c) {
            case 1: p.prosesPendaftaran(); break;
            case 2: p.panggilPasien(); break;
            case 3: p.tampilAntrean(); break;
            case 4: n = getValidString("NRM: "); p.cariRekamMedis(n); break;
            case 5: n = getValidString("NRM Batal: "); p.hapusPasien(n); break;
            case 6: p.tampilRiwayat(); break;
            case 7: p.urutkanPasienBerdasarkanNama(); break;
            case 8: p.updateDataPasien(); break;
            case 9: 
                cout << "\n-- TAMBAH RELASI KONTAK / KELUARGA --\n";
                n = getValidString("Masukkan NRM Pasien 1: ");
                n2 = getValidString("Masukkan NRM Pasien 2: ");
                if (n == n2) {
                    cout << "[-] NRM tidak boleh sama!\n";
                } else {
                    p.tambahRelasi(n, n2);
                }
                break;
            case 10: p.tampilkanGraph(); break;
            case 0: cout << "\nMenutup program. Terima kasih!\n"; break;
            default: cout << "[-] Pilihan tidak valid!\n";
        }
    } while (c != 0);
    return 0;
}