# TinyML C Tensor & Quantization Engine 🚀

A memory-efficient, dynamic Tensor data structure written in C, specifically designed for resource-constrained embedded systems (such as Arduino and ESP32). 

This project was developed as part of the **Data Structures and Algorithms** coursework to demonstrate low-level memory management, pointer manipulation, and model optimization techniques for TinyML applications.

## 📌 Project Overview
Deploying Artificial Intelligence models on microcontrollers requires strict memory management. Standard 32-bit float arrays quickly exhaust the available RAM on these devices. This project implements a custom `Tensor` structure from scratch in C, featuring **Quantization** to compress 32-bit Float weights into 8-bit Integers, reducing memory footprint by 75% while maintaining proportional accuracy.

## ✨ Key Technical Features
* **Contiguous Memory Management:** Matrices are flattened into 1D arrays using the `rows * cols` formula. This approach ensures contiguous memory allocation, which is highly optimized for CPU caching—mirroring standard practices in AI frameworks like TensorFlow Lite.
* **Memory Optimization via `Union`:** Instead of defining separate pointers for different data types within a struct, a `union` is utilized. The pointers for `float*` and `int8_t*` share the exact same physical memory address, eliminating memory waste.
* **Dynamic Allocation & Safety:** Uses `malloc()` to dynamically allocate memory based on tensor dimensions and data type. Memory leaks are strictly prevented by properly utilizing `free()` to release memory back to the system.
* **Type Casting & Quantization:** Iterates through the original Float32 AI model weights, scales them based on the maximum absolute value, and explicitly casts them to Int8 (`int8_t`), effectively compressing the model.

## 🛠️ Development & Agentic Coding
The architecture of this code was designed using an **Agentic Coding** approach. AI models (Gemini) were utilized as coding co-pilots within the IDE to assist in debugging memory leaks, optimizing the union architecture, and verifying pointer arithmetic during the development process.

## 🚀 How to Run

1. Clone the repository:
   `git clone [https://github.com/yourusername/your-repo-name.git](https://github.com/yourusername/your-repo-name.git)`
2. Navigate to the directory and compile the C file using GCC:
   `gcc tensor_quantization.c -o tensor_demo`
3. Run the compiled executable:
   `./tensor_demo`

   📊 Demo Output Example
   
--- Original Model Weights (32-bit Float) ---
1.25  -3.80  5.10  4.50  -0.45  2.00  
Memory Consumption: 24 Bytes

--- Quantized Weights (8-bit Int) ---
31  -94  127  112  -11  49  
Memory Consumption: 6 Bytes
