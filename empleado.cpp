#include <iostream>
#include <fstream>
#include <string>

/* Registro de longitud fija en binario sin delimitadores, agregar, eliminar, editar, imprimir */

using namespace std;

class Empleado {
    private:
        int id; // Max 9 chars
        char nombre[50]; // 50 chars max
        long telefono; // 10 chars 
        char email[50]; // 50 chars max
    public:
        Empleado () {
            id = 0;
            nombre[0] = '\0';
            telefono = 0;
            email[0] = '\0';
        }
        // Write all the getters and setters for the class
        void setCodigo(int newCodigo) {
            id = newCodigo;
        }
        void setTelefono(long newTelefono) {
            telefono = newTelefono;
        }
        void setNombre(const string &newNombre) {
            int i;
            for (i = 0; i < newNombre.length(); i++) {
                nombre[i] = newNombre[i];
                if(i == 49){
                    nombre[50] = '\0';
                    break;
                }
            }
            nombre[i+1] = '\0';
        }
        void setEmail(const string &newEmail) {
            for (int i = 0; i < newEmail.length(); i++) {
                email[i] = newEmail[i];
                if(i == 49){
                    email[50] = '\0';
                    break;
                }
            }
        }
        [[nodiscard]] int getCodigo() const {
            return id;
        }
        [[nodiscard]] string getNombre() const {
            string str(nombre);
            return str;
        }
        [[nodiscard]] long getTelefono() const {
            return telefono;
        }
        [[nodiscard]] string getEmail() const {
            string str(email);
            return str;
        }

