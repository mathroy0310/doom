# PART6 - Node structure

we just need to read the nodes of map BSP tree

nodes are in an array and we gonna sort them in array format

## Node Data Format
| Field Size | Data Type                            | Content                                          |
| :--------: | :----------------------------------- | :----------------------------------------------- |
| 0x00-0x01  | Partition line x coordinate          | X coordinate of the splitter                     |
| 0x02-0x03  | Partition line y coordinate          | Y coordinate of the splitter                     |
| 0x04-0x05  | Change in x to end of partition line | The amount to move in X to reach end of splitter |
| 0x06-0x07  | Change in y to end of partition line | The amount to move in Y to reach end of splitter |
| 0x08-0x09  | Right box top                        | First corner of right box (Y coordinate)         |
| 0x0A-0x0B  | Right box bottom                     | Second corner of right box (Y coordinate)        |
| 0x0C-0x0D  | Right box left                       | First corner of right box (X coordinate)         |
| 0x0E-0x0F  | Right box right                      | Second corner of right box (X coordinate)        |
| 0x10-0x11  | Left box top                         | First corner of left box (Y coordinate)          |
| 0x12-0x13  | Left box bottom                      | Second corner of left box (Y coordinate)         |
| 0x14-0x15  | Left box left                        | First corner of left box (X coordinate)          |
| 0x16-0x17  | Left box right                       | Second corner of left box (X coordinate)         |
| 0x18-0x19  | Right child index                    | Index of the right child + sub-sector indicator  |
| 0x1A-0x1B  | Left child index                     | Index of the left child + sub-sector indicator   |

this a lot of new stuff but interesting things
