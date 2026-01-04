# Webserver

This is my network programming project written in **C++** for **Linux (Ubuntu)**.  
It serves a few static websites and allows **GET, POST, DELETE** methods and **CGI** requests.

Here are some features of my projects:
- following **RFC** standards;
- transparent and scalable architechture; 
- **CMAKE** build;
- bash script with automated tests and convenient results indication; 
- parsing input configuration file and properly handling: comments, chain of token->lexem->configurations, monitoring syntax and lexic errors;
- GET, POST, DELETE method implemented;
- CGI requests implemented;
- TIMEOUT kill implemented to prevent overloading, abandoning requests and CGI malfanction like infinite loop;
- **SIEGE** passed with 100% availability;
- no memory leaks controlled by Valgrind;
- no FDs are left open controlled by Valgrind;
- **KERNEL SIGNAL** handling: interception and clean exit.

---

### System Requirements

- Linux (Ubuntu)
- gcc
- cmake
- bash
- valgrind

---

### Build

```bash
git clone https://github.com/romolog/C__graphics__3D_maze_2025 webserv;
cd webserv;
```
---

### Run 
```bash
./run_webserv.sh -DNDEBUG=OFF;
```