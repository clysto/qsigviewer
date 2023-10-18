#include <utils.h>

#include <QVector>

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

void Utils::psd(const QVector<double>& real, const QVector<double>& imag, int nfft, double fs, double fc,
                QVector<double>& freqs, QVector<double>& pxx) {
  int noverlap = nfft / 2;  // 50% overlap
  int nsegments = (real.size() - noverlap) / (nfft - noverlap);
  int nfreqs = nfft + 1;

  freqs.resize(nfreqs);
  pxx.resize(nfreqs);

  // Initialize PSD vector to zero
  for (int i = 0; i < nfreqs; ++i) {
    pxx[i] = 0.0;
  }

  fftw_plan plan;
  fftw_complex* fft_out = fftw_alloc_complex(nfft);

  for (int seg = 0; seg < nsegments; ++seg) {
    int offset = seg * (nfft - noverlap);
    // Windowing (Hanning window)
    for (int i = 0; i < nfft; ++i) {
      double window = 0.5 * (1 - std::cos(2 * M_PI * i / (nfft - 1)));
      fft_out[i][0] = real[offset + i] * window;
      fft_out[i][1] = imag[offset + i] * window;
    }
    // FFT
    plan = fftw_plan_dft_1d(nfft, fft_out, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    // Accumulate power
    for (int i = 0; i < nfreqs / 2; ++i) {  // positive frequencies
      pxx[nfreqs / 2 + i] += (fft_out[i][0] * fft_out[i][0] + fft_out[i][1] * fft_out[i][1]) / nsegments;
    }
    for (int i = nfreqs / 2; i < nfreqs; ++i) {  // negative frequencies
      pxx[i - nfreqs / 2] += (fft_out[i][0] * fft_out[i][0] + fft_out[i][1] * fft_out[i][1]) / nsegments;
    }
  }

  // Normalize and convert to dB
  for (int i = 0; i < nfreqs; ++i) {
    pxx[i] = 10 * std::log10(pxx[i]);
    freqs[i] = (i - nfreqs / 2) * fs / nfft + fc;
  }

  fftw_destroy_plan(plan);
  fftw_free(fft_out);
}

void Utils::applyPlotDarkTheme(QCustomPlot* plot, bool subGridVisible) {
  plot->setBackground(QBrush(QColor(0, 0, 0)));
  plot->xAxis->setBasePen(QPen(Qt::white, 1));
  plot->xAxis2->setBasePen(QPen(Qt::white, 1));
  plot->yAxis->setBasePen(QPen(Qt::white, 1));
  plot->yAxis2->setBasePen(QPen(Qt::white, 1));
  plot->xAxis->setTickPen(QPen(Qt::white, 1));
  plot->yAxis->setTickPen(QPen(Qt::white, 1));
  plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
  plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
  plot->xAxis->setTickLabelColor(Qt::white);
  plot->yAxis->setTickLabelColor(Qt::white);
  plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  if (subGridVisible) {
    plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plot->xAxis->grid()->setSubGridVisible(true);
    plot->yAxis->grid()->setSubGridVisible(true);
  }
  plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
  plot->xAxis->setLabelColor(Qt::white);
  plot->yAxis->setLabelColor(Qt::white);
  plot->xAxis2->setVisible(true);
  plot->xAxis2->setTicks(false);
  plot->xAxis2->setTickLabels(false);
  plot->yAxis2->setVisible(true);
  plot->yAxis2->setTicks(false);
  plot->yAxis2->setTickLabels(false);
}
