//
// Created by 毛亚琛 on 2023/10/13.
//

#ifndef QSIGVIEWER_SIGNALDATA_H
#define QSIGVIEWER_SIGNALDATA_H

#include <QString>
#include <QVector>
#include <vector>

class SignalData {
 private:
  int sampleRate;
  QVector<double> data;
  QVector<double> time;

 public:
  SignalData(int sampleRate, const QString &path);

  const QVector<double> &getData();

  const QVector<double> &getTime();
};

#endif  // QSIGVIEWER_SIGNALDATA_H
