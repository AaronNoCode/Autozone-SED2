#include <iostream>
#include <fstream>
#include <string>
using namespace std;
    
class IndexTuple {
private:
    int index;
    string id;
public:
    // Constructors
    IndexTuple() = default;
    IndexTuple(int newIndex, const string &newId) : index(newIndex), id(newId) {}

    // Getters and setters
    [[nodiscard]] int getIndex() const { return this->index; }
    [[nodiscard]] string getId() const { return this->id; }
    void setIndex(int newIndex) { this->index = newIndex; }
    void setId(const string &newId) { this->id = newId; }

    friend ofstream &operator<< (ofstream &ofs, const IndexTuple &idx){
        ofs << idx.index << '*' << idx.id << '#';
        return ofs;
    }
    friend istream &operator>> (istream &is, IndexTuple &idx) {
        string strAux;
        getline(is,strAux, '*');
        idx.index = stoi(strAux);
        getline(is, idx.id, '#');
        return is;
    }
    friend class Servicio;
};
