#include <iostream>

using namespace std;

const int HEIGHT = 5;
const int WIDTH = 6;

void initialiseMap(char map[][WIDTH]);
void transform(int elv[], char map[][WIDTH]);
void printMap(char map[][WIDTH]);
int fill(char map[][WIDTH]);

int main() {

    int elevation[WIDTH] = { 4, 2, 0, 3, 2, 5 };
    char map[HEIGHT][WIDTH];
    initialiseMap(map);
    transform(elevation, map);
    std::cout << "Total water: " << fill(map) << std::endl;
    printMap(map);
    return 0;
}


void initialiseMap(char map[][WIDTH]) {
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            map[r][c] = ' ';
        }
    }
}

void transform(int elv[], char map[][WIDTH]) {
    for (int i = 0; i < WIDTH; i++) {
        for (int r = 0; r < elv[i]; r++) {
            map[HEIGHT - 1 - r][i] = '#';
        }
    }
}

void printMap(char map[][WIDTH]) {
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            std::cout << map[r][c];
        }
        std::cout << std::endl;
    }
}

int fill(char map[][WIDTH]) {
    for (int r = HEIGHT - 1; r >= 0; r--) {
        bool withinWall = false;
        for (int c = 0; c < WIDTH; c++) {
            if (map[r][c] == '#') {
                withinWall = true;
                continue;
            }
            if (withinWall) {
                if (map[r][c] == '#') { withinWall = false; }
                else { map[r][c] = '*'; }
            }
        }
        if (withinWall) {
            for (int c = WIDTH; c >= 0; c--) {
                if (map[r][c] == '#') { break; }
                map[r][c] = ' ';
            }
        }
    }
    int count = 0;
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (map[r][c] == '*') { count++; }
        }
    }
    return count;
}