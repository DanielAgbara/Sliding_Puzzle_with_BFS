# Sliding_Puzzle_with_BFS
Input/Output Specifications
• The input file is going to have the structure explained below. You do not need to make any error checks for the
correctness of the input file. You can assume that input specifications are always going to be satisfied by the
test inputs.
#k
4
#initial state
1 2 3 4 5 6 7 8 9 10 15 11 13 14 0 12
– first and third line will be there for comment. You will ignore them while reading the file.
– second line will denote the number of rows (or columns) of board. i.e.,if k = 4, that means you will
read 16 labels from the input file. Your program can be tested for values of k where k ≤ 10. We’re
not going to test your program for crazy cases. Time is valuable for all of us :)
– fourth line consists of the labels of the puzzle in reading order from left to right, top to bottom.
For instance, input example given above is the reading order of the board that was given as the example
in previous page.
– empty block is going to be denoted by 0. The rest of the labels are going to be the integers from 1 to
k2 − 1.
• Execution of the program: Your program is going to calculate the moves necessary to make in order to
arrive to the goal state. As stated earlier in the previous page, goal state is the state where labels are in the
following order over the example board of 4×4:
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0
That is, all the labels are in sorted order from left to right, top to bottom, and the empty label (denoted by 0)
is at the bottom right corner of the board.
A solution is going to be considered invalid if either one of the following occurs:
– Invalid move: In a board, you can only move the tiles that are around the empty block, and you can
only move them orthogonally. Thus, diagonal moves, or moves that take a tile which is not in immediate
vicinity of empty block is considered invalid.
– Repeated state: If the board state that is to be produced after a move is identical to a state which was
previously created by your program, this will be considered as an invalid move.. (Our evaluation program
will keep track of intermediate states, and will check if you are revisiting an already visited board state)
• Output: The output file needs to strictly satisfy the following structure:
#moves
15 11 12
– first line is comment to be ignored.
– second line consists of labels of the blocks to be moved at a time, first move being the leftmost, and
each move separated by a white space. For instance, the example output given above is a valid output
to solve the initial state given in first page of this document. It stands for moving the blocks 15, 11, and
finally 12 to arrive to the goal state.
