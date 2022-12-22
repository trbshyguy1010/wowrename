# WowRename
File Renaming Tool made in C with foni C code (send help... im too new for this)

## how to compile :
```
gcc `pkg-config --cflags gtk+-3.0` -o main.o main.c `pkg-config --libs gtk+-3.0`
```
1. Run with : `./main.o`
1. Select your folder: `/your/folder/here`
1. Pressing the button will add ascending numbers before the file
#### Example :
`yourfile.txt` -> `n_yourfile.txt` where n is the order of the file from 0
