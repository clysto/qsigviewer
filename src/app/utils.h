//
// Created by maoyachen on 2023/10/16.
//

#ifndef QSIGVIEWER_UTILS_H
#define QSIGVIEWER_UTILS_H

#include <fftw3.h>
#include <qcustomplot.h>

#include <QString>

class Utils {
 public:
  static QString humanReadableTime(int sampleIndex, int sampleRate);
  static void psd(const QVector<double>& real, const QVector<double>& imag, int nfft, double fs, double fc,
           QVector<double>& frequency, QVector<double>& psd);
  static void applyPlotDarkTheme(QCustomPlot* plot, bool subGridVisible=false);
};

#endif  // QSIGVIEWER_UTILS_H
