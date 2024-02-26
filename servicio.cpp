#include <iostream>
#include <fstream>
#include <string>
#include "indexServicio.cpp"

/* Registro de longitud variable usando indización y separadores */

using namespace std;

class Servicio {
private:
    string servicesIndexFile = "backups/servicesIndex.txt";
    string servicesIndexFileAux = "backups/servicesIndexAux.txt";
    string servicesFile = "backups/services.txt";
    string servicesFileAux = "backups/servicesAux.txt";
    string id;
    string cost;
    string name;
public:
    Servicio(){};
    Servicio(const string &newId, const string &newCost, const string &newNombre);

    [[nodiscard]] string getId() const;
    [[nodiscard]] string getCost() const;
    [[nodiscard]] string getName() const;
    
    void setId(const string &newId);
    void setCost(const string &newCost);
    void setName(const string &newName);

    void agregar();
    void imprimir();
    void buscar();
    void modificar();
    void editarCampos(Servicio &servicio, bool &changed);
    void eliminar();

    [[nodiscard]] string toString() const;
    [[nodiscard]] string toUpper(const string &initialString);

    friend ofstream &operator << (ofstream &out, const Servicio &servicio) {
        out << servicio.id << '*';
        out << servicio.name << '*';
        out << servicio.cost << '#';
        return out;
    }
    friend istream &operator >> (istream &in, Servicio &servicio){
        getline(in, servicio.id,'*');
        getline(in, servicio.name,'*');
        getline(in, servicio.cost, '#');
        return in;
    }
};

Servicio::Servicio(const string &newId, const string &newCost, const string &newNombre) {
    this->id = newId;
    this->cost = newCost;
    this->name = newNombre;
}
string Servicio::getId() const {
    return this->id;
}
string Servicio::getCost() const {
    return this->cost;
}
string Servicio::getName() const {
    return this->name;
}
void Servicio::setId(const string &newId) {
    this->id = newId;
}
void Servicio::setCost(const string &newCost) {
    this->cost = newCost;
}
void Servicio::setName(const string &newName) {
    this->name = newName;
}

[[nodiscard]] string Servicio::toString() const {
    return "ID: " + this->id + "\nNombre: " + this->name + "\nCosto: " + this->cost + '\n';
}
[[nodiscard]] string Servicio::toUpper(const string &initialString){
    string finalString;
    for (char caracter : initialString){
        finalString += toupper(caracter);
    }
    return finalString;
}
void Servicio::agregar() {
    IndexTuple index;
    Servicio servicio;
    string strAux;
    cout << "ID: ";
    getline(cin, strAux, '\n');
    servicio.setId(toUpper(strAux));
    cout << "Nombre: ";
    getline(cin, servicio.name, '\n');
    cout << "Costo: ";
    getline(cin, servicio.cost, '\n');

    ofstream outputServicesFile (servicesFile, ios::in | ios::app | ios::ate);
    ofstream outputIndexFile (servicesIndexFile, ios::app);
    
    index.setIndex(outputServicesFile.tellp());
    index.setId(servicio.id);
    outputIndexFile << index;
    outputIndexFile.close();

    outputServicesFile << servicio;
    outputServicesFile.close();
}
void Servicio::imprimir() {
    ifstream inputServicesFile (servicesFile, ios::in);
    if(!inputServicesFile.good()){
        cout << "Archivo dde servicios inexistente" << endl;
        return; 
    }
    Servicio servicio;
    while(inputServicesFile.peek() != -1){
        inputServicesFile >> servicio;
        cout << servicio.toString() << endl; 
    }
    inputServicesFile.close();
}
void Servicio::buscar() {}
void Servicio::modificar() {}
void Servicio::eliminar() {}

int main (){
    Servicio servicio;
    int option;
    cout<<"1. Agregar\n2. Imprimir\n3. Buscar\n4. Modificar\n5. Eliminar\n6. Salir\n";
    while(option != 6){
        cout<<"\nOpcion: ";
        cin>>option;
        cin.ignore();
        switch (option) {
            case 1: 
                servicio.agregar();
                break;
            case 2:
                servicio.imprimir();
                break;
            /* case 3:
                servicio.buscar();
                break;
            case 4:
                servicio.modificar();
                break;
            case 5:
                servicio.eliminar();
                break; */
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
