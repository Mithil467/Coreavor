#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(560, 420);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open image", QDir::homePath()+"/Pictures");
    QFile file(fileName);
    currentFile = fileName;

    // Reset scale
    ui->graphicsView->resetMatrix();

    // Image is a Pixmap
    this->image.load(fileName);

    this->scene = new QGraphicsScene(this);

    this->scene->addPixmap(image);
    this->scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);


    int y = this->size().height() - image.height();
    int x = this->size().width() - image.width();
    double scaleFactor = 1.0;
    double xsf = this->size().width()/(double)image.width();
    double ysf = this->size().height()/(double)image.height();
    // There are 4 cases here -
    // 1. (+x,+y) Image fits well in the slot -> Do nothing
    // 2. (-x, +y) Image extends vertically
    // 3. (+x, -y) Image extends horizontally
    // 4. (-x, -y) Image extends both directions -> This has 2 cases

    if(x<0 && y<0)
    {
        if(x < y)
            scaleFactor = xsf;
        else
            scaleFactor = ysf;
    }
    if(x<0 && y>=0)
        scaleFactor = xsf;
   if(x>=0 && y<0)
       scaleFactor = ysf;
   scaleFactor -= 0.015;


        ui->graphicsView->scale(scaleFactor, scaleFactor);



    setWindowTitle(fileName);
}
