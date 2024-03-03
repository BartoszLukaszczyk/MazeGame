// main.cpp
#include "mainwindow.h"
#include "maze.h"
#include "cell.h"
#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{

    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // Tworzenie obiektu klasy Maze
    Maze maze(20, 20);
    // Metoda draw() jest wywoływana wewnątrz MainWindow::paintEvent(QPaintEvent *event)
    return a.exec();
}