        void insertar() {
            ofstream outputFile("backups/employees.bin", ios::app | ios::binary);
            Empleado empleado;
            string auxString;
            do{    
                cout << "Ingresar id de empleado: ";
                getline(cin, auxString, '\n');
            }while(!containsOnlyNumber(auxString) || auxString.length() > 9 || auxString.length() == 0);
            empleado.setCodigo(stoi(auxString));
            do{
                cout << "Ingresar nombre de empleado: ";
                getline(cin, auxString, '\n');
            }while(containsSpecialCharacter(auxString) || auxString.length() > 50 || auxString.length() == 0);
            empleado.setNombre(auxString);
            do{
                cout << "Ingresar telefono de empleado: ";
                getline(cin, auxString, '\n');
            }while(!containsOnlyNumber(auxString) || auxString.length() > 10 || auxString.length() == 0);
            empleado.setTelefono(stol(auxString));
            do{
                cout << "Ingresar email de empleado: ";
                getline(cin, auxString, '\n');
            }while(auxString.length() > 50 || auxString.length() == 0);
            empleado.setEmail(auxString);

            outputFile.write((char*)&empleado, sizeof(empleado));
            outputFile.close();
        }
        void imprimir() {
            ifstream inputFile("backups/employees.bin", ios::in | ios::binary);
            if(!inputFile.good()){
                cout << "Archivo de empleados inexistente\n";
            }
            Empleado empleado;
            while(inputFile.peek() != -1){
                inputFile.read((char*)&empleado, sizeof(empleado));
                cout << empleado.toString() << endl;
            }
            inputFile.close();
        }
        void buscar() {
            ifstream inputFile("backups/employees.bin", ios::in | ios::binary);
            if(!inputFile.good()){
                cout << "Archivo de empleados inexistente\n";
            }
            Empleado empleado;
            string id;
            cout << "Ingresar el id del empleado: ";
            getline(cin, id, '\n');

            while(inputFile.peek() != -1){
                inputFile.read((char*)&empleado, sizeof(empleado));
                if(empleado.getCodigo() == stoi(id)){
                    cout << empleado.toString();
                    inputFile.close();
                    return;
                }
            }
            cout << "No se encontro la empleado.\n";
            inputFile.close();
        }
        void modificar() {
            ofstream outputFile("backups/aux.bin", ios::out | ios::binary);
            ifstream inputFile("backups/employees.bin", ios::in | ios::binary);
            if(!inputFile.good()){
                cout << "Archivo de empleados inexistente\n";
            }
            Empleado empleado;
            bool found = false;
            int option = -1;
            string id, auxString;
            cout << "Ingresar el id de la empleado: ";
            getline(cin, id, '\n');
            while(inputFile.peek() != -1){
                inputFile.read((char*)&empleado,sizeof(empleado));
                if(empleado.getCodigo() == stoi(id)){
                    found = true;
                    cout << "\nProducto encontrado:\n" << empleado.toString()<<endl;
                    while(option != 0){
                        cout << "1. Codigo\n2. Nombre\n3. Telefono\n4. Email\n0. Salir\n";
                        cout << "Seleccione elemento para editar: ";
                        cin >> option;
                        cin.ignore();
                        switch(option) {
                            case 1:
                                do{    
                                    cout << "Ingresar nuevo codigo de empleado: ";
                                    getline(cin, auxString, '\n');
                                }while(!containsOnlyNumber(auxString) || auxString.length() > 9 || auxString.length() == 0);
                                empleado.setCodigo(stoi(auxString));
                                break;
                            case 2:
                                do{
                                    cout << "Ingresar nuevo nombre de empleado: ";
                                    getline(cin, auxString, '\n');
                                }while(containsSpecialCharacter(auxString) || auxString.length() > 50 || auxString.length() == 0);
                                empleado.setNombre(auxString);
                                break;
                            case 3: 
                                do{
                                    cout << "Ingresar nuevo telefono de empleado: ";
                                    getline(cin, auxString, '\n');
                                }while(!containsOnlyNumber(auxString) || auxString.length() > 10 || auxString.length() == 0);
                                empleado.setTelefono(stol(auxString));
                                break;
                            case 4:
                                do{
                                    cout << "Ingresar email de empleado: ";
                                    getline(cin, auxString, '\n');
                                }while(auxString.length() > 50 || auxString.length() == 0);
                                empleado.setEmail(auxString);
                                break;
                            case 0:
                                break;
                            default:
                                cout<<"Opción incorrecta"<<endl;
                        }
                    }
                }
                outputFile.write((char*)&empleado,sizeof(empleado));
            }
            if(!found){
                cout << "Producto no encontrado\n";
            } else {
                cout << "Producto modificado exitosamente\n";
            }
            inputFile.close();
            outputFile.close();
            remove("backups/employees.bin");
            rename("backups/aux.bin", "backups/employees.bin");
        }
        void eliminar() {
            ofstream outputFile("backups/aux.bin", ios::out | ios::binary);
            ifstream inputFile("backups/employees.bin", ios::in | ios::binary);
            if(!inputFile.good()){
                cout << "Archivo de empleados inexistente\n";
            }
            Empleado empleado;
            string id;
            bool found = false;
            cout << "Ingresar el id de la empleado a eliminar: ";
            getline(cin, id, '\n');
            while(inputFile.peek() != -1){
                inputFile.read((char*)&empleado,sizeof(empleado));
                if(empleado.getCodigo() == stoi(id)){
                    found = true;
                } else {
                    outputFile.write((char*)&empleado,sizeof(empleado)); 
                }
            }
            if(!found){
                cout << "No se encontró el empleado" << endl; 
            }else{
                cout << "Empleado eliminado" << endl; 
            }
            inputFile.close();
            outputFile.close();
            remove("backups/employees.bin");
            rename("backups/aux.bin", "backups/employees.bin");
        }
        [[nodiscard]] string toString() {
            return "Codigo: " + to_string(id) + "\nNombre: " + string(nombre) + "\nTelefono: " + to_string(telefono) + "\nEmail: " + string(email) + '\n';
        }
        bool containsOnlyNumber(const string &str){
        for(char c : str){
            if(!isdigit(c)){
                return false;
            }
        }
        return true;
    }
        bool containsSpecialCharacter(const string& str) const {
        for (char c : str) {
            if (!isalnum(c) && c != ' ') {
                return true;
            }
        }
        return false;
    }
        string rightPad(string str,int maxLength) {
            string aux = str.append(maxLength - str.length(),' ');
            return aux;
        }
        string removeBlankSpaces(string& str){
            string cleanString;
            for(int c = 0;str[c] != ' '; c++){
                cleanString += str[c];
            }
            return cleanString;
        }
};

int main(){
    Empleado empleado;
    int option = 0;
    while (option != 6) {
        cout<<"Gestion de Empleados\n";
        cout<<"====================\n";
        cout<<"\nOpciones:\n";
        cout<<"1. Agregar nuevo empleado\n";
        cout<<"2. Imprimir todos los empleados\n";
        cout<<"3. Buscar empleado\n";
        cout<<"4. Modificar empleado\n";
        cout<<"5. Eliminar empleado\n";
        cout<<"6. Salir\n";
        cout<<"Seleccione una opcion: ";
        cin>>option;
        cin.ignore();
        switch (option) {
            case 1:
                empleado.insertar();
                break;
            case 2:
                empleado.imprimir();
                break;
            case 3:
                empleado.buscar();
                break;
            case 4:
                empleado.modificar();
                break;
            case 5:
                empleado.eliminar();
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
