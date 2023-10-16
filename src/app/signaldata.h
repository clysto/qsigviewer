#ifndef QSIGVIEWER_SIGNALDATA_H
#define QSIGVIEWER_SIGNALDATA_H

#include <qcustomplot.h>

#include <QFile>
#include <QObject>
#include <QString>
#include <QVector>
#include <vector>

class SignalData : public QObject {
  Q_OBJECT

 protected:
  int sampleRate;
  QFile signalFile;
  QCPGraph* graph;
  QCustomPlot* plot;
  int chunkSize;
  int currentChunk;

 public:
  explicit SignalData(int chunkSize = 5120);
  void setup(QCustomPlot* qCustomPlot);
  virtual void loadChunk(int chunkIndex) = 0;

 public slots:
  void updateData();
};

class Complex64SignalData : public SignalData {
  Q_OBJECT
 public:
  Complex64SignalData(const QString& signalPath, int sampleRate);
  ~Complex64SignalData() override;

  void loadChunk(int chunkIndex) override;

 private:
  QVector<double> realData;
  QVector<double> imagData;
  QVector<double> timeData;
};

#endif  // QSIGVIEWER_SIGNALDATA_H
