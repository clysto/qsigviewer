#include <signaldata.h>

#include <QFile>
#include <QVector>

Complex64SignalData::Complex64SignalData(const QString &signalPath, int sampleRate, int chunkSize, int maxChunksCached)
    : SignalData(chunkSize, maxChunksCached),
      graphI(nullptr),
      graphQ(nullptr),
      graphDataI(new QCPDataContainer<QCPGraphData>),
      graphDataQ(new QCPDataContainer<QCPGraphData>) {
  this->sampleRate = sampleRate;
  this->signalFile.setFileName(signalPath);
  if (!signalFile.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("Cannot open file!");
  }
}

Complex64SignalData::~Complex64SignalData() { signalFile.close(); }

void Complex64SignalData::loadChunk(int chunkIndex) {
  auto chunkCount = signalFile.size() / (chunkSize * 8);
  if (chunkIndex < 0 || chunkIndex > chunkCount) {
    return;
  }

  QVector<QCPGraphData> tempDataI(chunkSize);
  QVector<QCPGraphData> tempDataQ(chunkSize);
  QVector<QCPGraphData>::iterator itI = tempDataI.begin();
  QVector<QCPGraphData>::iterator itQ = tempDataQ.begin();

  signalFile.seek(chunkIndex * chunkSize * 8);
  float temp;
  qint64 n;

  qDebug() << "reading chunk" << chunkIndex << "of" << chunkCount << "chunks";

  for (int i = 0; i < chunkSize; ++i) {
    itI->key = i + chunkIndex * chunkSize;
    itQ->key = i + chunkIndex * chunkSize;
    n = signalFile.read(reinterpret_cast<char *>(&temp), sizeof(float));
    if (n == 0) {
      break;
    }
    itI->value = temp;
    n = signalFile.read(reinterpret_cast<char *>(&temp), sizeof(float));
    if (n == 0) {
      break;
    }
    itQ->value = temp;
    ++itI;
    ++itQ;
  }

  graphDataI->add(tempDataI);
  graphDataQ->add(tempDataQ);
}

void Complex64SignalData::setup(QCustomPlot *qCustomPlot) {
  plot = qCustomPlot;
  graphI = plot->addGraph();
  graphQ = plot->addGraph();
  graphI->setData(graphDataI);
  graphQ->setData(graphDataQ);
  graphI->setPen(QPen(Qt::yellow));
  graphQ->setPen(QPen(Qt::green));
  QObject::connect(plot, QOverload<>::of(&QCustomPlot::beforeReplot), this, &SignalData::updateData);
}

void Complex64SignalData::unloadChunk(int chunkIndex) {
  int from = chunkIndex * chunkSize;
  int to = from + chunkSize;
  graphDataI->remove(from, to);
  graphDataQ->remove(from, to);
}

void SignalData::updateData() {
  QCPRange range = plot->xAxis->range();
  int startChunk = static_cast<int>(range.lower / chunkSize);
  int endChunk = static_cast<int>(range.upper / chunkSize);
  for (int chunkIndex = startChunk; chunkIndex <= endChunk; ++chunkIndex) {
    if (!loadedChunks.contains(chunkIndex)) {
      loadChunk(chunkIndex);
      loadedChunks.enqueue(chunkIndex);
      while (loadedChunks.size() > maxChunksCached + 1) {
        int chunk = loadedChunks.dequeue();
        unloadChunk(chunk);
      }
    }
  }
}

SignalData::SignalData(int chunkSize, int maxChunksCached)
    : chunkSize(chunkSize), maxChunksCached(maxChunksCached), plot(nullptr), sampleRate(1) {}
