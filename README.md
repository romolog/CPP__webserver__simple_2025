# Webserver

This is my network programming project written in **C++** for **Linux (Ubuntu)**.  
It serves a few static websites and allows **GET, POST, DELETE** methods and **CGI** requests.

Here are some features of my project:
<pre>
* RFC:		Compliance with relevant RFC standards
* Scalable:	Transparent and scalable architecture
* CMake:	CMake-based build system
* Bash:		Bash scripts with automated tests and clear result reporting
* Parsing:	Parsing of the input configuration file with proper handling of comments, token → lexeme → configuration chains, and detection of syntax and lexical errors
* Methods:	Implementation of the GET, POST, and DELETE methods
* CGI:		CGI request handling
* Timeout:	Timeout-based process termination to prevent overloading, abandoned requests, and CGI malfunctions such as infinite loops
* Siege:	Successfully tested with Siege, achieving 100% availability
* Memory:	No memory leaks, verified with Valgrind
* FDs:		No file descriptors left open, verified with Valgrind
* Signals:	Kernel signals handling with proper interception and clean shutdown
</pre>


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