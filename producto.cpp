#include <iostream>
#include <fstream>
#include <sstream>

/* Registros de longitud variable con campos de dimensión, agregar, imprimir, buscar, modificar y eliminar */

using namespace std;

class Product {
private:
    string code; // El código es una cadena
    string name;
    string description;
    string price;

public:
    Product();
    Product(const string& newCode, const string& newName, const string& newPrice, const string& newDescription);

    // Getters
    [[nodiscard]] string getCode() const { return this->code; }
    [[nodiscard]] string getName() const { return this->name; }
    [[nodiscard]] string getPrice() const { return this->price; }
    [[nodiscard]] string getDescription() const { return this->description; }

    // Setters
    void setCode(const string& newCode) { this->code = newCode; }
    void setName(const string& newName) { this->name = newName; }
    void setPrice(const string& newPrice) { this->price = newPrice; }
    void setDescription(const string& newDescription) { this->description = newDescription; }

    // Métodos
    void insertar();
    void imprimir() const;
    void buscar() const;
    void modificar();
    void eliminar() const; 

    friend ostream &operator << (ostream &out, const Product &product) {
        out << product.leftPad(product.code) << product.code;
        out << product.leftPad(product.name) << product.name;
        out << product.leftPad(product.price) << product.price;
        out << product.leftPad(product.description) << product.description;
        return out;
    }
    friend ifstream &operator >>(ifstream &is, Product &p){
        char fieldLength[3] = {0};
        char code[100] = {0};
        char name[100] = {0};
        char description[100] = {0};
        char price[20] = {0};
        string stringifiedField; 
        
        is.read(fieldLength, 2);
        is.read(code,stoi(fieldLength));
        stringifiedField = string(code);
        p.setCode(stringifiedField);

        is.read(fieldLength, 2);
        is.read(name,stoi(fieldLength));
        stringifiedField = string(name);
        p.setName(stringifiedField);
        
        is.read(fieldLength, 2);
        is.read(price,stoi(fieldLength));
        stringifiedField = string(price);
        p.setPrice(stringifiedField);
        
        is.read(fieldLength, 2);
        is.read(description,stoi(fieldLength));
        stringifiedField = string(description);
        p.setDescription(stringifiedField);

        return is;
    }

    string toString() const {
        return "Codigo: " + this->getCode() + "\n" + "Nombre: " + this->getName() + "\n" + "Precio: " + this->getPrice()  + "\n" + "Descripcion: " + this->getDescription() + "\n"; 
    } 
    string leftPad(const string &str) const {
        if(str.length() > 9){
            return to_string(str.length());
        }else{
            return '0' + to_string(str.length());
        }
    }
    bool containsSpecialCharacter(const string& str) const {
        for (char c : str) {
            if (!isalnum(c)) {
                return true;
            }
        }
        return false;
    }
    bool containsOnlyNumberOrDot(const string &str){
        for(char c : str){
            if(!isdigit(c) && c != '.'){
                return false;
            }
        }
        return true;
    }
};  

Product::Product(){
    code = ' ';
    name = ' ';
    description = ' ';
    price = ' ';
}

Product::Product(const string& newCode, const string& newName, const string& newPrice, const string& newDescription){
    code = newCode;
    name = newName;
    description = newDescription;
    price = newPrice;
}

void Product::insertar() {
    Product product;
    do{    
        cout << "Ingresar codigo de producto: ";
        getline(cin, product.code, '\n');
    }while(containsSpecialCharacter(product.getCode()));
    do{
        cout << "Ingresar nombre: ";
        getline(cin, product.name, '\n');
    }while(containsSpecialCharacter(product.getName()));
    do{
        cout << "Ingresar precio: ";
        getline(cin, product.price, '\n');
    }while(!containsOnlyNumberOrDot(product.getPrice()));
    do{
        cout << "Ingresar descripcion: "; 
        getline(cin, product.description, '\n');
    }while(containsSpecialCharacter(product.getDescription()));

    ofstream ofs("backups/products.txt", ios::app);
    ofs << product;
    ofs.close();

    cout << "Producto agregado exitosamente.\n";
}

void Product::imprimir() const {
    ifstream inputFile("backups/products.txt", ios::in);
    if (!inputFile.good()) {
        cout << "Archivo inexistente" << endl;
        return;
    }

    Product p;
    while (inputFile.peek() != -1){
        inputFile >> p;
        cout << p.toString() << endl;
    }

    inputFile.close();
}

