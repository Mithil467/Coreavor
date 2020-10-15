#include "mainwindow.h"

#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFontDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  resize(560, 420);

  QStringList argumentList = QApplication::arguments();
  if (argumentList.size() > 1) {
    currentFile = argumentList.at(1);
    showImage();
  }

  // Image menu
  connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
  connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
  connect(ui->actionOpen_Containing_Folder, &QAction::triggered, this,
          &MainWindow::openContainingFolder);
  connect(ui->actionProperties, &QAction::triggered, this,
          &MainWindow::properties);
  connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exit);

  // Edit menu
  connect(ui->actionRotate, &QAction::triggered, this, &MainWindow::rotate);
  connect(ui->actionTrash, &QAction::triggered, this, &MainWindow::trash);
  connect(ui->actionRename, &QAction::triggered, this, &MainWindow::rename);

  // View menu
  connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::zoomIn);
  connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::zoomOut);
  connect(ui->actionFullscreen, &QAction::triggered, this,
          &MainWindow::fullscreen);

  // Help menu
  connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);

  // Toolbar
  connect(ui->actionPrevious, &QAction::triggered, this,
          &MainWindow::previousImage);
  connect(ui->actionNext, &QAction::triggered, this, &MainWindow::nextImage);

  connect(ui->actionJPG, &QAction::triggered, this,
          [this] { saveAs(QString("jpg")); });
  connect(ui->actionPNG, &QAction::triggered, this,
          [this] { saveAs(QString("png")); });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::open() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Open image", QDir::homePath() + "/Pictures");

  if (fileName != "") {
    currentFile = fileName;
    showImage();
  }
}

void MainWindow::showImage() {
  // Reset scale
  ui->graphicsView->resetMatrix();

  // Image is a Pixmap
  image.load(currentFile);

  scene = new QGraphicsScene(this);

  scene->addPixmap(image);
  scene->setSceneRect(image.rect());
  ui->graphicsView->setScene(scene);

  scaleImageToFitWindow();
  QFileInfo file(currentFile);
  setWindowTitle(file.fileName());
  statusBar()->showMessage(QString("%1 %2x%3px %4 kB")
                               .arg(file.fileName())
                               .arg(image.width())
                               .arg(image.height())
                               .arg(QFile(currentFile).size() / 1000.0));
}

void MainWindow::scaleImageToFitWindow() {
  int y = ui->graphicsView->size().height() - image.height();
  int x = ui->graphicsView->size().width() - image.width();

  double scaleFactor = 1.0;

  double xsf = ui->graphicsView->size().width() / (double)image.width();
  double ysf = ui->graphicsView->size().height() / (double)image.height();

  // There are 4 cases here -
  // 1. (+x,+y) Image fits well in the slot -> Do nothing
  // 2. (-x, +y) Image extends vertically
  // 3. (+x, -y) Image extends horizontally
  // 4. (-x, -y) Image extends both directions -> This has 2 cases

  if (x < 0 && y < 0) {
    if (x < y)
      scaleFactor = xsf;
    else
      scaleFactor = ysf;
  }

  if (x < 0 && y >= 0) scaleFactor = xsf;
  if (x >= 0 && y < 0) scaleFactor = ysf;

  ui->graphicsView->scale(scaleFactor, scaleFactor);
}

void MainWindow::zoomIn() {
  zoomin++;
  if (zoomin <= 12)
    ui->graphicsView->scale(1.2, 1.2);
  else
    zoomin--;
}

void MainWindow::zoomOut() {
  zoomin--;
  if (zoomin >= -8)
    ui->graphicsView->scale(1 / 1.2, 1 / 1.2);
  else
    zoomin++;
}

void MainWindow::properties() {
  QFileInfo file(currentFile);
  QString x = QString(
                  "Name: %1\nType: %2\nSize: %3kB\nWidth: %4 px\nHeight: %5 "
                  "px\nModified Time: %6")
                  .arg(file.fileName())
                  .arg(file.suffix())
                  .arg(file.size() / 1000.0)
                  .arg(image.width())
                  .arg(image.height())
                  .arg(file.birthTime().toString("hh:mm:ss d/M/yyyy"));
  QMessageBox::information(this, currentFile, x);
}

void MainWindow::openContainingFolder() {
  QFileInfo file(currentFile);

  // Get the directory QString
  QDesktopServices::openUrl(file.absoluteDir().absolutePath());
}

void MainWindow::previousImage() {
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

void MainWindow::nextImage() {
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

  if (index != -1) {
    if (index < fileNames.size() - 1)
      currentFile = dir.absoluteFilePath(fileNames.at(index + 1));
    else
      currentFile = dir.absoluteFilePath(fileNames.at(0));
  } else {
    currentFile = "";
  }
  showImage();
}


void MainWindow::rotate() {
  if (!image.isNull()) {
    image = image.transformed(QTransform().rotate(90));
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
  }
}

void MainWindow::trash() {
  QFile::remove(currentFile);

  // Move to next image
  MainWindow::nextImage();
}

void MainWindow::fullscreen() {
  showFullScreen();
  ui->graphicsView->showFullScreen();

  ui->graphicsView->resetMatrix();
  scaleImageToFitWindow();
}

void MainWindow::about() {
  QMessageBox::information(this, "Coreavor",
                           "Simple image viewer\nDeveloper: Mithil Poojary");
}

void MainWindow::save() {
  if (currentFile != "" && !image.isNull()) image.save(currentFile);
}

void MainWindow::saveAs(QString x) {
  if (currentFile != "" && !image.isNull())
    image.save(currentFile + "." + x, x.toUpper().toLocal8Bit().constData());
}
void MainWindow::rename() {
  if(currentFile != "" && !image.isNull()) {
      bool ok;
      QFileInfo file(currentFile);
      QString currentName = file.fileName();
      QString text = QInputDialog::getText(this, tr("Rename"),
                                           tr("Name for the file:"), QLineEdit::Normal,
                                            currentName, &ok);
      if(ok && !text.isEmpty()) {
          QString renamedFile = file.absoluteDir().absoluteFilePath(text);
          QFileInfo checkFile(renamedFile);
          if(checkFile.exists())
              QMessageBox::information(this,"Warning",checkFile.fileName() + " already exists");
          else {
              image.save(renamedFile);
              QFile file(currentFile);
              file.remove();
              file.close();
              currentFile = renamedFile;
              showImage();
          }
      }
  }
}
void MainWindow::exit() { QApplication::quit(); }

void MainWindow::resizeEvent(QResizeEvent*) {
  ui->graphicsView->resetMatrix();
  scaleImageToFitWindow();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent*) {
  setWindowState(isFullScreen() ? Qt::WindowMaximized : Qt::WindowFullScreen);
}
