#ifndef QSIGVIEWER_TIMETICKER_H
#define QSIGVIEWER_TIMETICKER_H

#include <qcustomplot.h>

class TimeTicker : public QCPAxisTicker {
 public:
  explicit TimeTicker(int sampleRate);

  QString getTickLabel(double tick, const QLocale& locale, QChar formatChar, int precision) override;

  void setSampleRate(int sampleRate);

  int getSampleRate() const;

 private:
  int sampleRate;
};

#endif  // QSIGVIEWER_TIMETICKER_H