void Product::buscar() const {
    string searchCode;
    cout << "Ingrese el codigo del producto a buscar: ";
    getline(cin, searchCode, '\n');

    ifstream inputFile("backups/products.txt", ios::in);
    if (!inputFile.good()) {
        cout<<"Archivo inexistente"<<endl;
        return;
    }

    Product product;
    while (inputFile.peek() != -1){
        inputFile >> product;
        if (product.getCode() == searchCode) {
            cout << "Producto entontrado:\n" << product.toString();
            return;
        }
    }
    cout << "Producto no encontrado\n";
    inputFile.close();
}

void Product::modificar() {
    Product product;
    string searchCode;
    int option = 0;
    bool found = false;
    cout << "Ingrese el codigo del producto a modificar: ";
    getline(cin, searchCode, '\n');

    ifstream inputFile("backups/products.txt",ios::in);
    ofstream auxFile("backups/aux.txt",ios::out);
    if (!inputFile.good()) {
        cout<<"Archivo inexistente\n";
        return;
    }
    while(inputFile.peek() != -1){
        inputFile >> product;
        if(product.getCode() == searchCode){
            found = true;
            cout << "Producto encontrado:\n" << product.toString();
            while(option != 6){
                cout << "1. Codigo\n2. Nombre\n3. Precio\n4. Descripcion\n6. Salir\n";
                cout << "Seleccione elemento para editar: ";
                cin >> option;
                cin.ignore();
                switch(option) {
                    case 1:
                        cout<<"Nuevo codigo: ";
                        getline(cin, product.code, '\n');
                        break;
                    case 2:
                        cout<<"Nuevo nombre: ";
                        getline(cin, product.name, '\n');
                        break;
                    case 3:
                        cout<<"Nuevo precio: ";
                        getline(cin, product.price, '\n');
                        break;
                    case 4:
                        cout<<"Nueva descripcion: ";
                        getline(cin, product.description, '\n');
                        break;
                    case 6:
                        break;
                    default:
                        cout<<"Opción incorrecta"<<endl;
                }
            }
            auxFile << product;
        }else{
            auxFile << product;
        }
    }
    if(!found){
        cout << "Producto no encontrado\n";
    } else {
        cout << "Producto modificado exitosamente\n";
    }
    inputFile.close();
    auxFile.close();
    remove("backups/products.txt");
    rename("backups/aux.txt", "backups/products.txt");
}

void Product::eliminar() const {
    Product product;
    string searchCode;
    cout<<"Ingrese el código del producto a eliminar: ";
    getline(cin, searchCode,'\n');
    bool found = false;

    ifstream inputFile("backups/products.txt");
    ofstream auxFile("backups/aux.txt");
    if (!inputFile.good()) {
        cout<<"Archivo inexistente\n";
        return;
    }
    while (inputFile.peek() != -1) {
        inputFile >> product;
        if (product.getCode() == searchCode){
            found = true;
        }else{
            auxFile << product;
        } 
    }
    if (found) {
        cout << "Producto eliminado exitosamente\n";
    } else {
        cout << "Producto no encontrado\n";
    }
    inputFile.close();
    auxFile.close();
    remove("backups/products.txt");
    rename("backups/aux.txt", "backups/products.txt");
}

int main() {
    Product p;
    int option = 0;
    cout<<"Gestion de Productos\n";
    cout<<"====================\n";
    while (option != 6) {
        cout<<"\nOpciones:\n";
        cout<<"1. Agregar nuevo producto\n";
        cout<<"2. Imprimir todos los productos\n";
        cout<<"3. Buscar producto\n";
        cout<<"4. Modificar producto\n";
        cout<<"5. Eliminar producto\n";
        cout<<"6. Salir\n";
        cout<<"Seleccione una opcion: ";
        cin>>option;
        cin.ignore();
        switch (option) {
            case 1:
                p.insertar();
                break;
            case 2:
                p.imprimir();
                break;
            case 3:
                p.buscar();
                break;
            case 4:
                p.modificar();
                break;
            case 5:
                p.eliminar();
                break;
            case 6:
                cout<<"Saliendo del programa.\n";
                break;
            default:
                cout<<"Opcion no valida, intente nuevamente.\n";
                break;
        }
    }
    return 0;
}
