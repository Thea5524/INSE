# Task 1: Public-key based authentication 

**Question 1**: 
Implement public-key based authentication step-by-step with openssl according the following scheme.
![alt text](./image-1.png)

**Answer 1**:

1. Access to the forder with lab and creat the new folder

![image](./lab2/img/cd.png)

2. Creat the file with the simple message

![image](./lab2/img/echo.png)

3. Make the private and public key for server

![image](./lab2/img/pri,pu_key.png)
 
4. To create a user-defined network and add the containers to it to use the `docker network create`

![image](/lab2/img/docker%20creat.png)

5. Transfer the public key to the server with command `nc -l -p 12345 > public_key.pem`

![image](./lab2/img/tran_pu.png)

6. Server encrypt the message with the public key was received and use the command `cat` to show this file

![image](./lab2/img/server%20en.png)

7. Server transfer this file for client `nc -l -p 12345 > text-enc.txt`

8. The client decryptes this file by private key. Run the command `openssl rsa -in private_key.pem -out de_private.pem` to decrypt the private key

![image](./lab2/img/de_pri.png)





# Task 2: Encrypting large message 

Create a text file at least 56 bytes.
**Question 1**:
Encrypt the file with aes-256 cipher in CFB and OFB modes. How do you evaluate both cipher as far as error propagation and adjacent plaintext blocks are concerned. 
**Answer 1**:
1. Creat file `plain.txt` and check in this folder

![image](./lab2/img/plain.png)

2. Encrypt the file with CFB and check the result
![image](./lab2/img/cfb.png)

3. Encrypt the file with OFB and check the result
![image](./lab2/img/ofb.png)

> How do you evaluate both cipher as far as error propagation and adjacent plaintext blocks are concerned. 
- With CFB, the ciphertext from the previous block is fed into the block cipher for encryption, and the output of the encryption is XORed with the plaintext to generate the actual ciphertext.

![image](./lab2/img/cfb-explain.png)
- With OFB, similar to CFB, except that the data before (while in CFB, it should be "after") the XOR operation is fed into the next block.
![image](./lab2/img/ofb-explain.png)

**Question 2**:
Modify the 8th byte of encrypted file in both modes (this emulates corrupted ciphertext).
Decrypt corrupted file, watch the result and give your comment on Chaining dependencies and Error propagation criteria.

**Answer 2**:
1. Modify the 8th byte of encrypted file in CFB modes 

![image](./lab2/img/bit-cfb.png)

2. Decrypt the corrupted file `plain-cfb.txt`
![image](./lab2/img/de-cfb.png)
and then check this file 
![image](./lab2/img/cat%20de-cfb.png)

3. Modify the 8th byte of encrypted file in OFB modes
![image](./lab2/img/bit-ofb.png)

4. Decrypt the corrupted file `plain-ofb.txt`
![image](./lab2/img/de-ofb.png)
and then check this file 
![image](./lab2/img/cat%20de-ofb.png)

>Chaining dependencies and Error propagation criteria.

- CFB:  The mode achieves an interesting “self-synchronization” property; insertion or deletion of any number of s-bit characters into the ciphertext only temporarily disrupts correct decryption, error propagation.
- OFB: Absence of error propagation. Do not change the message when change the bit in encrypted file.
