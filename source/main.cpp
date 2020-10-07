#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.setWindowTitle("Coreavor");
  w.show();
  return a.exec();
}
