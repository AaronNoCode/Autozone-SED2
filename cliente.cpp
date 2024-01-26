#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/* Registros de longitud variable con delimitadores, agregar, imprimir, buscar, modificar y eliminar */

using std::ios, std::rename, std::stringstream, std::ofstream, std::ifstream, std::string, std::cout, std::endl, std::ostream, std::istream, std::to_string, std::getline, std::cin, std::toupper;

class Customer {
    private:
        string name;
        string email;
        string rfc;
        string id;
        string phone;
    public:
        Customer();
        Customer(string newName, string newEmail, string newRfc, string newId, string newPhone);

        // Getters
        [[nodiscard]] string getName() const;
        [[nodiscard]] string getEmail() const; 
        [[nodiscard]] string getRfc() const;
        [[nodiscard]] string getId() const;
        [[nodiscard]] string getPhone() const;

        // Setters
        void setPhone(const string&);
        void setName(const string&);
        void setEmail(const string&);
        void setRfc(const string&);
        void setId(const string&);

        [[nodiscard]] string toString() const;
        [[nodiscard]] string toUpper(const string& str) const;

        void agregar();
        void imprimir();
        void buscar();
        void modificar();
        void eliminar();

        friend ostream &operator << (ostream &out, const Customer &customer) {
            out << customer.name << '*';
            out << customer.email << '*';
            out << customer.rfc << '*';
            out << customer.id << '*';
            out << customer.phone << '#';
            return out;
        }
        friend istream &operator >> (istream &in, Customer &customer) {
            getline(in, customer.name,'*');
            getline(in, customer.email,'*');
            getline(in, customer.rfc, '*');
            getline(in, customer.id, '*');
            getline(in, customer.phone, '#');
            return in;
        }
};
Customer::Customer(){}
Customer::Customer(string newName, string newEmail, string newRfc, string newId, string newPhone)
{
    this->name = newName;
    this->email = newEmail;
    this->rfc = newRfc;
    this->id = newId;
    this->phone = newPhone;
}

void Customer::setPhone(const string& newPhone) {
    this->phone = newPhone;
}
void Customer::setName(const string& newName) {
    this->name = newName;
}
void Customer::setEmail(const string& newEmail) {
    this->email = newEmail;
}
void Customer::setRfc(const string& newRfc) {
    this->rfc = newRfc;
}
void Customer::setId(const string& newId) {
    this->id = newId;
}

[[nodiscard]] string Customer::getName() const{
    return this->name;
}
[[nodiscard]] string Customer::getEmail() const{
    return this->email;
}
[[nodiscard]] string Customer::getRfc() const{
    return this->rfc;
}
[[nodiscard]] string Customer::getId() const{
    return this->id;
}
[[nodiscard]] string Customer::getPhone() const{
    return this->phone;
}

