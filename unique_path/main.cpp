#include <iostream>
#include <vector>
#include <string>


const int DIRECTIONS[][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
const int HEIGHT = 3;
const int WIDTH = 4;


void displayGrid(char grid[HEIGHT][WIDTH]);
std::vector<std::string> uniquePaths(char grid[HEIGHT][WIDTH]);
bool findPosition(char grid[HEIGHT][WIDTH], char ch, int &row, int &col);
bool makeMove(char grid[HEIGHT][WIDTH], int row, int col);
void solve(char grid[HEIGHT][WIDTH], int startRow, int startCol, int endRow, int endCol,
    std::string path, std::vector<std::string> &pathList);
bool isComplete(char grid[HEIGHT][WIDTH]);


int main () {
    /** 
     * @ representing the starting square, $ representing the ending square
     *   representing empty square, # representing obstacles
     */
    char grid[HEIGHT][WIDTH] = {{'@',' ',' ',' '},
                                {' ',' ',' ',' '},
                                {' ',' ',' ','$'}};

    std::vector<std::string> pathList = uniquePaths(grid);
    displayGrid(grid);

    std::cout << "There are " << pathList.size() << " unique path(s) "
              << "that walks each empty square once." << std::endl;
    for (std::string path : pathList) {
        std::cout << path << std::endl;
    }

    return 0;
}


void displayGrid(char grid[HEIGHT][WIDTH]) {

    for (int i = 0; i <= 4 * WIDTH; i++) {
        std::cout << '-';
    }
    std::cout << std::endl;

    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            std::cout << "| " << grid[r][c] << ' ';
        }
        std::cout << '|' << std::endl;
        for (int i = 0; i <= 4 * WIDTH; i++) {
            std::cout << '-';
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}


std::vector<std::string> uniquePaths(char grid[HEIGHT][WIDTH]) {

    std::string path = "";
    std::vector<std::string> pathList;
    int startRow, startCol, endRow, endCol;

    if (!findPosition(grid, '@', startRow, startCol)) {
        pathList.clear();
        return pathList;
    }

    if (!findPosition(grid, '$', endRow, endCol)) {
        pathList.clear();
        return pathList;
    }

    solve(grid, startRow, startCol, endRow, endCol, path, pathList);

    return pathList;
}


bool findPosition(char grid[HEIGHT][WIDTH], char ch, int &row, int &col) {

    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (grid[r][c] == ch) {
                row = r; col = c;
                return true;
            }
        }
    }

    return false;
}


bool makeMove(char grid[HEIGHT][WIDTH], int row, int col) {

    if (row < 0 || row >= HEIGHT) { return false; }
    if (col < 0 || col >= WIDTH) { return false; }
    if (grid[row][col] == ' ') {
        grid[row][col] = '+';
        return true;
    }
    if (grid[row][col] == '$') {
        grid[row][col] = '*';
        return true;
    }
    return false;
}


void uniquePaths(char grid[HEIGHT][WIDTH], int startRow, int startCol, int endRow, int endCol,
    std::string path, std::vector<std::string> &pathList) {
    
    if (grid[endRow][endCol] == '*') {
        if (isComplete(grid)) { pathList.push_back(path); }
        return;
    }

    for (auto d : DIRECTIONS) {
        int nextRow = startRow + d[0], nextCol = startCol + d[1];
        char temp = grid[nextRow][nextCol];
        if (makeMove(grid, nextRow, nextCol)) {
            std::string addString = "";
            addString = addString + '(' + static_cast<char>(nextRow + '0') + ", "
                        + static_cast<char>(nextCol + '0') + ") ";
            path = path + addString;
            uniquePaths(grid, nextRow, nextCol, endRow, endCol, path, pathList);
            path.erase(path.length() - addString.length());
            grid[nextRow][nextCol] = temp;
        }
    }
}


bool isComplete(char grid[HEIGHT][WIDTH]) {

    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (grid[r][c] == ' ') { return false; }
        }
    }
    return true;
}