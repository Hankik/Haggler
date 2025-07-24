# Haggling Game

## overview

- C Style Programing in C++?
- Raylib
- Stole GingerBill's Buddy Allocator
- Using Sean Barret's hash map
- Objs are like Entities, Tags are like Components, and Msgs are like events
- Avoiding RAII with preallocation for simplicity

## notes

- tom.h is the headerfile for Tags and Objs
- tray.h holds my small array type
- sim.h is being used as a manager type for the game (to be reorganized later)

## todo

- [ ] complete tiled to haggler pipeline
- [ ] save game / load game
- [x] switch obj's tag tray to stb's hash map 
- [ ] make a ui objs for starting menu system (separate from sim obj)
- [ ] pathfinding for player movement
- [ ] particle system for juiciness