void Customer::agregar() {
    Customer customer;
    cout << "Name: ";
    getline(cin, customer.name, '\n');
    cout << "Email: ";
    getline(cin, customer.email, '\n');
    cout << "RFC: ";
    getline(cin, customer.rfc, '\n');
    cout << "ID: ";
    getline(cin, customer.id, '\n');
    cout << "Phone: ";
    getline(cin, customer.phone, '\n');
    ofstream ofs ("backups/customers.txt",ios::app);
    ofs << customer;
    ofs.close();
}
void Customer::buscar() {
    Customer customer;
    string name, line;
    stringstream ss;
    ifstream inputFile ("backups/customers.txt", ios::in);
    if(!inputFile.good()){
        cout << "Archivo inexistente" << endl;
        return;
    }

    cout << "Nombre a buscar: ";
    getline(cin, name,'\n');
    while(getline(inputFile, line, '#')){
        ss << line;
        ss >> customer;
        ss.clear();
        if (customer.toUpper(customer.getName()) == toUpper(name)) {
            cout << customer.toString() << endl<< endl;
            inputFile.close();
            return;
        }
    }
    cout << "No se encontro el cliente" << endl;
    inputFile.close();
}
void Customer::imprimir() {
    ifstream inputFile ("backups/customers.txt", ios::in);
    if(!inputFile.good()){
        cout << "Archivo inexistente" << endl;
        return;
    }
    Customer customer;
    while(!inputFile.eof()){
        inputFile >> customer;
        if(customer.getName() != "")
            cout << customer.toString() << endl << endl;
    }
    inputFile.close();
}
void Customer::modificar() {
    ofstream auxFile("backups/aux.txt", ios::out);
    ifstream inputFile("backups/customers.txt", ios::in);
    stringstream ss;
    Customer customer;
    string line, name;
    int option;
    cout << "Nombre del cliente a editar: ";
    getline(cin, name, '\n');

    while(getline(inputFile, line, '#')){
        ss << line;
        ss >> customer;
        ss.clear();

        if(toUpper(customer.getName()) == toUpper(name)){
            cout << customer.toString() << endl;
            while(option != 6){
                cout << "1. Nombre\n2. Email\n3. RFC\n4. ID\n5. Telefono\n6. Salir\n";
                cout << "Seleccione elemento para editar: ";
                cin >> option;
                cin.ignore();
                switch(option) {
                    case 1:
                        cout<<"Nuevo nombre: ";
                        getline(cin, customer.name, '\n');
                        break;
                    case 2:
                        cout<<"Nuevo email: ";
                        getline(cin, customer.email, '\n');
                        break;
                    case 3:
                        cout<<"Nuevo RFC: ";
                        getline(cin, customer.rfc, '\n');
                        break;
                    case 4:
                        cout<<"Nuevo ID: ";
                        getline(cin, customer.id, '\n');
                        break;
                    case 5:
                        cout<<"Nuevo telefono: ";
                        getline(cin, customer.phone, '\n');
                        break;
                    case 6:
                        break;
                    default:
                        cout<<"Opción incorrecta"<<endl;
                }
            }
            auxFile << customer;
        }else{
            auxFile << customer;
        }
    }
    inputFile.close();
    auxFile.close();
    remove("backups/customers.txt");
    rename("backups/aux.txt", "backups/customers.txt");
}
void Customer::eliminar() {
    ofstream auxFile ("backups/aux.txt", ios::out);
    ifstream inputFile("backups/customers.txt", ios::in);
    Customer customer;
    stringstream ss;
    string line, name;
    bool confirmDeletion = false;
    if(!inputFile.good()){
        cout << "Archivo inexistente" << endl;
        return;
    }

    cout << "Nombre de cliente a eliminar: ";
    getline(cin, name, '\n');
    while(getline(inputFile, line, '#')){
        ss << line;
        ss >> customer;
        ss.clear();

        if(toUpper(customer.getName()) == toUpper(name))
            confirmDeletion = true;
        else
            auxFile << customer;
    }
    confirmDeletion?cout<<name<<" eliminado"<<endl : cout<<"No se encontro el cliente"<<endl;
    inputFile.close();
    auxFile.close();
    remove("backups/customers.txt");
    rename("backups/aux.txt", "backups/customers.txt");
}

[[nodiscard]] string Customer::toString() const{
    return "Name: " + this->getName() + "\nEmail: " + this->getEmail() + "\nRFC: " + this->getRfc() + "\nID: " + this->getId() + "\nPhone: " + this->getPhone();
}
[[nodiscard]] string Customer::toUpper(const string& str) const {
    string newStr = "";
    for (char i : str) {
        newStr += toupper(i);
    }
    return newStr;
}

int main () {
    Customer c;
    int option;
    cout<<"1. Agregar\n2. Imprimir\n3. Buscar\n4. Modificar\n5. Eliminar\n6. Salir\n";
    while(option != 6){
        cout<<"\nOpcion: ";
        cin>>option;
        cin.ignore();
        switch (option) {
            case 1: 
                c.agregar();
                break;
            case 2:
                c.imprimir();
                break;
            case 3:
                c.buscar();
                break;
            case 4:
                c.modificar();
                break;
            case 5:
                c.eliminar();
                break;
            case 6: // Salir
                break;
            default:
                cout << "Opcion no valida" << endl;
                break;
        }
    }
    cout<<"\n\nEnd";
    return 0;
}
