class Cell {
public:
    int x, y; // Pozycja komórki
    bool walls[4]; // Ściany komórki: 0-góra, 1-prawo, 2-dół, 3-lewo
    bool visited; // Czy komórka była odwiedzona

    Cell(int x, int y) : x(x), y(y), visited(false) {
        // Na początku wszystkie ściany są obecne
        for (int i = 0; i < 4; ++i) walls[i] = true;
    }
};
