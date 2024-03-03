#include "MazeScene.h"
#include <QGraphicsRectItem>


MazeScene::MazeScene(Maze *maze) : maze(maze)
{
    int sceneWidth = maze->getCols() * 20; // Szerokość sceny w zależności od liczby kolumn
    int sceneHeight = maze->getRows() * 20; // Wysokość sceny w zależności od liczby wierszy
    setSceneRect(0, 0, sceneWidth, sceneHeight);
}


void MazeScene::drawBackground(QPainter* painter, const QRectF& rect) {
    maze->draw(*painter);
}
