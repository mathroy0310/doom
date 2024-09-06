# PART2 - Basic Map Data

map anatomy
- vertexes/vertices: are the endpoint of a wall in 2d, meeting point of two or more walls
- linedefs: are lines forming the connection between vertices, not all lines behavbe the same, theres a flag to define behavior of those lines
and much more but this only one needed for now and it should work only with those

for exmaple a square
```
____________
|          |
|          |
|          |
|          |
------------
```

it has 4 vertices and 4 lines, each side (lines) and each corner (vertices)

## Vertices format

its pretty simple only  point on a x,y coordinates


| Field Size | Data Type    | Content    |
| :--------: | :----------- | :--------- |
| 0x00-0x01  | Signed short | X Position |
| 0x02-0x03  | Signed short | Y Position |

## linedef format 

| Field Size | Data Type      | Content                                   |
| :--------: | -------------- | :---------------------------------------- |
| 0x00-0x01  | Unsigned short | Start vertex                              |
| 0x02-0x03  | Unsigned short | End vertex                                |
| 0x04-0x05  | Unsigned short | Flags (details below)                     |
| 0x06-0x07  | Unsigned short | Line type / Action                        |
| 0x08-0x09  | Unsigned short | Sector tag                                |
| 0x10-0x11  | Unsigned short | Right sidedef ( 0xFFFF side not present ) |
| 0x12-0x13  | Unsigned short | Left sidedef ( 0xFFFF side not present )  |

### linedef flag valu

|  Bit  | Description                                          |
| :---: | :--------------------------------------------------- |
|   0   | Blocks players and monsters                          |
|   1   | Blocks monsters                                      |
|   2   | Two sided                                            |
|   3   | Upper texture is unpegged (will research this later) |
|   4   | Lower texture is unpegged (will research this later) |
|   5   | Secret (shows as one-sided on automap)               |
|   6   | Blocks sound                                         |
|   7   | Never shows on automap                               |
|   8   | Always shows on automap                              |





print in that format and copy paste to desmos to view
```cpp
std::cout << "("  << vertex.XPosition << "," << vertex.YPosition << ")" << std::endl;
```
https://www.desmos.com/calculator/