#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "indexServicio.cpp"

/* Registro de longitud variable usando indización */

using namespace std;

string servicesIndexFile = "backups/servicesIndex.txt";
string servicesIndexFileAux = "backups/servicesIndexAux.txt";
string servicesFile = "backups/services.txt";
string servicesFileAux = "backups/servicesAux.txt";

class Servicio {
private:
    int id; // Max 9 chars
    char cost[11]; // 10 chars (7 cypher, 2 decimals and dot)
    char name[31]; // 30 chars ~ 49 chars (51 including nulls) 
public:
    Servicio(){};
    Servicio(int newId, const string &newCost, const string &newNombre);

    [[nodiscard]] int getId() const;
    [[nodiscard]] string getCost() const;
    [[nodiscard]] string getName() const;
    
    void setId(int newId);
    void setCost(const string &newCost);
    void setName(const string &newName);

    void agregar();
    void imprimir();
    void buscar();
    void modificar();
    // void editarCampos(Servicio &servicio, bool &changed);
    void eliminar();

    [[nodiscard]] string toString() const;
    [[nodiscard]] string toUpper(const string &initialString);
    [[nodiscard]] bool costValidation(const string& price);
    [[nodiscard]] bool containsSpecialCharacter(const string& str) const;
    Servicio editarCampos(Servicio servicio);

    friend ofstream &operator << (ofstream &out, const Servicio &servicio) {
        out.write((char*)&servicio, sizeof(servicio));
        return out;
    }
    friend istream &operator >> (istream &in, Servicio &servicio){
        in.read((char*)&servicio, sizeof(servicio));
        return in;
    }
};

Servicio::Servicio(int newId, const string &newCost, const string &newNombre) {
    this->id = newId;
    this->setCost(newCost);
    this->setName(newNombre);
}
int Servicio::getId() const {
    return this->id;
}
string Servicio::getCost() const {
    return string(this->cost);
}
string Servicio::getName() const {
    return string(this->name);
}
void Servicio::setId(int newId) {
    this->id = newId;
}
void Servicio::setCost(const string &newCost) {
    int i;
    for (i = 0; i < newCost.length(); i++) {
        cost[i] = newCost[i];
        if(i == 9){
            cost[10] = '\0';
            break;
        }
    }
    cost[i] = '\0';
}
void Servicio::setName(const string &newName) {
    int i;
    for (i = 0; i < newName.length(); i++) {
        name[i] = newName[i];
        if(i == 29){
            name[30] = '\0';
            break;
        }
    }
    name[i] = '\0';
}

