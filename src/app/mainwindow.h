#ifndef QSIGVIEWER_MAINWINDOW_H
#define QSIGVIEWER_MAINWINDOW_H

#include <signaldata.h>
#include <timeticker.h>

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

 private:
  Ui::MainWindow *ui;
  int maxChunksVisible;
  int chunkSize;
  QSharedPointer<TimeTicker> timeTicker;
  SignalData *signalData;

 public slots:

  [[maybe_unused]] void handleMenuAction(QAction *action);

  void handleSigOpen(const SignalInfo &signalInfo);
  void handleOffsetChange(int value);
  void handleScaleChange(int value);
  void handleTimeChange(int value);
  void handleDelayChange(int value);
  void handlePrintAction();
  void handlePsdOpen();
  void handleXRangeChanged(const QCPRange &range);
  void handleTimeInput();
  void handleTimeSliderChange(int value);
};

#endif  // QSIGVIEWER_MAINWINDOW_H
