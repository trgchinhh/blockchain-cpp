## Demo blockchain C++ (Nâng cấp RSA)

Demo mô phỏng validator tham gia vào mạng lưới blockchain thực hiện xác thực chữ ký số giao dịch, tính toán mã hash hợp lệ (đào) cho block bằng thuật toán Proof of Work (PoW) và thêm block vào chuỗi.

![Blockchain demo](docs/blockchain-demo.gif)

## Các tính năng nâng cấp nổi bật

* **Bảo mật ví điện tử (RSA Keypair):** Tự động sinh ngẫu nhiên cặp khóa Private Key (Bảo mật) và Public Key (Công khai) độ dài 1024-bit hoặc 2048-bit tùy chỉnh cho toàn bộ User, quản lý lưu trữ an toàn dưới dạng mã hóa Base64 trong tệp JSON
* **Chữ ký số giao dịch (RSA Signature):** Người gửi bắt buộc phải ký vào gói dữ liệu giao dịch bằng Private Key. Hệ thống sử dụng Public Key để verify, chống gian lận dữ liệu và mạo danh ví người khác
* **Mã hóa bất đối xứng:** Kết hợp bộ lọc mã hóa Crypto++ (`HexEncoder`, `Base64Decoder`) để đồng bộ hóa luồng dữ liệu nhị phân thô sang dạng chuỗi lưu trữ
* **Cơ chế Proof of Work (PoW):** Miner liên tục đào để tìm chỉ số `Nonce` để hàm băm SHA256 của toàn khối thỏa mãn độ khó (`Difficulty`) cấu hình trước.

---

## Yêu cầu hệ thống & Thư viện

Để biên dịch và chạy dự án này, hệ thống của bạn cần cài đặt các thư viện sau:

1.  **Crypto++ (cryptopp):** Thư viện mã hóa chuyên sâu phục vụ thuật toán ký RSA
2.  **OpenSSL (libssl & libcrypto):** Phục vụ cho các hàm băm cơ bản
3.  **Hashlib-cpp:** Thư viện tóm tắt tự viết giúp gọi hash tiện lợi như Python. Tải và xem hướng dẫn tại: [github.com/trgchinhh/hashlib-cpp](https://github.com/trgchinhh/hashlib-cpp)
4.  **JSON:** Thư viện phụ xử lý cấu trúc JSON (`lib/json.hpp`)

### Hướng dẫn cài nhanh Crypto++ trên Windows (MinGW64 / MSYS2)

Mở terminal MSYS2 (UCRT64 hoặc MINGW64) trong bộ trình dịch MSYS2 và chạy lệnh:
```bash
# Đối với môi trường UCRT64 
pacman -S mingw-w64-ucrt-x86_64-cryptopp

# Đối với môi trường MINGW64 
pacman -S mingw-w64-x86_64-cryptopp
```
