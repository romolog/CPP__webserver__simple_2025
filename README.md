# Webserver

This is my network programming project written in **C++** for **Linux (Ubuntu)**.  
It serves a few **static websites** and allows **GET POST DELETE and CGI**.

Though I have properly tested and developed more functions than those mentioned above, I have not yet added them to this showcase project. Once I integrate their separate test blocks into a fully automated, unified test script, I will include them accordingly.

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