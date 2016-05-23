# mazeweb
Liam Hill
---------
## http://hillliam.github.io/mazeweb/

how to build
------------

debug
======
emcc maze.cpp mazeweb.cpp -o mazeweb.js -g4 -s NO_EXIT_RUNTIME=1

release
=======
emcc maze.cpp mazeweb.cpp -o mazeweb.js -O2 -s NO_EXIT_RUNTIME=1

improvements
------------
- [ ] ability to open mazes created in edit mode or the windows application
- [ ] ability to save mazes created in edit mode
