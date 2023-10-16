//
// Created by maoyachen on 2023/10/16.
//

#include <utils.h>

QString Utils::humanReadableTime(int sampleIndex, int sampleRate) {
  double timeInSeconds = static_cast<double>(sampleIndex) / sampleRate;
  if (timeInSeconds >= 1.0) {
    return QString::number(timeInSeconds, 'f', 1) + "s";
  }
  if (timeInSeconds >= 0.001) {
    return QString::number(timeInSeconds * 1000.0, 'f', 1) + "ms";
  }
  if (timeInSeconds >= 0.000001) {
    return QString::number(timeInSeconds * 1000000.0, 'f', 1) + "µs";
  }
  if (timeInSeconds >= 0.000000001) {
    return QString::number(timeInSeconds * 1000000000.0, 'f', 1) + "ns";
  }
  return QString::number(timeInSeconds * 1000000000000.0, 'f', 1) + "ps";
}
