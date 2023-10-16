//
// Created by maoyachen on 2023/10/16.
//

#include <timeticker.h>

QString TimeTicker::getTickLabel(double tick, const QLocale& locale, QChar formatChar, int precision) {
  double timeInSeconds = tick / sampleRate;
  return QCPAxisTicker::getTickLabel(timeInSeconds, locale, formatChar, precision);
}

void TimeTicker::setSampleRate(int sampleRate) { this->sampleRate = sampleRate; }

int TimeTicker::getSampleRate() const { return sampleRate; }

TimeTicker::TimeTicker(int sampleRate) : sampleRate(sampleRate) {}
