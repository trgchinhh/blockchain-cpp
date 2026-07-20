#include "lib//utils.hpp"

const vector<string>names = {
    "Truong Chinh", "Truong An", "Duy Minh", "Duy Quang", "Ngoc Tu",
    "Thanh Phuc", "Manh Hung", "The An", "Thien An", "Gia Bao", 
    "Khanh Dang", "Hai Dang", "Khanh Duy", "Bao Khanh", "Manh Cuong"
};

string time_now() {
    return datetime::now().strftime("%H:%M:%S - %d/%m/%Y");
}