#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString currentFile;
    QGraphicsScene *scene;
    QPixmap image;
    int zoomin{0};

    private slots:
        void open();
        void zoomIn();
        void zoomOut();
        void properties();
        void openContainingFolder();
};
