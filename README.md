reversi6
====
## Overview
solve reversi 6x6

## Requirement
* g++ (>= 4.9.2) --std=c++11
* boost (>= 1.5.0)

## Usage
`./prg e4 c5 b4 e3`  
`./prg e4 c5 b4 e3 c2 d2`  
`./prg e4 c5 b4 e3 c2 d2 f3 f4`  
`./prg e4 c5 b4 e3 c2 d2 f3 f4 f5 b3`  
`./prg e4 c5 b4 e3 c2 d2 f3 f4 f5 b3 a3 c1`  

## ChangeLog

* v0.1
 * Bit Board
 * Move Ordering with Next Mobilty
 * PVS
 * 28: 61m30s, 26: 3m3s, 24: 1m20s, 22: 6.3s, 20: 1.2s
* v0.2
 * Move Ordering with Next Mobility and Static score
 * 28: 18m4s, 26: 50s, 24: 26s, 22: 2.4s, 20: 0.63s

## ToDo

* Board
 * test class for new implementation
 * evaluate, can_move, pass, set move
 * pointer (tree) baseed
 * unordered map does not work... why???
 * Zobrist hash
* PVS
 * multi-thread

## Author
* dieumort

## License
Copyright (C) 2015 dieumort All Rights Reserved.
