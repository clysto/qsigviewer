#include <mainwindow.h>
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setStyle("fusion");
  MainWindow w;
  w.show();
  return QApplication::exec();
}
