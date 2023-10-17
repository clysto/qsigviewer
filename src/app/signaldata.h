#ifndef QSIGVIEWER_SIGNALDATA_H
#define QSIGVIEWER_SIGNALDATA_H

#include <qcustomplot.h>
#include <utils.h>

#include <QFile>
#include <QObject>
#include <QString>
#include <QVector>

struct SignalInfo {
  enum Type { Complex64, Float32 } type;
  QString path;
  int sampleRate;
  int centerFrequency;
};

class SignalData : public QObject {
  Q_OBJECT

 protected:
  QCustomPlot* plot;
  int chunkSize;
  int maxChunksCached;
  QQueue<int> loadedChunks;
  SignalInfo signalInfo;

 public:
  explicit SignalData(QCustomPlot* plot, SignalInfo signalInfo, int chunkSize = 5120, int maxChunksVisible = 1);

  virtual void loadChunk(int chunkIndex) = 0;
  virtual void unloadChunk(int chunkIndex) = 0;
  virtual void currentPSD(QVector<double>& frequency, QVector<double>& psd) = 0;

 public slots:
  void updateData();
};

class Complex64SignalData : public SignalData {
  Q_OBJECT

 public:
  Complex64SignalData(QCustomPlot* plot, SignalInfo signalInfo, int chunkSize, int maxChunksCached);
  ~Complex64SignalData() override;

  void loadChunk(int chunkIndex) override;
  void unloadChunk(int chunkIndex) override;
  void currentPSD(QVector<double>& frequency, QVector<double>& psd) override;

 private:
  QFile signalFile;
  QCPGraph* graphI;
  QCPGraph* graphQ;
  QSharedPointer<QCPDataContainer<QCPGraphData>> graphDataI;
  QSharedPointer<QCPDataContainer<QCPGraphData>> graphDataQ;
};

class Float32SignalData : public SignalData {
  Q_OBJECT

 public:
  Float32SignalData(QCustomPlot* plot, SignalInfo signalInfo, int chunkSize, int maxChunksCached);
  ~Float32SignalData() override;

  void loadChunk(int chunkIndex) override;
  void unloadChunk(int chunkIndex) override;
  void currentPSD(QVector<double>& frequency, QVector<double>& psd) override;

 private:
  QFile signalFile;
  QCPGraph* graph;
  QSharedPointer<QCPDataContainer<QCPGraphData>> graphData;
};

#endif  // QSIGVIEWER_SIGNALDATA_H
