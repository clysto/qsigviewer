#ifndef QSIGVIEWER_SIGNALDATA_H
#define QSIGVIEWER_SIGNALDATA_H

#include <qcustomplot.h>

#include <QFile>
#include <QObject>
#include <QString>
#include <QVector>

class SignalData : public QObject {
  Q_OBJECT

 protected:
  int sampleRate;
  QFile signalFile;
  QCustomPlot* plot;
  int chunkSize;
  int maxChunksVisible;
  QQueue<int> loadedChunks;

 public:
  explicit SignalData(int chunkSize = 5120, int maxChunksVisible = 1);
  virtual void setup(QCustomPlot* qCustomPlot) = 0;
  virtual void loadChunk(int chunkIndex) = 0;

 public slots:
  void updateData();
};

class Complex64SignalData : public SignalData {
  Q_OBJECT
 public:
  Complex64SignalData(const QString& signalPath, int sampleRate);
  ~Complex64SignalData() override;

  void setup(QCustomPlot* qCustomPlot) override;
  void loadChunk(int chunkIndex) override;

 private:
  QCPGraph* graphI;
  QCPGraph* graphQ;
  QSharedPointer<QCPDataContainer<QCPGraphData>> graphDataI;
  QSharedPointer<QCPDataContainer<QCPGraphData>> graphDataQ;
};

#endif  // QSIGVIEWER_SIGNALDATA_H
