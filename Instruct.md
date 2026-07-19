//  ____  _             _        _           _       
// |  _ \| |          | |      | |         (_)      
// | |_) | | ___   ___| | _____| |__   __ _ _ _ __  
// |  _ <| |/ _ \ / __| |/ / __| '_ \ / _` | | '_ \    Demo blockchain C++ (Nâng cấp RSA)
// | |_) | | (_) | (__|   < (__| | | | (_| | | | | |   Thư viện: Crypto++ 
// |____/|_|\___/ \___|_|\_\___|_| |_|\__,_|_|_| |_|   Tác giả: Nguyễn Trường Chinh
// 
// Bản quyền: MIT LICENSE 2026

/* DOC
────────────────────────────────────────────────────────────────────────────────────────────
Hoàn thành ngày 01/07/2026                                  
Demo đào block và thêm block vào blockchain kết hợp chữ ký số RSA

Tìm hiểu hash sha256 là gì ?
  - Hash là hàm băm dữ liệu với đầu vào con người có thể đọc hiểu 
    nhưng đầu ra là 1 chuỗi được mã hóa dài 256(bit) ~ 64 ký tự   
  - Mã băm không thể giải mã (hàm 1 chiều)
  - Chỉ cần thay đổi nhỏ ở input -> output thay đổi hoàn toàn
  - Hiệu ứng thác đổ (Avalanche Effect): Thay đổi dù chỉ 1 dấu chấm, 
    toàn bộ chuỗi hash đầu ra sẽ biến đổi hoàn toàn không theo quy luật.
  - Phân biệt trong code: MD5 dùng để băm Transaction Code (nhanh, ngắn gọn),
    còn SHA256 dùng để băm toàn bộ Block bảo vệ chuỗi liên kết.

Cơ chế chữ ký số (RSA Signature)
  - Mỗi User tham gia mạng lưới sở hữu một cặp khóa: Private và Public Key
  - Người gửi ký giao dịch bằng Private Key -> Tạo ra Signature dạng chuỗi
  - Toàn bộ mạng lưới sử dụng Public Key để kiểm tra (Verify) giao dịch đó
  - Đảm bảo tính bảo mật, chống gian lận và mạo danh ví người khác
  - Chống chối bỏ (Non-repudiation): Khi một giao dịch đã được ký bằng 
    Private Key hợp lệ, người gửi không thể phủ nhận giao dịch đó do chính họ tạo ra.
  - Mã hóa Hex: Chữ ký nhị phân thô (Raw bytes) sinh ra từ bộ ký sẽ được mã hóa
    sang dạng chuỗi ký tự Hex nhằm truyền tải và lưu trữ an toàn trong tệp JSON.

Cách hoạt động
  - Khi 1 block yêu cầu vào blockchain, hệ thống sẽ:
    - Gom dữ liệu (transactions) đã được xác thực chữ ký thành công
    - Lấy hash của block trước (previous hash) để liên kết chuỗi
    - Thêm timestamp (thời gian khởi tạo khối)
    - Thêm nonce (số thử bắt đầu từ 0)

  - Validator (hoặc miner) sẽ:
    - Thử nhiều giá trị nonce khác nhau
    - Mỗi lần thử sẽ tính lại hash của block
    - Mục tiêu: tìm được hash thỏa điều kiện (ví dụ: bắt đầu bằng "0000")
    -> Quá trình này gọi là Proof of Work (PoW)

Ví dụ:
  hash = SHA256(block_id + data + previous_hash + time + nonce)

  Validator sẽ lặp:
    nonce = 0,1,2,3,... cho đến khi:
    hash thỏa điều kiện độ khó (difficulty)
    Ví dụ nếu difficulty = 4
      -> khi hash ra a6957asd.... (hash chưa thỏa)
      -> Khi hash ra 000089a5.... (hash thỏa) 

Độ khó (difficulty):
  - Quy định số lượng số 0 ở đầu hash
  - Difficulty càng cao -> càng khó tìm -> càng tốn thời gian
  - Tính bất đối xứng: Việc tìm ra Nonce hợp lệ cực kỳ khó và tốn tài nguyên phần cứng,
    nhưng khi đã tìm ra, các node khác chỉ mất một phép toán để kiểm tra lại (Verify nhanh).

Khi tìm được hash hợp lệ:
  - Block được "mine" thành công
  - Được thêm vào blockchain
  - Validator nhận reward (phần thưởng)
  -> Quá trình này thường được gọi là đào coin

Cấu trúc Genesis Block (Khối nguyên thủy)
  - Là khối đầu tiên (Index = 0) đặt nền móng cho toàn bộ Blockchain.
  - Không có khối đứng trước, vì vậy Previous Hash được gán mặc định chuỗi 64 ký tự '0'.
  - Chứa thông điệp khởi thủy của nhà sáng lập mạng lưới.

Đặc điểm quan trọng:
  - Không thể đoán trước hash
  - Chỉ có thể brute-force (thử sai liên tục)
  - Dễ verify (kiểm tra nhanh), khó tạo (tính lâu)
  - Tính bất biến (Immutability): Nếu bất kỳ ai cố tình chỉnh sửa dù chỉ 1 byte dữ liệu ở một khối cũ,
    mã hash khối đó thay đổi -> gãy liên kết với khối tiếp theo -> toàn bộ chuỗi phía sau trở nên vô hiệu.

Ứng dụng:
  - Blockchain (Bitcoin, Ethereum, Smart Contract)
  - Bảo mật mật khẩu, chữ ký điện tử doanh nghiệp
  - Kiểm tra toàn vẹn dữ liệu phần mềm, file tải về

Tóm tắt:
  Mining = tìm nonce sao cho hash hợp lệ
  -> đảm bảo tính bảo mật, toàn vẹn dữ liệu và phi tập trung của blockchain
────────────────────────────────────────────────────────────────────────────────────────────
*/
