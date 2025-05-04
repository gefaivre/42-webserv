# Webserv - 42 Project

## 🌐 Overview

**Webserv** is a web server implementation written in C++ as part of the curriculum at 42 programming school.  
The project aims to replicate the core functionalities of a real HTTP server like Nginx or Apache, based on the RFC 2616 specification.

It handles multiple clients simultaneously, serves static files, manages different routes and supports basic CGI execution.

## 🚀 Getting Started

### Requirements

- Unix-like system (Linux/macOS)
- C++98 compliant compiler
- make

### Installation & Run


```bash
git clone git@github.com:gefaivre/42-webserv.git
cd 42-webserv
```
Addapt the **root** variable in **config/default.conf**

```bash
make
./webserv config/default.conf
```

## 🛠 Features

- HTTP 1.1 compliant
- Supports multiple simultaneous clients
- GET, POST, DELETE methods
- Configurable via .conf files (like Nginx)
- CGI support
- Error pages & redirection
- Basic load handling with select()

## ⚙️ Configuration

The server is configured through `.conf` files, which define:
- Port and host to bind to
- Root directory
- Error pages
- Location blocks with custom rules (methods allowed, CGI paths, etc.)

Example:

```
server {
  listen 8042;
  server_name domain1.com;
  cgi .php /usr/bin/php-cgi;
  root /home/gefaivre/Documents/42-webserv/www;
  autoindex on;
  accepted_methods       GET POST DELETE;
  location / {
    accepted_methods GET POST DELETE;
  }
}
```

See more in the *configurations* directory
