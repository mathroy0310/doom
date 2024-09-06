# PART1 - WAD Files

### official open source code 

https://github.com/chocolate-doom/chocolate-doom
https://github.com/id-Software/DOOM


WAD files are not included in this repository.

WAD files are separate files that contain the game data for DOOM.
They are separated in 3 categories, Header, Lumps and Directory.

## Header Format

| Field Size |  Data Type   | Content                                            |
| :--------: | :----------: | :------------------------------------------------- |
| 0x00-0x03  | 4 ASCII char | Must be an ASCII string (either "IWAD" or "PWAD"). |
| 0x04-0x07  | unsigned int | The number entries in the directory.               |
| 0x08-0x0b  | unsigned int | Offset in bytes to the directory in the WAD file.  |

## Directories Format

| Field Size |  Data Type   | Content                                                     |
| :--------: | :----------: | :---------------------------------------------------------- |
| 0x00-0x03  | unsigned int | Offset value to the start of the lump data in the WAD file. |
| 0x04-0x07  | unsigned int | The size of the lump in bytes.                              |
| 0x08-0x0f  | 8 ASCII char | ASCII holding the name of the lump.                         |

You can use Slade 3 to open and edit WAD files. 
