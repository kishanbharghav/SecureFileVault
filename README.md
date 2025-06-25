## 🔐 Secure File Vault — C++ Encryption Project

### 📌 **Description:**

**Secure File Vault** is a command-line based application written in **C++** that allows users to **securely store, encrypt, and retrieve files** using a simple custom encryption method. It features a clean, web-styled console interface inspired by HTML/CSS layouts for a better user experience.

The project demonstrates core concepts of **file I/O**, **basic encryption (XOR)**, and **directory management**, making it ideal for those exploring cybersecurity fundamentals and systems-level programming.

### 🧰 **Features:**

* 📥 **Encrypt & Store Files:** Upload a file and securely store it in a vault directory with encryption.
* 📤 **Retrieve Files:** Decrypt and recover files when needed into a separate `retrieved/` folder.
* 📄 **Indexing:** Maintain a file map (`vault_index.txt`) linking original filenames to encrypted hashes.
* 🧹 **Clean Console UI:** Mimics a web interface using terminal styling (like HTML/CSS).
* 🗑️ **Delete Files:** Remove encrypted files and update the index accordingly.

### 🚧 **In Progress:**

* 🌐 Building a **Web App frontend** using **HTML and CSS**
* 🔗 Connecting the web frontend to the **C++ backend**
* 🔒 Upgrading encryption from XOR to **AES (Advanced Encryption Standard)** using a secure crypto library

