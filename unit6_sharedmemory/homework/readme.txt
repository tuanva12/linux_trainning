Mô tả bài toán:
    Có 2 chương trình client và server chia sẻ với nhau 1 vùng nhớ
    Client sẽ nhận dữ liệu mà người dùng nhập vào từ bàn phím, chứa thông tin của 1 sinh viên. Thông tin bao gồm họ tên, ngay sinh, điểm tốt nghiệp
    Mỗi khi người dùng nhập xong dữ liệu của 1 sinh viên, client sẽ ghi bản tin đó vào shared memory, sau đó gửi signal báo hiệu cho server thông qua API signal() của Linux
    Server sau khi nhận được signal sẽ đọc bản tin từ shared memory và lưu vào file
    Việc đồng bộ quá trình ghi, đọc vào shared memory để tránh xung đột thì cần áp dụng kỹ thuật named semaphore của Linux.



BUILD:
gcc -o server server.c -lrt -pthread
gcc -o client client.c -lrt -pthread