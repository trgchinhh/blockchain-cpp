//  ____  _             _        _           _       
// |  _ \| |          | |      | |         (_)      
// | |_) | | ___   ___| | _____| |__   __ _ _ _ __  
// |  _ <| |/ _ \ / __| |/ / __| '_ \ / _` | | '_ \    Demo blockchain C++ (Nâng cấp RSA)
// | |_) | | (_) | (__|   < (__| | | | (_| | | | | |   Thư viện: Crypto++ 
// |____/|_|\___/ \___|_|\_\___|_| |_|\__,_|_|_| |_|   Tác giả: Nguyễn Trường Chinh
// 
// Bản quyền: MIT LICENSE 2026

#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <random>
#include <fstream>
#include <thread>
#include <atomic>

// thư viện tóm tắt dùng hash trong openssl gọn như python
// xem hướng dẫn và tải tại: https://github.com/trgchinhh/hashlib-cpp
#include "lib/hashlib.h"

// Thư viện phụ dùng cho json 
#include "lib/json.hpp"

// Thư viện phụ dùng chrono sinh ngày tháng năm 
#include "lib/datetime.h"

// Thư viện Crypto++ cho RSA
// Cài từ Ming64
// Lệnh cài: pacman -S mingw-w64-x86_64-cryptopp
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/pssr.h>

// Thư viện indicator cho progress bar
// Tải thư viện tại: https://github.com/p-ranav/indicators
#include "lib/indicators/progress_bar.hpp"
#include "lib/indicators/cursor_control.hpp"

using namespace std;
using namespace dt;
using namespace hashlib;
using namespace chrono;
using namespace CryptoPP;
using namespace indicators;
using namespace indicators::option;
using json = nlohmann::ordered_json;
