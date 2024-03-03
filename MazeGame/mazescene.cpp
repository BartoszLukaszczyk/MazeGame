#include "MazeScene.h"
#include <QGraphicsRectItem>


MazeScene::MazeScene(Maze *maze) : maze(maze)
{
    int sceneWidth = maze->getCols() * 20; // Maze width depends on number of columns
    int sceneHeight = maze->getRows() * 20; // Maze height depends on number of rows
    setSceneRect(0, 0, sceneWidth, sceneHeight);
}


void MazeScene::drawBackground(QPainter* painter, const QRectF& rect) {
    maze->draw(*painter);
}
