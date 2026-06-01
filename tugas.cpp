#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <exception>

using namespace std;

// 1. Struktur Kelas TokoElektronik
class TokoElektronik {
private:
    // Atribut Private: array kapasitas 3 sebagai enkapsulasi
    array<string, 3> etalase;

public:
    // Constructor untuk mengisi data awal ke dalam array
    TokoElektronik() {
        etalase = {"Smart TV", "Kipas Angin", "Kulkas"};
    }

    // Method Public untuk mengambil produk
    string ambilProduk(size_t nomorRak) {
        try {
            // Wajib menggunakan .at() saat mengakses array
            return etalase.at(nomorRak);
        }
        // Menangkap error bawaan jika indeks melewati batas
        catch (const out_of_range& e) {
            // Melemparkan kembali (throw) pesan error kustom
            string pesanError = "Gagal Mengambil Barang : Rak nomor " + to_string(nomorRak) + " kosong atau tidak tersedia!";
            throw pesanError;
        }
    }
};

// Fungsi bantuan Read: Menampilkan barang secara otomatis
void tampilkanGudang() {
    ifstream infile("gudang.txt");
    if (!infile.is_open()) {
        cout << "[Info] Basis data gudang.txt masih kosong atau belum dibuat." << endl;
        return;
    }
    string baris;
    int no = 1;
    cout << "\n--- Daftar Barang di Gudang ---" << endl;
    while (getline(infile, baris)) {
        cout << no << ". " << baris << endl;
        no++;
    }
    infile.close();
    cout << "-------------------------------" << endl;
}

// Fungsi Create: Menambahkan barang ke dalam file
void tambahBarang() {
    ofstream outfile("gudang.txt", ios::app); // Mode append
    string namaBarang;
    cout << "Masukkan nama barang baru: ";
    // Menangkap input string dengan spasi
    getline(cin >> ws, namaBarang);
    outfile << namaBarang << endl;
    outfile.close();
    cout << "Barang berhasil ditambahkan!" << endl;
}

// Fungsi Update: Memperbarui data barang tertentu
void ubahBarang() {
    ifstream infile("gudang.txt");
    if (!infile.is_open()) {
        cout << "Gudang kosong, tidak ada yang bisa diubah." << endl;
        return;
    }

    int target;
    cout << "Pilih nomor urut barang yang ingin diubah: ";
    cin >> target;

    ofstream tempFile("temp.txt");
    string baris;
    int no = 1;
    bool ditemukan = false;

    // Looping membaca file asli dan menulis ke file temporary
    while (getline(infile, baris)) {
        if (no == target) {
            string namaBaru;
            cout << "Masukkan nama barang pengganti: ";
            getline(cin >> ws, namaBaru);
            tempFile << namaBaru << endl; // Tulis data baru
            ditemukan = true;
        } else {
            tempFile << baris << endl; // Tulis data lama
        }
        no++;
    }
    infile.close();
    tempFile.close();

    // Mengganti file lama dengan file temporary yang sudah diupdate
    remove("gudang.txt");
    rename("temp.txt", "gudang.txt");

    if (ditemukan) {
        cout << "Data barang berhasil diperbarui!" << endl;
    } else {
        cout << "Nomor barang tidak ditemukan!" << endl;
    }
}

// Fungsi Delete: Menghapus data barang
void hapusBarang() {
    ifstream infile("gudang.txt");
    if (!infile.is_open()) {
        cout << "Gudang kosong, tidak ada yang bisa dihapus." << endl;
        return;
    }

    int target;
    cout << "Pilih nomor urut barang yang ingin dihapus: ";
    cin >> target;

    ofstream tempFile("temp.txt");
    string baris;
    int no = 1;
    bool ditemukan = false;

    while (getline(infile, baris)) {
        if (no == target) {
            ditemukan = true;
            // Sengaja di-skip (tidak ditulis ke tempFile) agar terhapus
        } else {
            tempFile << baris << endl;
        }
        no++;
    }
    infile.close();
    tempFile.close();

    remove("gudang.txt");
    rename("temp.txt", "gudang.txt");

    if (ditemukan) {
        cout << "Data barang berhasil dihapus!" << endl;
    } else {
        cout << "Nomor barang tidak ditemukan!" << endl;
    }
}

int main() {
    TokoElektronik toko;
    int pilihan;

    // 4. Pengujian pada Fungsi main() dengan Looping
    do {
        cout << "\n=========================================" << endl;
        cout << "    SISTEM MANAJEMEN TOKO GIBRAN JAYA    " << endl;
        cout << "=========================================" << endl;
        
        // Membaca dan menampilkan daftar barang secara otomatis
        tampilkanGudang();

        cout << "Menu Pilihan:" << endl;
        cout << "1. Tambah Data Barang (Create)" << endl;
        cout << "2. Ubah Data Barang (Update)" << endl;
        cout << "3. Hapus Data Barang (Delete)" << endl;
        cout << "4. Simulasi Etalase (Uji Exception Handling)" << endl;
        cout << "0. Keluar" << endl;
        cout << "Masukkan pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                tambahBarang();
                break;
            case 2:
                ubahBarang();
                break;
            case 3:
                hapusBarang();
                break;
            case 4:
                cout << "\n=== Menjalankan Simulasi Etalase ===" << endl;
                
                // Skenario 1: Harus sukses
                try {
                    cout << "[Skenario 1] Mengambil barang di rak indeks ke-1..." << endl;
                    string barang = toko.ambilProduk(1);
                    cout << "-> Berhasil! Anda mengambil: " << barang << endl;
                } catch (string& e) {
                    cout << "-> " << e << endl;
                }

                // Skenario 2: Harus gagal dan menampilkan pesan custom
                try {
                    cout << "\n[Skenario 2] Mengambil barang di rak indeks ke-5..." << endl;
                    string barang = toko.ambilProduk(5);
                    cout << "-> Berhasil! Anda mengambil: " << barang << endl; // Baris ini tidak akan dieksekusi
                } catch (string& e) {
                    // Menangkap string pesan error yang di-throw
                    cout << "-> " << e << endl; 
                }
                break;
            case 0:
                cout << "Program selesai. Keluar dari sistem." << endl;
                break;
            default:
                cout << "Pilihan tidak valid, silakan coba lagi." << endl;
                break;
        }
    } while (pilihan != 0);

    return 0;
}