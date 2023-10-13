#include <signaldata.h>

#include <QFile>
#include <QVector>

SignalData::SignalData(int sampleRate, const QString &path) : sampleRate(sampleRate) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("Cannot open file!");
  }
  auto size = file.size();
  data = QVector<double>(size / 4);
  time = QVector<double>(data.size());
  float temp;
  for (int i = 0; i < data.size(); ++i) {
    file.read(reinterpret_cast<char *>(&temp), sizeof(float));
    data[i] = temp;
    time[i] = static_cast<float>(i) / static_cast<float>(this->sampleRate);
  }
}

const QVector<double> &SignalData::getData() { return data; }

const QVector<double> &SignalData::getTime() { return time; }
