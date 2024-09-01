# Windows Process Injection Sample Project

### **Disclaimer**

This project is for educational and research purposes only. It is intended to help cybersecurity professionals and students understand the concepts and techniques behind Windows process injection. **Do not use this code for malicious activities.** The author is not responsible for any misuse of this project.

---

## **Overview**

This repository contains a sample implementation of Windows process injection technique. Process injection is a technique used by malware authors to inject malicious code into the address space of another process, enabling stealthy execution and persistence. Understanding these techniques is essential for developing effective defensive measures.

---

## **Features**

- **Remote Thread Injection**
- **DLL Injection** -> T.B.C.
- **APC (Asynchronous Procedure Call) Injection** -> T.B.C.
- **Process Hollowing** -> T.B.C.
- **Thread Hijacking** -> T.B.C.

Each technique is implemented in a separate module, with detailed comments explaining the process.

---

## **Getting Started**

### **Prerequisites**

- **Windows Operating System:** This project is designed to work on Windows.
- **Visual Studio:** The code can be compiled using Visual Studio.
- **Administrator Privileges:** Some techniques require elevated privileges.

### **Compiling the Code**

1. Clone the repository:
   ```bash
   git clone https://github.com/tawrid/windows_process_injection.git

2. Open the solution file in Visual Studio.
3. Build the solution using the appropriate configuration (Debug/Release).

### **Running the Sample**
1. Choose a Target Process: You can use any running process on your system.
2. Run the Executable: Execute the compiled binary with the necessary arguments, such as the target process ID or name.
3. Monitor the Behavior: Use tools like Process Explorer or Task Manager to observe the injection process.

### **Legal and Ethical Considerations**
1. Do not run this code on any system without explicit permission from the owner.
2. Ensure that you are compliant with all applicable laws and regulations.
3. This code is provided "as-is" with no warranties or guarantees.


### **References**
1. <a href="https://docs.microsoft.com/en-us/windows/win32/">Microsoft Developer Documentation</a>
2. <a href="https://docs.microsoft.com/en-us/sysinternals/">Windows Internals by Mark Russinovich</a>
3. <a href="https://www.youtube.com/watch?v=oJMvS1Ivg_E">Process Hacker (Youtube Video)</a>