[[nodiscard]] string Servicio::toString() const {
    return "ID: " + to_string(this->id) + "\nNombre: " + this->getName() + "\nCosto: " + this->getCost() + '\n';
}
[[nodiscard]] string Servicio::toUpper(const string &initialString){
    string finalString;
    for (char caracter : initialString){
        finalString += toupper(caracter);
    }
    return finalString;
}
bool Servicio::costValidation(const string& price){
    int priceLength = price.length();
    if(priceLength > 10){
        cout << "Numero demasiado grande" << endl;
        return false;
    }
    for(int i = 0; i < priceLength-1; i++){
        if(!isdigit(price[i]) && price[i] != '.'){
            cout << "Formato de numero incorrecto" << endl; 
            return false;
        }
        if(price[i] == '.' && price[i+3] != '\0'){
            cout << "El precio solo debe tener dos decimales" << endl;
            return false;
        }
    }
    return true;
}
bool Servicio::containsSpecialCharacter(const string& str) const {
    for (char c : str) {
        if (!isalnum(c) && c != ' ') {
            return true;
        }
    }
    return false;
}
bool containsOnlyNumber(const string &str){
    for(char c : str){
        if(!isdigit(c)){
            return false;
        }
    }
    return true;
}
void Servicio::agregar() {
    IndexTuple index;
    Servicio servicio;
    string strAux;
    do{
        cout << "ID: ";
        getline(cin, strAux, '\n');
    }while(containsSpecialCharacter(strAux) || !containsOnlyNumber(strAux));
    servicio.setId(stoi(strAux));
    do{
        cout << "Nombre: ";
        getline(cin, strAux, '\n');
    } while (containsSpecialCharacter(strAux) || strAux.length() > 30 || strAux.length() == 0);
    servicio.setName(strAux);
    do{
        cout << "Costo: ";
        getline(cin, strAux, '\n');
    } while (!costValidation(strAux) || strAux.length() == 0);
    servicio.setCost(strAux);

    ofstream outputServicesFile (servicesFile, ios::ate | ios::app);
    ofstream outputIndexFile (servicesIndexFile, ios::app);
    
    index.setIndex(outputServicesFile.tellp());
    index.setId(servicio.getId());
    outputIndexFile << index;
    outputIndexFile.close();

    outputServicesFile << servicio;
    outputServicesFile.close();
}
void Servicio::imprimir() {
    ifstream inputServicesFile (servicesFile, ios::in);
    if(!inputServicesFile.good()){
        cout << "Archivo de servicios inexistente" << endl;
        return; 
    }
    Servicio servicio;
    while(inputServicesFile.peek() != -1){
        inputServicesFile >> servicio;
        cout << servicio.toString() << endl; 
    }
    inputServicesFile.close();
}
void Servicio::buscar() {
    ifstream inputIndexFile (servicesIndexFile, ios::in);
    ifstream inputServicesFile (servicesFile, ios::in);
    if(!inputIndexFile.good()){
        cout<<"Archivo de índices de servicios no existente"<<endl;
        return;
    }
    if(!inputServicesFile.good()){
        cout<<"Archivo de servicios no existente"<<endl;
        return;
    }

    Servicio servicio;
    IndexTuple index;
    string idBuscado;
    do{
        cout << "Ingresa ID de servicio:";
        getline(cin, idBuscado,'\n');
    } while(!containsOnlyNumber(idBuscado) || idBuscado.length() == 0);
    
    while(inputIndexFile.peek() != -1){
        inputIndexFile >> index;
        if(index.getId() == stoi(idBuscado)){
            inputServicesFile.seekg(index.getIndex());
            inputServicesFile >> servicio;
            cout << servicio.toString();
            inputIndexFile.close();
            inputServicesFile.close();
            return;
        }
    }
    cout << "Servicio no encontrado" << endl; 
    inputIndexFile.close();
    inputServicesFile.close();
}
Servicio Servicio::editarCampos(Servicio servicio){
    string strAux;
    int option = -1;
    while(option != 0){
        cout << "1. ID\n2. Nombre\n3. Costo\n0. Salir\n";
        cout << "Seleccione elemento para editar: ";
        cin >> option;
        cin.ignore();

        switch (option) {
        case 1:
            do{
                cout << "Ingresa nuevo ID de producto: ";
                getline(cin, strAux, '\n');
            }while(containsSpecialCharacter(strAux) || !containsOnlyNumber(strAux));
            servicio.setId(stoi(strAux));
            break;
        case 2:
            do{
                cout << "Nombre: ";
                getline(cin, strAux, '\n');
            } while (containsSpecialCharacter(strAux) || strAux.length() > 30 || strAux.length() == 0);
            servicio.setName(strAux);
            break;
        case 3:
            do{
                cout << "Costo: ";
                getline(cin, strAux, '\n');
            } while (!costValidation(strAux) || strAux.length() == 0);
            servicio.setCost(strAux);
            break;
        case 0:
            break;
        default:
            cout << "Opción inválida" << endl;
            break;
        }
    }
    return servicio;
}
void Servicio::modificar() {
    ifstream inputServicesFile(servicesFile, ios::in);
    ofstream outputServicesFile(servicesFile, ios::in | ios::ate);
    ifstream inputServicesIndexFile(servicesIndexFile, ios::in);
    ofstream outputServicesIndexFile(servicesIndexFileAux, ios::out);
    if(!inputServicesFile.good()){
        cout<<"Archivo de servicios no existente"<<endl;
        return;
    }
    if(!inputServicesIndexFile.good()){
        cout<<"Archivo de indices de servicios no existente"<<endl;
        return;
    }

    Servicio servicio;
    IndexTuple index;
    string idBuscado;

    cout << "Ingrese ID del servicio a modificar: "; 
    getline(cin, idBuscado, '\n');
    while(inputServicesIndexFile.peek() != -1){
        inputServicesIndexFile >> index;
        if(index.getId() == stoi(idBuscado)){
            inputServicesFile.seekg(index.getIndex());
            inputServicesFile >> servicio;
            cout << "Servicio encontrado:" << endl;
            cout << servicio.toString() << endl;
            servicio = editarCampos(servicio);
            outputServicesFile.seekp(index.getIndex());
            outputServicesFile << servicio;
            index.setId(servicio.getId());
        }
        outputServicesIndexFile << index;
    }
     
    inputServicesFile.close();
    inputServicesIndexFile.close();
    outputServicesIndexFile.close();

    remove(servicesIndexFile.c_str());
    rename(servicesIndexFileAux.c_str(), servicesIndexFile.c_str());
}
void Servicio::eliminar() {
    ifstream inputIndexFile (servicesIndexFile, ios::in);
    ifstream inputServicesFile (servicesFile, ios::in);
    ofstream outputIndexFile (servicesIndexFileAux, ios::out);
    ofstream outputServicesFile (servicesFileAux, ios::out);

    if(!inputIndexFile.good()){
        cout<<"Archivo de índices de servicios no existente"<<endl;
        return;
    }
    if(!inputServicesFile.good()){
        cout<<"Archivo de servicios no existente"<<endl;
        return;
    }
    Servicio servicio;
    IndexTuple index;
    string idBuscado = " ";
    int idRecordToOmit = 0, previousRecordIndex = 0, actualRecordIndex = 0;
    bool changeIndex = false; 
    do{
        cout << "Ingresa ID de servicio a eliminar: ";
        getline(cin, idBuscado,'\n');
    } while(!containsOnlyNumber(idBuscado) || idBuscado.length() == 0);
    idRecordToOmit = stoi(idBuscado);

    while(inputIndexFile.peek() != -1){
        inputIndexFile >> index;
        if(index.getId() == idRecordToOmit) {
            inputIndexFile.seekg(0);
            while(inputServicesFile.peek() != -1){
                inputIndexFile >> index;
                inputServicesFile >> servicio;
                if(idRecordToOmit == index.getId()){
                    previousRecordIndex = index.getIndex();
                    changeIndex = true;
                    continue;
                }
                if(changeIndex){
                    actualRecordIndex = index.getIndex();
                    index.setIndex(previousRecordIndex);
                    previousRecordIndex = actualRecordIndex;
                    outputIndexFile << index;
                    outputServicesFile << servicio;
                }else{
                    outputIndexFile << index;
                    outputServicesFile << servicio;
                }
            }

            inputIndexFile.close();
            inputServicesFile.close();
            outputIndexFile.close();
            outputServicesFile.close();

            remove(servicesFile.c_str());
            remove(servicesIndexFile.c_str());
            rename(servicesFileAux.c_str(), servicesFile.c_str());
            rename(servicesIndexFileAux.c_str(), servicesIndexFile.c_str());
            
            return;
        }
    }
    cout << "Servicio inexistente" << endl;
}


int main (){
    Servicio servicio;
    int option;
    while(option != 6){
        cout<<"Gestion de Servicios\n====================\n";
        cout<<"1. Agregar\n2. Imprimir\n3. Buscar\n4. Modificar\n5. Eliminar\n6. Salir\n";
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
            case 3:
                servicio.buscar();
                break;
            case 4:
                servicio.modificar();
                break;
            case 5:
                servicio.eliminar(); // TODO
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
