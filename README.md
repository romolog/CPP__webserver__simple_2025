# Webserver

This is my network programming project written in **C++** for **Linux (Ubuntu)**.  
It serves a few static websites and allows **GET, POST, DELETE** methods and **CGI** requests.

Here are some features of my project:
<pre>
* RFC:				Compliance with relevant RFC standards
* Scalable:			Transparent and scalable architecture
* CMake:			CMake-based build system
* Bash:				Bash script with automated tests and clear result reporting
* Parsing:			Parsing of the input configuration file with proper handling of 
					comments, token → lexeme → configuration chains, 
					and detection of syntax and lexical errors
* Configuration:	The configuration file supports multiple servers with different ports 
					or hostnames, a default index page or directory autoindex, 
					and routing request to target directory. 
* I/O multiplex:	Network programming implementing input&nbsp;/&nbsp;output multiplex based on epoll API
* Sockets:			Sockets status control for removing clients on errors or connection drops
* Methods:			Implementation of the GET, POST, and DELETE methods
* CGI:				CGI request handling
* Chunks:			Chunked transfer encoding requests and responses is supported
* Timeout:			Timeout-based process termination to prevent overloading, 
					abandoned requests, and CGI malfunctions such as infinite loops
* Siege:			Successfully tested with Siege, achieving 100% availability
* Memory control:	No memory leaks, verified with Valgrind
* FDs congrol:		No file descriptors left open, verified with Valgrind
* Kernel signals:	Kernel signals handling with proper interception and clean shutdown
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
cmake -B build && cmake --build build;
```

### Build in debug mode and Run with automated tests
```bash
./run_webserv.sh -DNDEBUG=OFF;
```

### Run with the default configuration
```bash
./webserv;
```

### Run with a custom configuration
```bash
./webserv <path to config file>;
```