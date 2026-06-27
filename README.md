# hsh

A simple Unix-like shell written in C11.

A minimal command-line interpreter for Unix-like systems. It supports basic shell built-ins, process execution via fork/exec, and command-line editing via GNU Readline.

## Features

* **Interactive Prompt:** Displays the current working directory using ANSI color codes.
* **Line Editing & Autocomplete:** Uses GNU Readline for TAB completion and history navigation (Up/Down arrows).
* **Built-in Commands:** Supports `cd`, `history`, and `exit` inside the shell loop.
* **Process Management:** Executes external programs in child processes using `fork` and `exec`.
* **Testing:** Includes basic unit tests for the line-tokenization logic.

## Building and Running

### Prerequisites

Install dependencies (Debian/Ubuntu):

```bash
sudo apt update
sudo apt install cmake libreadline-dev
```

### Build

```bash
git clone https://github.com/Aleksan9898/hsh.git
cd hsh
cmake -B build
cmake --build build
```

### Run shell

```bash
./build/hsh
```

### Run tests

```bash
./build/run_unit_tests
```

## Implementation Details

The project avoids unsafe legacy string functions like `strcpy` and `strcat` where possible.

For performance-sensitive string operations, POSIX functions such as `stpcpy` are used in controlled contexts.

This helps reduce risk of buffer overflows and improves compatibility with static analysis tools like `clang-analyzer`.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
