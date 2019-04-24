#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game(new GameWidget(this))
{
    ui->setupUi(this);
    connect(ui->startButton, SIGNAL(clicked()), game,SLOT(startGame()));
    connect(ui->stopButton, SIGNAL(clicked()), game,SLOT(stopGame()));
    connect(game, SIGNAL(updatefps(QString)), ui->fpsLabel, SLOT(setText(QString)));
    connect(ui->loadButton, SIGNAL(clicked()), game, SLOT(loadRLE()));
    connect(ui->saveButton, SIGNAL(clicked()), game, SLOT(saveRLE()));
    connect(ui->fpsInput, SIGNAL(textChanged(QString)), game, SLOT(setFPS(QString)));
    connect(ui->rewindButton, SIGNAL(clicked()), game, SLOT(rewind()));
    ui->gameLayout->addWidget(game);
}
MainWindow::~MainWindow()
{
    delete ui;
}
