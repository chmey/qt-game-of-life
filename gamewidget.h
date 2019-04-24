#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <memory>
#include <QTimer>
#include <QTime>
#include <QFileDialog>
#include <fstream>
#include <string>
#include <array>
#include "universe.h"

#define HIST_SIZE 5
class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);

    ~GameWidget();
protected:
    void paintEvent(QPaintEvent *);
signals:
    void updatefps(QString);
public slots:
    void iterate();
    void startGame();
    void stopGame();
    void loadRLE();
    void saveRLE();
    void setFPS(QString text);
    void rewind();
private:
    std::unique_ptr<Universe> universe;
    std::array<std::unique_ptr<Universe>, HIST_SIZE> history;
    void drawUniverse(QPainter &p);
    std::unique_ptr<QTimer> timer;
    int frameCount;
    int iterationCount;
    std::unique_ptr<QTime> frameTime;
};

#endif // GAMEWIDGET_H
