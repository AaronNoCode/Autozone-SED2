/* Inverted index for factures */

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct IndexTuple {
    string secKey;
    vector <int> indexes;
    IndexTuple(): secKey(" "), indexes(){}
    IndexTuple(const string key, const int index){
        secKey = key;
        indexes.push_back(index);
    }
    void print(){
        cout << secKey << ": ";
        for (int index : indexes)
            cout << index << " ";
        cout << endl;
    }
};

class InvertedIndex{
private:
    vector <IndexTuple> invIndex;
public:
    void add(const string secKey, const int index){
        for (IndexTuple &tuple : invIndex){
            if(secKey == tuple.secKey){
                tuple.indexes.push_back(index);
                return;
            }
        }
        IndexTuple tuple(secKey, index);
        invIndex.push_back(tuple);
    }
    void deletion(const int index2Del, const string secKey2Del){ // Should recieve the secKey too to make the deletion easier 
        int currentInnerIndex = 0;
        int currentInvIndex = 0;
        for (IndexTuple &tuple : invIndex) {
            if(tuple.secKey == secKey2Del){
                if(tuple.indexes.size() == 1){
                    invIndex.erase(invIndex.begin() + currentInvIndex);
                    updateIndexFromDeletion(index2Del);
                    return;
                }
                for(int &currentOuterIndex : tuple.indexes){
                    if (currentOuterIndex == index2Del){
                        tuple.indexes.erase(tuple.indexes.begin() + currentInnerIndex);
                        updateIndexFromDeletion(index2Del);
                        return;
                    }
                    currentInnerIndex++;
                }
            }
            currentInvIndex++;
        }
    }
    // Check for empty vector when returning to tickets
    [[nodiscard]]vector <int> lookUp(const string secKey) {
        for (IndexTuple &tuple : invIndex) {
            if(tuple.secKey == secKey) {
                return tuple.indexes;
            }
        }
        vector <int> empty;
        return empty;
    }
    void updateIndexFromDeletion (const int indexUpdated){
        for(IndexTuple &tuple : invIndex){
            for(int &index : tuple.indexes){
                if(index > indexUpdated){
                    index-=41;
                }
            }
        }
    }

    void updateKey(const string oldKey, const string newKey){
        for (IndexTuple &tuple : invIndex){
            if(tuple.secKey == oldKey){
                tuple.secKey = newKey;
                return;
            }
        }
    }

    void print(){
        for (IndexTuple &tuple : invIndex)
            tuple.print();
        cout << endl;
    }

    friend ifstream &operator >> (ifstream &ifs, InvertedIndex &ii){
        string strAux = " ";
        while (ifs.peek() != -1){
            IndexTuple tupleAux;
            getline(ifs, tupleAux.secKey, '?');
            while (getline(ifs, strAux, ' ')){
                tupleAux.indexes.push_back(stoi(strAux));
                if(ifs.peek() == '\n'){
                    ifs.ignore(1);
                    ii.invIndex.push_back(tupleAux);
                    break;
                }
            }
        }
        return ifs;
    }
    friend ofstream &operator << (ofstream &ofs, InvertedIndex &ii){
        for (IndexTuple &item : ii.invIndex){
            ofs << item.secKey << '?';
            for (int index : item.indexes){
                ofs << index << ' ';
            }
            ofs << '\n';
        }
        return ofs;
    }
};


/* int main () {

    InvertedIndex ii;
    InvertedIndex ii2;
    // Make three insertions    
    ii.add("hello", 41);
    ii.add("hello", 205);
    ii.add("world", 82);
    ii.add("world", 123);
    ii.add("world", 246);
    ii.add("world", 287);
    ii.add("notme", 164);
    ii.add("notme", 328);
    ii.deletion(123, "world");
    ii.print();
    vector<int>prop =  ii.lookUp("world");
    for (int i : prop)
        cout << i << " ";
    

    return 0;
} */
