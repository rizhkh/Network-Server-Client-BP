# Network (Client-Server protocol)
This remote client-server program allows user to send and receive files, with appropriate privilege allows the file to be read,write and executed, from one server to the other

## Description
Allows to perform basic functions and desired tasks required by the operating party .
Code will confirm checks for appropriate flags with any errors on standby, if prompted will return it to the operating user.
The input functionality is done as an input of a complete string 
```
read file.txt 4 r

Note: Command “read” or “write” can only be written as 'read' or 'write'. 
The '4' is number of bytes to be read or written and 'r' at the end of the string is one of flags.
```
In this piece of code, the threads in this program uses mutexers to allow multiple connections following First In First Out protocol, and connecting connections IP addresses can be displayed by the admin for logs and monitor purposes

## Install & Run
Unix: Upload project in folder, untar for safety purposes and execute
```
run Server and Client file after compilation using ' ./File.c ' in terminal.
```




