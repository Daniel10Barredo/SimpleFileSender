# Simple File Sender

Simple File Sender for linux and windows for OSCP preparation.

(Just sends a binary file through the socket to the server that receives the content and saves it)

Usage

```bash
#Receiver (Server - just linux)
./receiver <fileOut>
#Sender (Client - windows and linux)
./sender <HostIP> <File>
.\sender.exe <HostIP> <File>
```

---
### For Server

Compilation
```
gcc -static receiver.c -o receiver64
gcc -static -m32 receiver.c -o receiver32
```

---
### For Client

Compilation for linux
```
gcc -static sender_lin.c -o sender64
gcc -static -m32 sender_lin.c -o sender32
```

Compilation for windows
```
x86_64-w64-mingw32-gcc -static sender_win.c -o sender64.exe -lws2_32
i686-w64-mingw32-gcc sender_win.c -o sender32.exe -lws2_32
```
