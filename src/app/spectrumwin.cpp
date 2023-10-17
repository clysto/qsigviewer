//
// Created by maoyachen on 2023/10/17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SpectrumWin.h" resolved

#include "spectrumwin.h"

#include "ui_SpectrumWin.h"
#include "utils.h"

SpectrumWin::SpectrumWin(QWidget* parent, QVector<double> frequency, QVector<double> psd)
    : QDialog(parent), ui(new Ui::SpectrumWin) {
  ui->setupUi(this);
  Utils::applyPlotDarkTheme(ui->sigview);
  ui->sigview->xAxis->setLabel("Frequency (Hz)");
  ui->sigview->yAxis->setLabel("PSD");
  ui->sigview->addGraph();
  ui->sigview->graph(0)->setData(frequency, psd);
  ui->sigview->graph(0)->setPen(QPen(Qt::yellow));
  ui->sigview->xAxis->setRange(frequency.first(), frequency.last());
  auto minmax = std::minmax_element(psd.constBegin(), psd.constEnd());
  ui->sigview->yAxis->setRange(*minmax.first, *minmax.second);
  ui->sigview->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->sigview->replot();
  connect(ui->sigview, &QCustomPlot::plottableClick, this, &SpectrumWin::onPlottableClick);
}

SpectrumWin::~SpectrumWin() { delete ui; }

void SpectrumWin::onPlottableClick(QCPAbstractPlottable* plottable, int dataIndex) {
  if (plottable) {
    auto graph = qobject_cast<QCPGraph*>(plottable);
    if (graph) {
      auto data = graph->data()->at(dataIndex);
      auto x = data->key;
      auto y = data->value;
      auto text = QString("x: %1\ny: %2").arg(x).arg(y);
      QToolTip::showText(QCursor::pos(), text, ui->sigview);
    }
  }
}
