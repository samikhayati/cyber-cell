
# Cyber Cell 🔴

https://github.com/samikhayati/cyber-cell/assets/121733284/c47ac962-471a-43ea-82e9-fd6894ffe4a4

*🔊 This video includes sound*
### Description

Cyber-cell is a simple program that runs a bot for the popular browser game Agar.io.  
The bot’s goal is to grow by collecting pellets while avoiding enemies to maximize its survival time and size.


---


## Building on Windows

### Prerequisites

Ensure [Chocolatey](https://chocolatey.org/install) is installed and available in your system PATH.


### Clone the Repository

```
git clone https://github.com/samikhayati/cyber-cell
cd cyber-cell
```
### Install Dependencies

```
choco install cmake
choco install opencv --package-parameters="/Environment"
```

### Configure and Build with CMake

```
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DOpenCV_DIR="C:\tools\opencv\build"
cmake --build build --config Debug
```

### Run the Executable

Before running the executable, open your browser, navigate to Agar.io, and start a game.

```
.\build\Debug\agarioplaybot.exe
```
---

### Usage Hint

Use **key0** to close the program.


