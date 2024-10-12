# Takuzu
Takuzu is a logic-based puzzle game played on an NxN grid, where N is typically 4, 8, 16, 32, or 64. The grid is filled with 0s and 1s, and the objective is to complete the grid following these rules:

-Each row and column must contain an equal number of 0s and 1s.  
-No more than two consecutive 0s or 1s can appear in any row or column.  
-No two rows or columns can be identical.  

The game has two modes: **Solver Mode** and **Generation Mode**.

- **Solver Mode (Default)**: Use this mode to solve an existing grid. The grid must be NxN in size, where N can be 4, 8, 16, 32, or 64. By default, the program will find one solution, but you can use the `-a` option to find all solutions.

- **Generation Mode**: Use this mode to generate new grids with the `-gN` option, where N is the grid size (default is 8, with options of 4, 8, 16, 32, or 64). To generate grids with a unique solution, use the `-u` option along with `-g`.

For both modes, the `-v` option will print each choice made during the solving process, along with the grid at every step. To redirect the solutions to a specific file, use the `-o` option followed by the desired output file name (e.g., `.txt`).
