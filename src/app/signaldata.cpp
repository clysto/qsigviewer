#include <signaldata.h>

#include <QFile>
#include <QVector>

Complex64SignalData::Complex64SignalData(const QString &signalPath, int sampleRate) : SignalData() {
  this->sampleRate = sampleRate;
  this->signalFile.setFileName(signalPath);
  if (!signalFile.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("Cannot open file!");
  }
  realData.reserve(chunkSize);
  imagData.reserve(chunkSize);
  timeData.reserve(chunkSize);
}

Complex64SignalData::~Complex64SignalData() { signalFile.close(); }

void Complex64SignalData::loadChunk(int chunkIndex) {
  auto chunkCount = signalFile.size() / (chunkSize * 8);
  if (chunkIndex < 0 || chunkIndex >= chunkCount) {
    throw std::runtime_error("Invalid chunk index!");
  }
  realData.clear();
  imagData.clear();
  timeData.clear();
  signalFile.seek(chunkIndex * chunkSize * 8);
  float temp;
  for (int i = 0; i < chunkSize; ++i) {
    signalFile.read(reinterpret_cast<char *>(&temp), sizeof(float));
    realData.push_back(temp);
    signalFile.read(reinterpret_cast<char *>(&temp), sizeof(float));
    imagData.push_back(temp);

    timeData.push_back(static_cast<float>(i) / static_cast<float>(sampleRate));
  }
  currentChunk = chunkIndex;
}

void SignalData::setup(QCustomPlot *qCustomPlot) {
  this->plot = qCustomPlot;
  graph = plot->addGraph();
  connect(plot, &QCustomPlot::beforeReplot, this, &SignalData::updateData);
}

void SignalData::updateData() {
  QCPRange range = plot->xAxis->range();
  double start = range.lower;
  int chunk = static_cast<int>(start * sampleRate / chunkSize);
  if (chunk != currentChunk) {
    loadChunk(chunk);
  }
}

SignalData::SignalData(int chunkSize)
    : chunkSize(chunkSize), graph(nullptr), plot(nullptr), sampleRate(1), currentChunk(-1) {}
