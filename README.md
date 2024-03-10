# Shell Simulator

This Shell Simulator, developed for COSC 208: Introduction to Computer Systems in Fall 2023, is a C-based application that mimics the behavior of a Unix shell. It supports basic shell commands, including background process management, and is designed to provide an educational insight into how shell environments operate.

## Features

- **Command Parsing**: Tokenizes user input into commands and arguments for execution.
- **Built-in Commands**: Includes built-in support for `cd` to change directories and `fg` to bring background processes to the foreground.
- **Background Execution**: Allows commands to run in the background when appended with `&`, enabling continuous shell usage.
- **Signal Handling**: Manages SIGCHLD to clean up zombie processes and ignores SIGINT to prevent shell termination with Ctrl+C.
- **Process Management**: Supports forking and executing commands, with proper handling of foreground and background processes.

## Development

The development of this Shell Simulator focused on understanding process creation and management in Unix-like systems, parsing user input, and signal handling. Challenges included managing background processes and preventing shell termination through signal interception.

## Acknowledgments

Special thanks to [Ethan Rackleff] for completing this project with me.

## Usage

Compile the shell simulator using a C compiler and run the generated executable:

```bash
gcc -o shell_simulator shell_simulator.c
./shell_simulator

Once started, the shell presents a prompt (`shell> `) waiting for user input. Users can enter commands just like in a Unix shell. Special features include:

- To run a command in the background, append `&` at the end of the command.
- Use `cd [directory]` to change the current working directory.
- Use `fg` to continue the most recent background process in the foreground.
- Input `Ctrl+C` or type `exit` to quit the shell simulator.

