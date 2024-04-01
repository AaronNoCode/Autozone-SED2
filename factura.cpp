#include <iostream>
#include <fstream>
#include <string>
#include "cliente.cpp"
#include "producto.cpp"
#include "fecha.cpp"
#include "invIndexTicket.cpp"

/* Registro de longitud fija sin delimitadores, agregar, eliminar, editar, imprimir con índices invertidos */

using namespace std;

class Factura {
    private:
        string facturaId; // 10 chars
        string clienteId; // 10 chars id
        string productoId; // 10 chars id
        Date fecha; //8 day + month + year
        string productQuantity; // 3 chars ~ 41 chars total
        InvertedIndex invIndex;
    public:
        Factura () {
            facturaId = ' ';
            productQuantity = ' ';
        }
        Factura (const string& newId, const string& newQuantity, const string& newClienteId, const string& newProductoId, const Date& newFecha) {
            this->facturaId = newId;
            this->productQuantity = newQuantity;
            this->clienteId = newClienteId;
            this->productoId = newProductoId;
            this->fecha = newFecha;
        }
        void setOrderId(const string& newId) {
            this->facturaId = newId;
        }
        void setQuantity(const string& newQuantity) {
            this->productQuantity = newQuantity;
        }
        void setClienteId(const string& newClienteId) {
            this->clienteId = newClienteId;
        }
        void setProductoId(const string& newProductoId) {
            this->productoId = newProductoId;
        }
        void setFecha(const Date& newFecha) {
            this->fecha = newFecha;
        }

        [[nodiscard]] string getOrderId() const {
            return this->facturaId;
        }
        [[nodiscard]] string getQuantity() const {
            return this->productQuantity;
        }
        [[nodiscard]] string getClienteId() const {
            return this->clienteId;
        }
        [[nodiscard]] string getProductoId() const {
            return this->productoId;
        }
        [[nodiscard]] Date getFecha() const {
            return this->fecha;
        }

