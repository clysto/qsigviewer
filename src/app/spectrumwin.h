//
// Created by maoyachen on 2023/10/17.
//

#ifndef QSIGVIEWER_SPECTRUMWIN_H
#define QSIGVIEWER_SPECTRUMWIN_H

#include <QDialog>

#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SpectrumWin;
}
QT_END_NAMESPACE

class SpectrumWin : public QDialog {
  Q_OBJECT

 public:
  explicit SpectrumWin(QWidget *parent, QVector<double> frequency, QVector<double> psd);
  ~SpectrumWin() override;

 private:
  Ui::SpectrumWin *ui;

 public slots:
  void onPlottableClick(QCPAbstractPlottable *plottable, int dataIndex);
};

#endif  // QSIGVIEWER_SPECTRUMWIN_H
