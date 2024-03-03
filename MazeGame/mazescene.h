#ifndef MAZESCENE_H
#define MAZESCENE_H

#include <QGraphicsScene>
#include "maze.h"

class MazeScene : public QGraphicsScene
{
public:
    MazeScene(Maze *maze);

protected:
    void drawBackground(QPainter* painter, const QRectF& rect) override;

private:
    Maze *maze;
};

#endif // MAZESCENE_H
