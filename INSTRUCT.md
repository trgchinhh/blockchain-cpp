# Blockchain Demo C++

> Demo Blockchain C++ sử dụng **Crypto++**, kết hợp **SHA-256**, **Proof of Work (PoW)** và **RSA Digital Signature**.

- **Ngôn ngữ:** C++
- **Thư viện:** Crypto++
- **Hoàn thành:** 01/07/2026
- **Tác giả:** Nguyễn Trường Chinh (NTC++)
- **Giấy phép:** MIT License

---

# Blockchain hoạt động như thế nào?

## Hash SHA-256 là gì?

SHA-256 là thuật toán băm (Hash Function) biến dữ liệu đầu vào thành chuỗi băm dài **256 bit (64 ký tự Hex)**.

### Đặc điểm

- Hàm băm một chiều, **không thể giải mã ngược**.
- Chỉ cần thay đổi một ký tự ở dữ liệu đầu vào thì kết quả băm sẽ thay đổi hoàn toàn.
- Có **hiệu ứng thác đổ (Avalanche Effect)**.
- Trong demo:
  - **MD5** dùng để tạo **Transaction Code** (nhanh, ngắn gọn).
  - **SHA-256** dùng để băm toàn bộ Block nhằm bảo vệ chuỗi Blockchain.

---

## Chữ ký số RSA (RSA Digital Signature)

Mỗi người dùng tham gia mạng sẽ sở hữu:

- Private Key
- Public Key

Quy trình hoạt động:

1. Người gửi ký giao dịch bằng **Private Key**.
2. Sinh ra một **Digital Signature**.
3. Các node khác sử dụng **Public Key** để xác minh chữ ký.

### Lợi ích

- Xác thực người gửi.
- Chống giả mạo giao dịch.
- Chống chối bỏ (Non-repudiation).
- Bảo vệ tính toàn vẹn của dữ liệu.

### Lưu trữ

Chữ ký RSA sinh ra ở dạng **Raw Bytes** nên sẽ được mã hóa sang **Hex** để dễ lưu trong JSON hoặc truyền qua mạng.

---

# Quy trình tạo Block

Khi một Block được tạo, hệ thống sẽ chuẩn bị:

- Danh sách Transaction đã xác thực
- Previous Hash
- Timestamp
- Nonce

Ví dụ dữ liệu được băm:

```text
SHA256(
    block_id +
    transactions +
    previous_hash +
    timestamp +
    nonce
)
```

---

# Proof of Work (PoW)

Validator (hoặc Miner) sẽ liên tục thử các giá trị Nonce:

```text
nonce = 0
nonce = 1
nonce = 2
...
```

Mỗi lần thử:

```text
Hash = SHA256(Block Data)
```

Cho tới khi Hash thỏa điều kiện độ khó.

Ví dụ:

```text
Difficulty = 4

Hash:
a6957asd...
=> Không hợp lệ

000089a5...
=> Hợp lệ
```

---

# Difficulty

Difficulty quy định số lượng số **0** ở đầu Hash.

Ví dụ:

```text
Difficulty = 3

000abfd8...
```

Difficulty càng cao:

- Càng nhiều phép tính.
- Càng mất nhiều thời gian đào.
- Càng tốn tài nguyên CPU.

---

# Sau khi Mine thành công

Khi tìm được Nonce phù hợp:

- Block được Mine thành công.
- Block được thêm vào Blockchain.
- Validator nhận Reward.

Quá trình này thường được gọi là **đào coin (Mining)**.

---

# Genesis Block

Genesis Block là Block đầu tiên của Blockchain.

Đặc điểm:

- Index = 0
- Previous Hash = 64 ký tự '0'
- Chứa thông điệp khởi tạo của Blockchain.

Ví dụ:

```text
Previous Hash

0000000000000000000000000000000000000000000000000000000000000000
```

---

# Tính chất của Blockchain

## Không thể đoán trước Hash

Không có công thức để dự đoán Hash.

Chỉ có thể:

- Brute Force
- Thử liên tục các giá trị Nonce

---

## Khó tạo nhưng dễ kiểm tra

Đây là đặc điểm quan trọng của PoW.

- Tạo Block hợp lệ rất tốn thời gian.
- Kiểm tra Hash chỉ cần một phép tính SHA-256.

---

## Tính bất biến (Immutability)

Nếu chỉ sửa **1 byte** trong một Block cũ:

- Hash Block đó thay đổi.
- Previous Hash của Block kế tiếp không còn đúng.
- Toàn bộ Blockchain phía sau bị vô hiệu.

---

# Ứng dụng

Blockchain được sử dụng trong:

- Bitcoin
- Ethereum
- Smart Contract
- Chữ ký điện tử
- Bảo mật mật khẩu
- Kiểm tra toàn vẹn phần mềm
- Kiểm tra file tải về

---

# Tóm tắt

```text
 Transaction
      │
      ▼
 RSA Signature
      │
      ▼
    Verify
      │
      ▼
 Create Block
      │
      ▼
 Proof of Work
      │
      ▼
 Mine Success
      │
      ▼
Add Block to Blockchain
```

**Mining = Tìm Nonce để Hash thỏa điều kiện Difficulty**

→ Đảm bảo tính **bảo mật**, **toàn vẹn dữ liệu** và **phi tập trung** của Blockchain.
