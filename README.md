🌐 First-Communication-With-A-Server
This was my third school project for a computer architecture class. The goal was to build a program in C that can connect to a server, pass messages back and forth, and complete different tasks sent over by the server.

Language: C

OS Environment: Windows (using Winsock)

✨ Features
Socket Connection: Hooks up to a remote server using low level C networking.

Live Chat / Messaging: Send custom messages straight from the terminal instead of hardcoding them into the code.

Task Solving: Automatically handles challenges thrown at it by the server (like a XOR cipher).

🛠️ The Process
Step 1: Figuring out Networks First, I had to learn how networking actually works in Windows. I spent time figuring out libraries like <winsock2.h> and <ws2tcpip.h> just to open a clean connection and receive basic data.

Step 2: Making it Interactive The next big hurdle was getting the terminal to play nice. Instead of pre-writing messages in the code, I added the ability to type a message into the console and send it off in the terminal itself.

Step 3: Breaking Ciphers Once the connection was solid, I worked on getting the program to solve tasks sent by the server. The main highlight here was writing a decoder to crack a XOR cipher.

🧠 What I've Learned
This project was a huge learning curve for me. It forced me to think a lot more logically and gave me a much better grasp on how computers talk to each other.

📡 Network Basics
I learned how sockets actually work under the hood. I got hands-on experience setting up Windows sockets (WSAStartup), opening connections, and properly closing them so I didn't leave memory leaking.

💾 Buffer & Text Management
Programming this in C meant I couldn't cut corners with strings. I learned how to carefully handle data buffers, read user input from the terminal, and make sure incoming server data didn't overflow the program.

🔒 Bitwise Operations & Logic
Implementing the XOR cipher gave me a practical reason to use bitwise operators.

🔍 Debugging Hacks
Network code rarely works on the first try. I spent a lot of time learning how to look up specific network error codes to figure out exactly why a connection dropped or why data got corrupted.

💻 How to Run It (Windows)
Because this uses the Windows socket library, you need to make sure you link lws2_32 when compiling it with GCC:
Bash
gcc main.c -o client -lws2_32
./client

💭 Possible Improvements
Different logic for handling tasks (Making sure it doesn't break when the user sends a wrong message).
Better UI instead of just the terminal.
Making the sending and recieveing into its own functions.

I've also included a server.c file which would simulate a server like the one used in class.
