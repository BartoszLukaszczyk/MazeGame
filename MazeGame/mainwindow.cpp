//mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QPainter>
#include "maze.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QString>
#include <QLabel>

// Constructor for the MainWindow class.
// Sets up the UI elements and initializes the maze.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QLabel *label = new QLabel(this);
    label->setText("Creator: \nBartosz Łukaszczyk");
    label->show();
    label->setGeometry(650, 520, 100, 30);
    ui->setupUi(this);
    maze = new Maze(20, 20);  
    scene = new MazeScene(maze);  
    ui->MazeGraphicsView->setScene(scene);
    ui->MazeGraphicsView->resetTransform();
    ui->MazeGraphicsView->centerOn(0, 0);
    maze->setPlayerPosition(0, 0);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onGenerateButtonClicked);

    onGenerateButtonClicked();

    ui->MazeGraphicsView->viewport()->update();

    elapsedTime = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    timer->start(1000);
    timerLabel = new QLabel(this);
    timerLabel->setGeometry(30, 30, 100, 30);
    timerLabel->show();
    setWindowTitle("MazeGame");
}
// Destructor for the MainWindow class.
// Cleans up memory by deleting the UI, maze, scene, timer, and timer label objects.
MainWindow::~MainWindow()
{
    delete ui;
    delete maze;
    delete scene;
    delete timer;
    delete timerLabel;
}
// Slot function triggered when the "Generate" button is clicked.
// Resets the maze, player position, and elapsed time.
void MainWindow::onGenerateButtonClicked()
{
    maze->setPlayerPosition(0, 0);
    maze->resetMaze();
    elapsedTime = 0;
    ui->MazeGraphicsView->viewport()->update();

}
// Event handler for painting on the MainWindow.
// Draws the maze using QPainter.
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(ui->MazeGraphicsView->viewport());
    maze->draw(painter);
    QMainWindow::paintEvent(event);
    ui->MazeGraphicsView->viewport()->update();
}
// Event handler for key press events.
// Moves the player based on the arrow keys (up, down, left, right).
// Calculates the score and displays a congratulatory message if the player reaches the exit.
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    std::pair<int, int> position = maze->getPlayerPosition();
    int row = position.first;
    int col = position.second;

    switch (event->key()) {
    case Qt::Key_A:
        col--;
        break;
    case Qt::Key_D:
        col++;
        break;
    case Qt::Key_W:
        row--;
        break;
    case Qt::Key_S:
        row++;
        break;
    case Qt::Key_R:
        break;
    }
    
    QPainter painter(ui->MazeGraphicsView->viewport());
    maze->setPlayerPosition(row, col);
    totalSteps++;
    update();

    int maxRow = maze->getRows() - 1;
    int maxCol = maze->getCols() - 1;
    if (row == maze->getRows() - 1 && col == maze->getCols() - 1) {
        finalTime = elapsedTime; 
        int shortestPathLength = maze->calculateShortestPathLength();
        float score = 10*  (100 - static_cast<float>(finalTime)/shortestPathLength * 100) + shortestPathLength / 4;
        int roundedScore = qRound(score);

        QMessageBox msgBox;
        msgBox.setText(
            QString("<div align='center'><b>Congratulations!</b><br/>"
                    "You Found a Way Out.<br/>"
                    "Your Time: %1 s.<br/>"
                    "Path Lenght: %2.</div>"
                    "<div align='center'><b>Score: %3.</b></div>")
                .arg(finalTime)
                .arg(shortestPathLength)
                .arg(roundedScore));
        msgBox.exec();
        score = 0;
        totalSteps = 0;
    }
}
// Slot function to update the timer label.
// Increments the elapsed time and updates the timer label text.
void MainWindow::updateTimer() {
    elapsedTime++;
    timerLabel->setText(QString("Time: %1s").arg(elapsedTime));
}
// Slot function triggered when the "Reset" button is clicked.
// Resets the maze and player position.
void MainWindow::on_pushButton_clicked()
{
    maze->resetPlayerPosition();
    maze->resetMaze();
    ui->MazeGraphicsView->viewport()->update();
}
