# CodeMedic 🩺

### C++ Code Health & Complexity Analyzer

CodeMedic is a web-based C++ static code analysis tool that evaluates the structural health and complexity of C++ source code.

It analyzes uploaded `.cpp` files without executing them and provides code metrics, complexity estimation, OOP analysis, warnings, a Code Health Score, and Before vs After comparison.

---

## 🚀 Features

- 📊 **Code Health Score**
- ⏱️ **Estimated Computational Complexity**
- ⚠️ **Code Warnings & Diagnostics**
- 📈 **Source Code Statistics**
- 🔄 **Before vs After Code Comparison**
- 🧩 **Object-Oriented Programming Analysis**
- 🔐 **Encapsulation Detection**
- 🧬 **Inheritance Detection**
- 🎭 **Polymorphism Detection**
- 🔁 **Loop & Nested Loop Detection**
- 🔀 **Conditional Statement Detection**
- 📝 **Function & Class Detection**
- 🌐 **Web-based Dashboard**
- ⚡ **C++ Backend API**

---

## 🛠️ Technologies Used

### Backend
- C++17
- Object-Oriented Programming
- STL
- File Handling
- String Processing
- Pattern-based Static Analysis
- Windows Winsock API

### Frontend
- HTML5
- CSS3
- JavaScript
- Fetch API
- Responsive Dashboard UI

### Development Tools
- Visual Studio Code
- MinGW / g++
- Git
- GitHub

---

## 🏗️ Project Architecture

```text
CODEMEDIC
│
├── backend
│   ├── include
│   │   ├── CodeAnalyzer.h
│   │   └── CodeComparator.h
│   │
│   ├── src
│   │   ├── CodeAnalyzer.cpp
│   │   ├── CodeComparator.cpp
│   │   └── main.cpp
│   │
│   └── server.cpp
│
├── frontend
│   ├── index.html
│   ├── style.css
│   └── script.js
│
├── test_files
│   ├── good_code.cpp
│   ├── complex_code.cpp
│   ├── problematic_code.cpp
│   ├── random_test.cpp
│   ├── nested_test.cpp
│   ├── conditions_test.cpp
│   └── oop_test.cpp
│
├── screenshots
├── reports
├── .gitignore
└── README.md