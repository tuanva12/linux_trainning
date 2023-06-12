#  linux_trainning
## Token
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
#

## Refer
https://github.com/Johannes4Linux/Linux_Driver_Tutorial.git
#

## unit1: Hello word file
## unit2: Build image for beaglebone black
## unit3: File system
- helofile:<br>
    Đọc file in ra màn hình.
- copyfile:<br>
   Chương trình copy file.

## unit4: Process
- Homework: <br>
    Viết 2 chương trình cha và con.<br>
    Chương trình cha gọi con với tham số truyền vào là đường dẫn đến file.<br>
    Chương trình con ghi hello world vào file và trả về trạng thái kết quả cho cha.<br>
    Cha phải biết kết quả thực hiện của chương trình con.<br>

## unit5: Multithread
- simple_thread: Vi du khởi tạo multi thread.
- mutex: Su dung mutex trong multi thread
- exam_1: Tạo 3 thread, tăng giá trị 1 biến chung và ghi giá trị vào 1 trong 2 file output, sử dụng semafore.

## unit6: shared memory
1. hello
- gcc -o read read.c -lrt
- gcc -o write write.c -lrt
2. homework: Tạo giả lập server và client, server luôn lắng nghe từ memory shared, client nhận dữ liệu từ bà phím nhập vào tên và tuổi, khi nhập xong gửi tín hiệu tới sever để đọc dữ liệu, ghi vào log file.
- gcc -o server server.c -lrt
- gcc -o client client.c -lrt

## unit7: hello kernel

## unit8: character device.