        void insertar() {
            ofstream outputFile("backups/orders.txt", ios::app);
            Factura orden;
            do{    
                cout << "Ingresar codigo de orden: ";
                getline(cin, orden.facturaId, '\n');
            }while(containsSpecialCharacter(orden.getOrderId()));
            do{
                cout << "Ingresar cantidad: ";
                getline(cin, orden.productQuantity, '\n');
            }while(!containsOnlyNumber(orden.getQuantity()) || orden.getQuantity().length() > 3);
            do{
                cout << "Ingresar codigo de cliente: ";
                getline(cin, orden.clienteId, '\n');
            }while(containsSpecialCharacter(orden.getClienteId()));
            do{
                cout << "Ingresar codigo de producto: ";
                getline(cin, orden.productoId, '\n');
            }while(containsSpecialCharacter(orden.getProductoId()));
            do{
                cout << "Ingresar dia: ";
                cin >> orden.fecha.day;
            }while(orden.fecha.getDay() < 1 || orden.fecha.getDay() > 31);
            do{
                cout << "Ingresar mes: ";
                cin >> orden.fecha.month;
            }while(orden.fecha.getMonth() < 1 || orden.fecha.getMonth() > 12);
            do{
                cout << "Ingresar anio: ";
                cin >> orden.fecha.year;
            }while(orden.fecha.getYear() < 1900 || orden.fecha.getYear() > 9999);
            outputFile.seekp(0,ios::end);
            int index = outputFile.tellp();
            invIndex.add(orden.getClienteId(), index);
            outputFile << orden;
            outputFile.close();
        }
        void imprimir() {
            ifstream inputFile("backups/orders.txt", ios::in);
            if(!inputFile.good()){
                cout << "Archivo inexistente\n";
            }
            Factura orden;
            while(inputFile.peek() != -1){
                inputFile >>orden;
                cout << orden.toString() << endl;
            }
            inputFile.close();
        }
        void buscar() {
            ifstream inputFile("backups/orders.txt", ios::in);
            if(!inputFile.good()){
                cout << "Archivo inexistente\n";
            }
            Factura orden;
            vector<int> indexes;
            string id;
            int option;
            cout << "Buscar por:\n1. Codigo de orden\n2. Codigo de cliente\n";
            cin >> option;
            cin.ignore();
            switch (option){
            case 1:
                cout << "Ingresar el codigo de la orden: ";
                getline(cin, id, '\n');
                while(inputFile.peek() != -1){
                    inputFile >> orden;
                    if(orden.getOrderId() == id){
                        cout << orden.toString();
                        return;
                    }
                }
                cout << "No se encontro la orden.\n";
                inputFile.close();
                return;
                break;
            case 2:
                cout << "Ingresar el codigo del cliente: ";
                getline(cin, id, '\n');
                indexes = invIndex.lookUp(id);
                if(indexes.empty()){
                    cout << "No se encontraron ordenes para el cliente.\n";
                    return;
                }
                for(int index : indexes){
                    inputFile.seekg(index, ios::beg);
                    inputFile >> orden;
                    cout << orden.toString();
                }
                break;
            default:
                cout<< "Opción incorrecta\n";
            }
            
        }
        void modificar() { // Solamente se puede editar cantidad, fecha y código porque las demás propiedades son claves foráneas 
            ofstream outputFile("backups/aux.txt", ios::out);
            ifstream inputFile("backups/orders.txt", ios::in);
            if(!inputFile.good()){
                cout << "Archivo inexistente\n";
            }
            Factura orden;
            bool found = false;
            int option = -1;
            string id;
            cout << "Ingresar el codigo de la orden: ";
            getline(cin, id, '\n');
            while(inputFile.peek() != -1){ // TODO checar si sirve o cambiar por peek()
                inputFile >> orden;
                if(orden.getOrderId() == id){
                    found = true;
                    cout << "\nProducto encontrado:\n" << orden.toString()<<endl;
                    while(option != 0){
                        cout << "1. Codigo\n2. Cantidad\n3. Fecha\n0. Salir\n";
                        cout << "Seleccione elemento para editar: ";
                        cin >> option;
                        cin.ignore();
                        switch(option) {
                            case 1:
                                cout<<"Nuevo codigo de orden: ";
                                getline(cin, orden.facturaId, '\n');
                                break;
                            case 2:
                                cout<<"Nueva cantidad : ";
                                getline(cin, orden.productQuantity, '\n');
                                break;
                            case 3: // Cada atributo de fecha
                                do{
                                    cout << "Editar:\n1. Dia\n2. Mes\n3. Anio\n4. Salir\n";
                                    cin >> option;
                                    cin.ignore();

                                    switch (option){
                                        case 1:
                                            do{
                                                cout << "Ingresar dia: ";
                                                cin >> orden.fecha.day;
                                            }while(orden.fecha.getDay() < 1 || orden.fecha.getDay() > 31);
                                        break;
                                        case 2:
                                            do{
                                                cout << "Ingresar mes: ";
                                                cin >> orden.fecha.month;
                                            }while(orden.fecha.getMonth() < 1 || orden.fecha.getMonth() > 12);
                                        break;
                                        case 3:
                                            do{
                                                cout << "Ingresar anio: ";
                                                cin >> orden.fecha.year;
                                            }while(orden.fecha.getYear() < 1900 || orden.fecha.getYear() > 9999);
                                        break;
                                        case 4: // Salir
                                            break;
                                        default:
                                            cout<<"Opción incorrecta"<<endl;
                                    }
                                } while (option != 4);
                                break;
                            case 0:
                                break;
                            default:
                                cout<<"Opción incorrecta"<<endl;
                        }
                    }
                }
                outputFile << orden;
            }
            if(!found){
                cout << "Producto no encontrado\n";
            } else {
                cout << "Producto modificado exitosamente\n";
            }
            inputFile.close();
            outputFile.close();
            remove("backups/orders.txt");
            rename("backups/aux.txt", "backups/orders.txt");
        }
        void eliminar() {
            ofstream outputFile("backups/aux.txt", ios::out);
            ifstream inputFile("backups/orders.txt", ios::in);
            if(!inputFile.good()){
                cout << "Archivo inexistente\n";
            }
            Factura orden;
            string id;
            bool found = false;
            cout << "Ingresar el codigo de la orden a eliminar: ";
            getline(cin, id, '\n');
            while(inputFile.peek() != -1){
                inputFile >> orden;
                if(orden.getOrderId() == id){
                    invIndex.updateIndexFromDeletion(stoi(orden.getOrderId()));
                    found = true;
                } else {
                    outputFile << orden; 
                }
            }
            if(!found){
                cout << "No se encontró la orden" << endl; 
            }else{
                cout << "Orden eliminada" << endl; 
            }
            inputFile.close();
            outputFile.close();
            remove("backups/orders.txt");
            rename("backups/aux.txt", "backups/orders.txt");
        }

