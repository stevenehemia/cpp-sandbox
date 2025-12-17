#include <iostream>
#include <string>

using namespace std;


void initialiseGrid(char grid[10][10]);
void printGrid(char grid[10][10]);
void convertCell(const char *cell, int &x, int &y);
void attackCell(char grid[10][10], const char *cell);
void attackCells(char grid[10][10], const char *cell[]);


int main()
{
    char battleshipsGrid[10][10];
    initialiseGrid(battleshipsGrid);
    printGrid(battleshipsGrid);

    attackCell(battleshipsGrid, "D4");
    printGrid(battleshipsGrid);

    const char *targetCells[] = {"D4", "A1", "H6", NULL};
    attackCells(battleshipsGrid, targetCells);
    printGrid(battleshipsGrid);

    return 0;
}


void initialiseGrid(char grid[10][10])
{
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            grid[r][c] = '.';
        }
    }
}

void printGrid(char grid[10][10])
{
    cout << "  0 1 2 3 4 5 6 7 8 9" << endl;
    for (int r = 0; r < 10; r++) {
        cout << static_cast<char>('A' + r);
        for (int c = 0; c < 10; c++) {
            cout << " " << grid[r][c];
        }
        cout << endl;
    }
}

void convertCell(const char *cell, int &x, int &y)
{
    y = static_cast<int>(cell[0] - 'A');
    x = static_cast<int>(cell[1] - '0');
}

void attackCell(char grid[10][10], const char *cell)
{
    int r, c = 0;
    convertCell(cell, c, r);
    grid[r][c] = 'x';
}

void attackCells(char grid[10][10], const char *cell[])
{
    if (cell[0] == NULL) { return; }

    attackCell(grid, cell[0]);
    attackCells(grid, cell + 1);
}