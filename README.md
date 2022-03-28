# clipboard_swapper
A simple cross-platform program, written in C, to swap your clipboard content.

## How it works
The program needs a lookup-table where it can search for matching words. 
If the word that you copy to the clipboard is somewhere in the lookup-table, it will replace the copied word with the word associated to it.

It even works in both directions. However to prevent an infinite swapping-loop you first have to copy another word before swapping back.

## Word Table
The provided lookup-table for the program has to be in a certain format:
```

word1,word2
word3,word4
...

```
To add the lookup-table to the project simply add a __.txt__ file to the project directory and add your words to it.
Then either execute the __make__ command or rename it __wordTable.txt__ and copy it to the __bin__ directory.

## Running the program
On Windows by default the console which hosts the program is hidden. To check if the program is running simply open the Task Manager and search for it.
On Linux the program isn't hidden by default but you can make it a background process by adding a __&__ on executing.
#### Stopping the program
This might be a trivial matter, but because there is no GUI or something like that, you have to stop the task yourself (e.g. Task Manager).

## Dependencies
Dependencies for the project:
```

git, make

```
This project also uses a lightweight clipboard library which depends on ***CMake***: [libclipboard](https://github.com/jtanx/libclipboard)

## Installation
Simply run these commands and you should find the binary in the __bin__ directory.
```

git clone https://github.com/Moarbue/clipboard_swapper.git
cd clipboard_swapper
make install

```
or all in one
```

git clone https://github.com/Moarbue/clipboard_swapper.git && cd clipboard_swapper && make install

```
## Platforms
The program was tested on Arch Linux (should work on every other distro too) and on Windows using MSYS2 for the mingw-toolchain.