        void loadIndex(){
            ifstream inputFile("backups/invIndex.txt");
            if(!inputFile.good()){
                ofstream ofs("backups/invIndex.txt");
                ofs.close();
            }
            inputFile >> invIndex;
            inputFile.close();
        }
        [[nodiscard]]
        string toString() {
            return "Codigo de orden: " + this->facturaId + "\nCliente: " + this->getClienteId() + "\nProducto: " + this->getProductoId() + "\nFecha: " + fecha.toStringDisplay() + "\nCantidad: " + productQuantity + '\n';
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
            if (!isalnum(c)) {
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
        friend ifstream &operator >> (ifstream &ifs, Factura &orden){
            char customerId[11] = {0};
            char productId[11] = {0};
            char date[9] = {0};
            char orderId[11] = {0};
            char productQuantity[4] = {0};
            string stringifiedField;

            ifs.read(orderId,10);
            ifs.read(customerId,10);
            ifs.read(productId,10);
            ifs.read(date,8);
            ifs.read(productQuantity,3);

            stringifiedField = string(orderId);
            orden.setOrderId(orden.removeBlankSpaces(stringifiedField));

            stringifiedField = string(customerId);
            orden.setClienteId(orden.removeBlankSpaces(stringifiedField));
            
            stringifiedField = string(productId);
            orden.setProductoId(orden.removeBlankSpaces(stringifiedField));

            stringifiedField = string(date);
            orden.fecha.setDay(stoi(stringifiedField.substr(0,2)));
            orden.fecha.setMonth(stoi(stringifiedField.substr(2,2)));
            orden.fecha.setYear(stoi(stringifiedField.substr(4,4)));

            stringifiedField = string(productQuantity);
            orden.setQuantity(orden.removeBlankSpaces(stringifiedField));

            return ifs;
        }
        friend ofstream &operator << (ofstream &ofs, Factura &orden){
            ofs << orden.rightPad(orden.getOrderId(),10) << orden.rightPad(orden.getClienteId(), 10) << orden.rightPad(orden.getProductoId(), 10) << orden.fecha.toStringToFile() << orden.rightPad(orden.getQuantity(),3);
            return ofs;
        }

};
int main(){
    Factura oc;
    int option = 0;
    cout<<"Gestion de Productos\n";
    cout<<"====================\n";
    while (option != 6) {
        cout<<"\nOpciones:\n";
        cout<<"1. Agregar nueva factura\n";
        cout<<"2. Imprimir todas las facturas\n";
        cout<<"3. Buscar factura\n";
        cout<<"4. Modificar factura\n";
        cout<<"5. Eliminar factura\n";
        cout<<"6. Salir\n";
        cout<<"Seleccione una opcion: ";
        cin>>option;
        cin.ignore();
        switch (option) {
            case 1:
                oc.insertar();
                break;
            case 2:
                oc.imprimir();
                break;
            case 3:
                oc.buscar();
                break;
            case 4:
                oc.modificar();
                break;
            case 5:
                oc.eliminar();
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
