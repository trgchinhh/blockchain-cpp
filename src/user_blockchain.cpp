#include "lib//user_blockchain.hpp"

void UserBlockchain::save_all_user_keys() {
    RSAHelper::generate_and_save_keys(names, path_publickeys, path_privatekeys);
}

void UserBlockchain::check_and_initialize_keys() {
    if (!fs::exists(path_data)) {
        cout << "[Hệ thống] Không tìm thấy thư mục data. Đang tiến hành tạo thư mục " << path_data << endl;
        fs::create_directories(path_data);
    }

    auto ensure_json_file_exists = [](const string& file_path) {
        if (!fs::exists(file_path) || fs::file_size(file_path) == 0) {
            ofstream create_file(file_path);
            create_file << "{}"; 
            create_file.close();
        }
    };

    ensure_json_file_exists(path_publickeys);
    ensure_json_file_exists(path_privatekeys);

    bool need_regenerate = false;
    ifstream publickeys_check(path_publickeys);
    ifstream privatekeys_check(path_privatekeys);
    if (!publickeys_check.good() || !privatekeys_check.good()) {
        need_regenerate = true;
    } else {
        try {
            json public_json, private_json;
            publickeys_check >> public_json;
            privatekeys_check >> private_json;
            for (const string& name : names) {
                if (!public_json.contains(name) || !public_json[name].contains("public_key")) {
                    need_regenerate = true;
                    break;
                }
                if (!private_json.contains(name) || !private_json[name].contains("private_key")) {
                    need_regenerate = true;
                    break;
                }
            }
        } catch (...) {
            need_regenerate = true;
        }
    }

    if (publickeys_check.is_open()) publickeys_check.close();
    if (privatekeys_check.is_open()) privatekeys_check.close();
    if (need_regenerate) {
        cout << "[Hệ thống] Khóa người dùng bị thiếu hoặc không đầy đủ. Đang khởi tạo lại cặp khóa mới" << endl;
        save_all_user_keys();
        cout << "[Hệ thống] Đã khởi tạo thành công cặp khóa mới tại thư mục " << path_data << endl;
    } else {
        cout << "[Hệ thống] Kiểm tra tính toàn vẹn hoàn tất. Tất cả khóa người dùng đều hợp lệ" << endl;
    }
}
