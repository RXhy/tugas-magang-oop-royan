#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

//SUPERMARKET HQ
class supermarket
{
private:
    float rekening = 300;

public:
    float biayaRestock = 0;
    float untungHarian = 0;
    string nama;
    float harga;
    int stok;

    //GET REKENING
    float getRekening(){
        return rekening;
    }

    //Kalkulasi penghasilan
    void updateRekening(float amount){
        rekening += amount;
    }

    bool bangkrut() {
        return rekening <= 0;
    }   

    //RESTOCK
    void restock(int beliStok,float harga){
        stok += beliStok;
        biayaRestock += harga*(0.2)*beliStok;
        cout << "---------------------------------------" << endl;
        cout << "stok " << nama << " telah direstock menjadi " << stok << " dengan biaya sebesar " << biayaRestock << endl;
    }
};

//PRODUCT
class product : public supermarket
{
public: 
    //PRODUCT CONSTRUCTOR
    product(string name,float price,int stock)
    {
        nama = name;
        harga = price;
        stok = stock;
        
        cout << "---------------------------------------" << endl;
        cout << "produk baru " << nama << " dengan harga " << harga << " sebanyak " << stok << " pcs berhasil ditambahkan" << endl; 
    }

    //UPDATE PRICE
    void setHargaBaru(int newprice)
    {
        harga = newprice;
        cout << "---------------------------------------" << endl;
        cout << "harga " << nama << " diubah menjadi " << harga << endl;
    }

    //DISPLAY LIST PRODUCT
    void displayProduct() const
    {
        cout << nama << " | harga : " << harga << " | stok : " << stok << endl;
    }

    //BUYING PRODUCT
    bool buy(int quantity) {
        if (quantity > stok) {
            cout << "Stok tidak cukup untuk membeli " << quantity << " pcs dari " << nama << endl;
            return false;
        }
        stok -= quantity;
        float totalPendapatan = harga * quantity;
        untungHarian += totalPendapatan;
        return true;
    }
};

//ADD PRODUCT
void newProduct(vector<product>& products, supermarket& duit) {
    if (duit.getRekening() < 1000){
        cout << "Uang tidak cukup untuk menambah produk baru. Butuh 1000 uang." << endl;
        return;
    }
    string name;
    float price;
    int stock;

    cout << "Masukkan nama produk: ";
    cin >> name;
    cout << "Masukkan harga produk: ";
    cin >> price;
    cout << "Masukkan jumlah stok produk: ";
    cin >> stock;

    products.emplace_back(name, price, stock); 
    duit.updateRekening(-1000);
}
//BUYING RANDOM PROGRAM
void transactions(vector<product>& products,supermarket& duit) {
    cout << "---------------------------------------" << endl;
    for (auto& prod : products) {
        int quantityToBuy = rand() % (prod.stok + 1);
        if (prod.stok == 0){
            cout << "stok " << prod.nama << " habis" << endl;
            continue;
        } 
        if (quantityToBuy > 0) {
            cout << "Pembeli membeli " << quantityToBuy << " pcs " << prod.nama <<endl;
            prod.buy(quantityToBuy);
        }
        cout << "penghsilan  " << prod.nama << " dalam sehari sebesar "<< prod.untungHarian << endl;
        duit.updateRekening(prod.untungHarian);
    } 
}
//CHECK PRODUCT STOCK
bool zeroStock(const vector<product>& products) {
    for (const auto& prod : products) {
        if (prod.stok > 0) {
            return false;
        }
    }
    return true;
}
//RESTOCK
void restockProduct(vector<product>& products, supermarket& duit) {
    string namastok;
    int beliStok;
    cout << "nama produk yang ingin direstock: ";
    cin >> namastok;
    cout << "jumlah stok yang ingin dibeli: ";
    cin >> beliStok;

    for (auto& prod : products) {
        if (prod.nama == namastok) {
            prod.restock(beliStok, prod.harga);
            duit.updateRekening(-prod.biayaRestock); // Mengurangi rekening dengan biaya restock
            break;
        }    
    }
}
//DELETE PRODUCT
void deleteProduct(vector<product>& products) {
    string nameDel;
    cout << "Masukkan nama produk yang ingin dihapus: ";
    cin >> nameDel;

    auto it = std::remove_if(products.begin(), products.end(),
    [&nameDel](const product& prod) { return prod.nama == nameDel; });
    if (it != products.end()) {
        products.erase(it, products.end());
        cout << "---------------------------------------" << endl; 
        cout << "Produk " << nameDel << " berhasil dihapus." << endl;
    } else {
        cout << "Produk dengan nama " << nameDel << " tidak ditemukan." << endl;
    }
}

