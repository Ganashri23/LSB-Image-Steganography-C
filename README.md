# LSB Image Steganography in C

## 📌 Project Description

LSB Image Steganography is a C-based project that hides secret data inside a BMP image using the Least Significant Bit (LSB) technique. The hidden data can later be extracted from the stego image using the decoding process.

## 🎯 Objective

The main objective of this project is to understand how data can be securely hidden inside an image using bitwise operations and file handling in C.

## ✨ Features

- Hide a secret file inside a BMP image
- Extract the hidden file from the stego image
- Check image capacity before encoding
- Store and retrieve secret file extension
- Store and retrieve secret file size
- Preserve the BMP image format
- Command-line based execution

## 🛠️ Technologies Used

- **Language:** C
- **File Format:** BMP
- **Concepts Used:**
  - File Handling
  - Bitwise Operations
  - Pointers
  - Structures
  - Functions
  - Command Line Arguments
  - Dynamic Memory
  - Header Files

## 📂 Project Structure

```text
LSB-Image-Steganography-C/
│
├── common.h
├── types.h
│
├── encode.c
├── encode.h
│
├── decode.c
├── decode.h
│
├── test_encode.c
│
├── beautiful.bmp
├── stego.bmp
└── secret.txt
