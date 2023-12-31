#include <spectrumwin.h>
#include <ui_SpectrumWin.h>
#include <utils.h>

SpectrumWin::SpectrumWin(QWidget* parent, QVector<double> frequency, QVector<double> psd)
    : QDialog(parent), ui(new Ui::SpectrumWin) {
  ui->setupUi(this);
  Utils::applyPlotDarkTheme(ui->sigview);
  ui->sigview->xAxis->setLabel("Frequency (Hz)");
  ui->sigview->yAxis->setLabel("PSD");
  ui->sigview->xAxis->setRange(frequency.first(), frequency.last());
  ui->sigview->addGraph();

  double min = 9999999, max = -9999999;
  for (auto& v : psd) {
    if (v != -std::numeric_limits<double>::infinity()) {
      if (v < min) min = v;
      if (v > max) max = v;
    }
  }

  ui->sigview->yAxis->setRange(min, max);
  ui->sigview->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

  ui->sigview->graph(0)->setData(frequency, psd);
  ui->sigview->graph(0)->setPen(QPen(Qt::yellow));

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
      auto text = QString("x: %1Hz\ny: %2").arg(x).arg(y);
      QToolTip::showText(QCursor::pos(), text, ui->sigview);
    }
  }
}
