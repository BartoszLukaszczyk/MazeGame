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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QLabel *label = new QLabel(this);
    label->setText("Creator: \nBartosz Łukaszczyk");
    label->show();
    label->setGeometry(650, 520, 100, 30);
    ui->setupUi(this);
    maze = new Maze(20, 20);  // tworzy nowy labirynt o rozmiarze 20x20
    scene = new MazeScene(maze);  // inicjalizujemy scenę tutaj
    ui->MazeGraphicsView->setScene(scene);
    // Ustawienie skali na 1:1
    ui->MazeGraphicsView->resetTransform();
    // Centrowanie widoku na punkcie (0,0) sceny
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

MainWindow::~MainWindow()
{
    delete ui;
    delete maze;
    delete scene;
    delete timer;
    delete timerLabel;
}

void MainWindow::onGenerateButtonClicked()
{
    maze->setPlayerPosition(0, 0);
    maze->resetMaze();
    elapsedTime = 0;
    ui->MazeGraphicsView->viewport()->update();

}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(ui->MazeGraphicsView->viewport());
    maze->draw(painter);
    QMainWindow::paintEvent(event);
    ui->MazeGraphicsView->viewport()->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Pobieranie aktualnej pozycji gracza
    std::pair<int, int> position = maze->getPlayerPosition();
    int row = position.first;
    int col = position.second;

    // Sprawdzanie, który klawisz został naciśnięty i akualizowanie pozycji gracza
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

    // Malowanie komórki gracza
    QPainter painter(ui->MazeGraphicsView->viewport());
    maze->setPlayerPosition(row, col);
    totalSteps++;
    update();

    int maxRow = maze->getRows() - 1;
    int maxCol = maze->getCols() - 1;
    if (row == maze->getRows() - 1 && col == maze->getCols() - 1) {
        finalTime = elapsedTime; // zapisz ostateczny czas
        int shortestPathLength = maze->calculateShortestPathLength();
        float score = 10*  (100 - static_cast<float>(finalTime)/shortestPathLength * 100) + shortestPathLength / 4;
        int roundedScore = qRound(score);

        QMessageBox msgBox;
        msgBox.setText(
            QString("<div align='center'><b>Congratulations!</b><br/>"
                    "You Find Escape.<br/>"
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
void MainWindow::updateTimer() {
    elapsedTime++;
    timerLabel->setText(QString("Time: %1s").arg(elapsedTime));
}
void MainWindow::on_pushButton_clicked()
{
    maze->resetPlayerPosition();
    maze->resetMaze();
    ui->MazeGraphicsView->viewport()->update();
}
