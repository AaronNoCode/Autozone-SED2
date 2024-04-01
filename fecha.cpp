#include <iostream>
#include <fstream>
#include <string>

using std::string, std::to_string, std::cout, std::ifstream, std::ofstream;

class Date {
    private: 
        int day;
        int month;
        int year;
    public:
        Date(){
            day = 0;
            month = 0;
            year = 0;
        }
        Date (int newDay, int newMonth, int newYear){
            this->day = newDay;
            this->month= newMonth;
            this->year = newYear;
        }

        int getDay() const {
            return day;
        }
        int getMonth() const {
            return month;
        }
        int getYear() const {
            return year;
        }

        void setMonth(int newMes) {
            month = newMes;
        }
        void setDay(int newDia) {
            day = newDia;
        }
        void setYear(int newAnio) {
            year = newAnio;
        }
        string toStringToFile(){
            return leftPad(to_string(day)) + leftPad(to_string(month)) + to_string(year);
        }
        string toStringDisplay(){
            return leftPad(to_string(day)) + '/' + leftPad(to_string(month)) + '/' + to_string(year);
        }
        string leftPad(const string &str) const {
        if(str.length() == 2){
            return str;
        }else{
            return '0' + str;
        }
    }
    friend ifstream &operator >> (ifstream &ifs, Date &date){
        ifs.read((char*)&date, sizeof(date));
        return ifs;
    } 
    friend ofstream &operator << (ofstream &ofs, Date &date){
        ofs << date.toStringToFile();
        return ofs;
    }

    friend class Factura;
    friend class HashTable;
};
