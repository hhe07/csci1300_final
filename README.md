CSCI 1300 Fall 2022
Author: Leo Ge
Recitation: 104 – Christopher Ojukwu
Project 3: Dungeon Escape

# Compilation
``g++ -std=c++17 Member.cpp Party.cpp Merchant.cpp Puzzle.cpp Map.cpp Monster.cpp Driver.cpp game.cpp Assets.cpp -o game.out ``

# Misc.
Changed from CYO to Dungeon Escape halfway through.

# Sources

- https://stackoverflow.com/questions/21097523/insert-name-of-struct-was-not-declared-in-this-scope
  - Used to resolve debugging issue with where to put structs
- https://stackoverflow.com/questions/30069384/provides-no-initializer-for-reference-member
- https://stackoverflow.com/questions/22437903/non-static-reference-member-cant-use-default-assignment-operator
- https://stackoverflow.com/questions/23476307/passing-reference-to-constructor-default-argument
- https://stackoverflow.com/questions/53176082/function-operator-declared-implicitly-cannot-be-referenced-it-is-a-deleted-fu
  - Used to resolve issues with references in constructors, because using pointers was definitely worse :)
- https://stackoverflow.com/questions/5685471/error-jump-to-case-label-in-switch-statement
  - Used to resolve an issue with switch statements.
- https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
  - Used to make a decision on how to format strings (decided on pain instead of this)
- https://stackoverflow.com/questions/3136520/determine-if-map-contains-a-value-for-a-key
  - Idea used throughout to do input verification on map keys
- https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
  - Used as inspiration for less annoying split algorithm
- https://en.cppreference.com/w/cpp/container/map
  - General reference on map data structure
- https://en.cppreference.com/w/cpp/container/vector
  - General reference on vector data structure
- https://www.ing.iac.es//~docs/external/bash/abs-guide/colorizing.html#
  - Information about how to bold fonts