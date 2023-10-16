#include <signaldata.h>

#include <QFile>
#include <QVector>

Complex64SignalData::Complex64SignalData(const QString &signalPath, int sampleRate)
    : SignalData(),
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

  for (int i = 0; i < chunkSize; ++i) {
    itI->key = i + chunkIndex * chunkSize;
    itQ->key = i + chunkIndex * chunkSize;
    signalFile.read(reinterpret_cast<char *>(&temp), sizeof(float));
    itI->value = temp;
    signalFile.read(reinterpret_cast<char *>(&temp), sizeof(float));
    itQ->value = temp;
    ++itI;
    ++itQ;
  }

  graphDataI->add(tempDataI);
  graphDataQ->add(tempDataQ);

  loadedChunks.enqueue(chunkIndex);

  while (loadedChunks.size() > maxChunksVisible + 1) {
    int chunk = loadedChunks.dequeue();
    int from = chunk * chunkSize;
    int to = from + chunkSize;
    graphDataI->remove(from, to);
    graphDataQ->remove(from, to);
  }
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

void SignalData::updateData() {
  QCPRange range = plot->xAxis->range();
  double start = range.lower;
  double end = range.upper;
  int chunk1 = static_cast<int>(start / chunkSize);
  int chunk2 = static_cast<int>(end / chunkSize);
  for (int i = chunk1; i <= chunk2; ++i) {
    if (!loadedChunks.contains(i)) {
      loadChunk(i);
    }
  }
}

SignalData::SignalData(int chunkSize, int maxChunksVisible)
    : chunkSize(chunkSize), maxChunksVisible(maxChunksVisible), plot(nullptr), sampleRate(1) {}
