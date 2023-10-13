#include <mainwindow.h>
#include <qcustomplot.h>

#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setStyle("fusion");
  MainWindow w;
  w.show();
  return QApplication::exec();
}
