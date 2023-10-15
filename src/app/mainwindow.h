#ifndef QSIGVIEWER_MAINWINDOW_H
#define QSIGVIEWER_MAINWINDOW_H

#include <signaldata.h>

#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
  std::vector<SignalData> allSignals;

 private:
  Ui::MainWindow *ui;

 public slots:

  [[maybe_unused]] void handleMenuAction(QAction *action);

  void handleSigOpen(const QString &filePath, int sampleRate);

  void handleOffsetChange(int value);
  void handleScaleChange(int value);
  void handleTimeChange(int value);
  void handleDelayChange(int value);
  void handlePrintAction();
};

#endif  // QSIGVIEWER_MAINWINDOW_H
