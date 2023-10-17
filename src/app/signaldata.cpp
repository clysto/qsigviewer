#include <signaldata.h>

#include <QFile>
#include <QVector>

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

SignalData::SignalData(QCustomPlot *plot, SignalInfo signalInfo, int chunkSize, int maxChunksCached)
    : chunkSize(chunkSize), maxChunksCached(maxChunksCached), plot(plot), signalInfo(signalInfo) {
  QObject::connect(plot, QOverload<>::of(&QCustomPlot::beforeReplot), this, &SignalData::updateData);
}

Complex64SignalData::Complex64SignalData(QCustomPlot *plot, SignalInfo signalInfo, int chunkSize, int maxChunksCached)

    : SignalData(plot, signalInfo, chunkSize, maxChunksCached),
      graphI(nullptr),
      graphQ(nullptr),
      graphDataI(new QCPDataContainer<QCPGraphData>),
      graphDataQ(new QCPDataContainer<QCPGraphData>) {
  this->signalFile.setFileName(signalInfo.path);
  if (!signalFile.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("Cannot open file!");
  }
  graphI = plot->addGraph();
  graphQ = plot->addGraph();
  graphI->setData(graphDataI);
  graphQ->setData(graphDataQ);
  graphI->setPen(QPen(Qt::yellow));
  graphQ->setPen(QPen(Qt::green));
}

Complex64SignalData::~Complex64SignalData() {
  signalFile.close();
  plot->removeGraph(graphI);
  plot->removeGraph(graphQ);
}

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

void Complex64SignalData::unloadChunk(int chunkIndex) {
  int from = chunkIndex * chunkSize;
  int to = from + chunkSize;
  graphDataI->remove(from, to);
  graphDataQ->remove(from, to);
}

void Complex64SignalData::currentPSD(QVector<double> &frequency, QVector<double> &psd) {
  int from = plot->xAxis->range().lower;
  int to = plot->xAxis->range().upper;
  QVector<double> real(to - from);
  QVector<double> imag(to - from);
  for (int i = from; i < to; ++i) {
    real[i - from] = graphDataI->at(i)->value;
    imag[i - from] = graphDataQ->at(i)->value;
  }
  Utils::psd(real, imag, 1024, signalInfo.sampleRate, signalInfo.centerFrequency, frequency, psd);
}

Float32SignalData::Float32SignalData(QCustomPlot *plot, SignalInfo signalInfo, int chunkSize, int maxChunksCached)
    : SignalData(plot, signalInfo, chunkSize, maxChunksCached),
      graph(nullptr),
      graphData(new QCPDataContainer<QCPGraphData>) {
  this->signalFile.setFileName(signalInfo.path);
  if (!signalFile.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("Cannot open file!");
  }
  graph = plot->addGraph();
  graph->setData(graphData);
  graph->setPen(QPen(Qt::yellow));
}

Float32SignalData::~Float32SignalData() {
  signalFile.close();
  plot->removeGraph(graph);
}

void Float32SignalData::loadChunk(int chunkIndex) {
  auto chunkCount = signalFile.size() / (chunkSize * 4);
  if (chunkIndex < 0 || chunkIndex > chunkCount) {
    return;
  }

  QVector<QCPGraphData> tempData(chunkSize);
  QVector<QCPGraphData>::iterator it = tempData.begin();

  signalFile.seek(chunkIndex * chunkSize * 4);
  float temp;
  qint64 n;

  qDebug() << "reading chunk" << chunkIndex << "of" << chunkCount << "chunks";

  for (int i = 0; i < chunkSize; ++i) {
    it->key = i + chunkIndex * chunkSize;
    n = signalFile.read(reinterpret_cast<char *>(&temp), sizeof(float));
    if (n == 0) {
      break;
    }
    it->value = temp;
    ++it;
  }

  graphData->add(tempData);
}

void Float32SignalData::unloadChunk(int chunkIndex) {
  int from = chunkIndex * chunkSize;
  int to = from + chunkSize;
  graphData->remove(from, to);
}

void Float32SignalData::currentPSD(QVector<double> &frequency, QVector<double> &psd) {}