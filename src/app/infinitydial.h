#ifndef QSIGVIEWER_INFINITYDIAL_H
#define QSIGVIEWER_INFINITYDIAL_H

#include <QDial>

class InfinityDial : public QDial {
  Q_OBJECT
 public:
  explicit InfinityDial(QWidget *parent = nullptr);
  ~InfinityDial() override;

 private:
  int previousValue = 0;

 public slots:
  void handleValueChanged(int value);

 signals:
  void valueIncreased(int value);
};

#endif  // QSIGVIEWER_INFINITYDIAL_H
