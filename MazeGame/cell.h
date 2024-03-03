#ifndef CELL_H
#define CELL_H

class Cell {
public:
    int x, y; // Pozycja komórki
    bool walls[4];
    bool highlightedWalls[4];
    bool visited; // Czy komórka była odwiedzona

    Cell(int x, int y) : x(x), y(y), visited(false) {
        // Na początku wszystkie ściany są obecne
        for (int i = 0; i < 4; ++i) walls[i] = true;
    }
    bool isPlayerHere = false;
    int steps = 0;
};

#endif // CELL_H
