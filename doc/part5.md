# PART5 - Player and Things

before drawing and rendering the player we need to know what player is

player is a "Thing" so position of the player is specified in a Thing lump of the map

first we need to read and load the thing lump
create a player class and add the player to automap 

we gonna need angle and position to the player so I added setters and getters for this, Im creating the player with and ID with idea it could have more than one player, 

I changed the Map constructor so it needs a Player

## Thing Format

| Field Size | Data Type      | Content    |
| :--------: | :------------- | :--------- |
| 0x00-0x01  | Signed short   | X Position |
| 0x02-0x03  | Signed short   | Y Position |
| 0x04-0x05  | Unsigned short | Angle      |
| 0x06-0x07  | Unsigned short | Type       |
| 0x08-0x09  | Unsigned short | Flags      |


added Thing struct and thing vector in Map class


to draw the player ive tried to do a circle but i did a square 3x3 instead