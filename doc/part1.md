# PART1

### official open source code 

https://github.com/chocolate-doom/chocolate-doom
https://github.com/id-Software/DOOM


WAD files are not included in this repository.

WAD files are separate files that contain the game data for DOOM.
They are separated in 3 categories, Header, Lumps and Directory.

## Header
- first 4 bytes are the header, which is the string "IWAD" or "PWAD" in ASCII. (4 char)
- byte 4 to 7 are the number of entries in the directory. (uint32)
- byte 8 to 11 are the offset (in byte) to the directory. (uint32)

## Directories

- first 4 bytes are the offset to the lump. (uint32)
- byte 4 to 7 are the size of the lump. (uint32)
- byte 8 to 15 are the name of the lump. (8 char)

You can use Slade 3 to open and edit WAD files. 