int main(){
    //RANDOM GENERATOR 
    srand(static_cast<unsigned int>(time(0)));
    vector<product> products;

    //HEADER
    cout << "+------- SERBA ADA SUPERMARKET -------+\n"<< endl;
    supermarket duit;
    int day = 0;

    cout << "Teruslah berjualan hingga bangkrut, supermarketmu akan bangkrut jika semua stok produk atau uang di rekeningmu habis."<< endl;
    cout << "jangan lupa restock produkmu ! ingat, restock 1 produk memiliki biaya setara dengan seperlima harga produknya." << endl;
    cout << "membutuhkan uang sebanyak 1000 untuk menambahkan produk baru" << endl;
    cout << "Produk untuk memulai usaha" << endl;
    products.emplace_back("cabai", 45, 20);
    products.emplace_back("bayam", 20, 40); 

    //LOOPING FOR PLAYING
    bool game = true;
    while (game == true){
    
    //MAIN MENU
    cout << "=======================================\nDAY " << day << endl; 
    cout << "Rekening supermarket hari ini: " << duit.getRekening() << endl;
    cout << "\nApa yang ingin anda lakukan?\n1. Cek list produk ('list')\n2. Tambahkan produk ('add')\n3. Edit harga produk ('editharga')\n4. Restock produk ('restock')\n5. Hapus produk ('delete')" << endl;

    //FOOTER
    cout << "\nKetik 'next' untuk hari selanjutnya\nKetik 'end' jika ingin berhenti" << endl;
    
    //INPUT HANDLER
    string input;
    cin >> input;

    // ENDING PROGRAM
    if (input == "end"){
        game = false;
    }
    //NEXT DAY  
    else if (input == "next"){ 
        day += 1;
        transactions(products,duit);
    }
    // SHOW LIST PRODUCT
    else if (input == "list"){
        cout << "-------------DAFTAR PRODUK-------------" << endl;
        for (const auto& prod : products) {
        prod.displayProduct();
        }
    }
    //ADD NEW PRODUCT
    else if (input == "add"){
        cout << "tambah produk baru" << endl;
        newProduct(products,duit);
    }
    //EDIT NEW PRICE 
    else if (input == "editharga"){
        string namaGanti;
        float newprice;
        cout << "nama produk yang ingin diganti: ";
        cin >> namaGanti;
        cout << "harga produk baru: ";
        cin >> newprice;

        for (auto& prod : products) {
            if (prod.nama == namaGanti){
                prod.setHargaBaru(newprice);
                break;
            }    
        }
    }    
    //RESTOCK
    else if (input == "restock"){
        restockProduct(products,duit);
    }
    //DELETE PRODUCT
    else if (input == "delete"){
        deleteProduct(products);
    }
    
    //ERROR HANDLING
    else{
        cout << "Kesalahan input, tidak ada menu dengan nama " << input << " !!!" << endl;
    }

    //GAME MECHANIC
    if (zeroStock(products)){
        cout << "Semua stok produk telah habis !!! Supermarketmu mau jualan apa ???" << endl;
        game = false;
    }else if (duit.bangkrut())
    {
        game = false;
    }

    }
    return 0;
}