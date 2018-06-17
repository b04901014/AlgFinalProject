# AlgFinalProject
Algorithm Final Project Team 2

Author: Li-Wei Chen

This project is for the ICCAD contest, please visit <http://iccad-contest.org/2018/tw/Problem_D/2018ICCADContest_ProblemD.pdf> for further information.

## How to Run
  ```
  mkdir bin
  make
  ./bin/Watermark <input> <md5file_1> <md5file_2> ... <md5file_n> <output>
  ```

## TODO
  1. Change the input format into the one in the contest, which is
  ```
  ./bin/Watermark -i <input> -o <output> -m <md5file_1> <md5file_2> ... <md5file_n>
  ```
  2. Test on verilog simulation
