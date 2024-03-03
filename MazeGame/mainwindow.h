//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "maze.h"
#include "mazescene.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QTimer>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onGenerateButtonClicked();
    void on_pushButton_clicked();
    void updateTimer();

private:
    Ui::MainWindow *ui;
    Maze* maze;
    MazeScene* scene;
    QTimer *timer;
    QLabel *timerLabel;
    int elapsedTime; // in seconds
    int finalTime;
    int totalSteps = 0;
};

#endif // MAINWINDOW_H
