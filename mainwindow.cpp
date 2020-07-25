#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <QDateTime>
#include <QDesktopServices>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(560, 420);

    // Image menu
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::print);
    connect(ui->actionSet_As_Desktop_Background, &QAction::triggered, this, &MainWindow::setAsDesktopBackground);
    connect(ui->actionOpen_Containing_Folder, &QAction::triggered, this, &MainWindow::openContainingFolder);
    connect(ui->actionProperties, &QAction::triggered, this, &MainWindow::properties);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exit);

    // Edit menu
    connect(ui->actionRotate, &QAction::triggered, this, &MainWindow::rotate);
    connect(ui->actionTrash, &QAction::triggered, this, &MainWindow::trash);

    // View menu
    connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(ui->actionFullscreen, &QAction::triggered, this, &MainWindow::fullscreen);

    // Help menu
    connect(ui->actionAbout , &QAction::triggered, this, &MainWindow::about);

    // Toolbar
    connect(ui->actionPrevious, &QAction::triggered, this, &MainWindow::previousImage);
    connect(ui->actionNext, &QAction::triggered, this, &MainWindow::nextImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open image", QDir::homePath() + "/Pictures");

    if (fileName != "")
    {
        currentFile = fileName;
        showImage();
    }
}

void MainWindow::showImage()
{
    // Reset scale
    ui->graphicsView->resetMatrix();

    // Image is a Pixmap
    this->image.load(currentFile);

    this->scene = new QGraphicsScene(this);

    this->scene->addPixmap(image);
    this->scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);

    int y = this->size().height() - image.height();
    int x = this->size().width() - image.width();

    double scaleFactor = 1.0;

    double xsf = this->size().width() / (double)image.width();
    double ysf = this->size().height() / (double)image.height();

    // There are 4 cases here -
    // 1. (+x,+y) Image fits well in the slot -> Do nothing
    // 2. (-x, +y) Image extends vertically
    // 3. (+x, -y) Image extends horizontally
    // 4. (-x, -y) Image extends both directions -> This has 2 cases

    if (x < 0 && y < 0)
    {
        if (x < y)
            scaleFactor = xsf;
        else
            scaleFactor = ysf;
    }

    if (x < 0 && y >= 0)
        scaleFactor = xsf;
    if (x >= 0 && y < 0)
        scaleFactor = ysf;

    // Although the above math seems right,
    // there is a small unknown difference
    scaleFactor -= 0.015;

    ui->graphicsView->scale(scaleFactor, scaleFactor);
    QFileInfo file(currentFile);
    setWindowTitle(file.fileName());
    this->statusBar()->showMessage(QString("%1 %2x%3px %4 B").arg(file.fileName()).arg(image.width()).arg(image.height()).arg(QFile(currentFile).size()));
}

void MainWindow::zoomIn()
{
    zoomin++;
    if (zoomin <= 12)
        ui->graphicsView->scale(1.2, 1.2);
    else
        zoomin--;
}

void MainWindow::zoomOut()
{
    zoomin--;
    if (zoomin >= -8)
        ui->graphicsView->scale(1 / 1.2, 1 / 1.2);
    else
        zoomin++;
}

void MainWindow::properties()
{
    QFileInfo file(currentFile);
    QString x = QString("%1 %2").arg(file.absoluteFilePath()).arg(file.size());
}

void MainWindow::openContainingFolder()
{
    QFileInfo file(currentFile);

    // Get the directory QString
    QDesktopServices::openUrl(file.absoluteDir().absolutePath());
}

void MainWindow::previousImage()
{
    QFileInfo file(currentFile);
    QDir dir = file.absoluteDir();
    QStringList nameFilters;
    nameFilters << "*.png"
                << "*.gif"
                << "*.jpg"
                << "*.jpeg"
                << "*.svg";
    QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);

    int index = fileNames.indexOf(QRegExp(QRegExp::escape(file.fileName())));
    if (index > 0)
        currentFile = dir.absoluteFilePath(fileNames.at(index - 1));
    else
        currentFile = dir.absoluteFilePath(fileNames.at(fileNames.size() - 1));

    showImage();
}

void MainWindow::nextImage()
{
    QFileInfo file(currentFile);
    QDir dir = file.absoluteDir();
    QStringList nameFilters;
    nameFilters << "*.png"
                << "*.gif"
                << "*.jpg"
                << "*.jpeg"
                << "*.svg";
    QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);

    int index = fileNames.indexOf(QRegExp(QRegExp::escape(file.fileName())));
    if (index < fileNames.size() - 1)
        currentFile = dir.absoluteFilePath(fileNames.at(index + 1));
    else
        currentFile = dir.absoluteFilePath(fileNames.at(0));

    showImage();
}


void MainWindow::rotate()
{
    this->image = this->image.transformed(QTransform().rotate(90));
    this->scene->addPixmap(image);
    this->scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
}


void MainWindow::trash()
{

}


void MainWindow::fullscreen()
{

}


void MainWindow::about()
{
    QMessageBox::information(this, "Coreavor", "Simple image viewer\nDeveloper: Mithil Poojary");
}

void MainWindow::save()
{
    image.save(currentFile);
}

void MainWindow::saveAs()
{

}

void MainWindow::setAsDesktopBackground()
{

}

void MainWindow::print()
{

}

void MainWindow::exit()
{
    QApplication::quit();
}
