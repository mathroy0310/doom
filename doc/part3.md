# PART3 - Adding SDL

this part is pretty easy you just need to add SDL my makefile is kinda special since it looks if you are in nix-shell it links with nix-shell SDL if not it links with system SDL

engine/DOOM is the internal of the doom clone it will track most of data player, game, ennemy stuff

if you look on the constructior doom the res is really low to give the doom engine look

now doom loads the WAD

also added keyevents but they dont do anything yet,

this part is simple really just the basic SDL window stuff but with this seperated in game and doom.cpp 
game.cpp is everything about the window really

[SDL Wiki](https://wiki.libsdl.org/SDL2/FrontPage)
[Lazy Foo SDL tutorials](https://lazyfoo.net/SDL_tutorials/)