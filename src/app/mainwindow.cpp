#include <filedialog.h>
#include <mainwindow.h>
#include <timeticker.h>
#include <ui_MainWindow.h>
#include <utils.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      maxChunksVisible(5),
      chunkSize(10240),
      timeTicker(new TimeTicker(1000)) {
  ui->setupUi(this);
  ui->sigview->setBackground(QBrush(QColor(0, 0, 0)));
  ui->sigview->xAxis->setBasePen(QPen(Qt::white, 1));
  ui->sigview->xAxis2->setBasePen(QPen(Qt::white, 1));
  ui->sigview->yAxis->setBasePen(QPen(Qt::white, 1));
  ui->sigview->yAxis2->setBasePen(QPen(Qt::white, 1));
  ui->sigview->xAxis->setTickPen(QPen(Qt::white, 1));
  ui->sigview->yAxis->setTickPen(QPen(Qt::white, 1));
  ui->sigview->xAxis->setSubTickPen(QPen(Qt::white, 1));
  ui->sigview->yAxis->setSubTickPen(QPen(Qt::white, 1));
  ui->sigview->xAxis->setTickLabelColor(Qt::white);
  ui->sigview->yAxis->setTickLabelColor(Qt::white);
  ui->sigview->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  ui->sigview->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  ui->sigview->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  ui->sigview->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  ui->sigview->xAxis->grid()->setSubGridVisible(true);
  ui->sigview->yAxis->grid()->setSubGridVisible(true);
  ui->sigview->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  ui->sigview->yAxis->grid()->setZeroLinePen(Qt::NoPen);
  ui->sigview->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
  ui->sigview->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
  ui->sigview->xAxis->setLabel("Time (s)");
  ui->sigview->yAxis->setLabel("Amplitude");
  ui->sigview->xAxis->setLabelColor(Qt::white);
  ui->sigview->yAxis->setLabelColor(Qt::white);
  ui->sigview->setInteractions(QCP::iRangeDrag);
  ui->sigview->yAxis->setRange(-1, 1);
  ui->sigview->xAxis->setRange(0, maxChunksVisible * chunkSize);
  ui->sigview->xAxis->setTicker(timeTicker);
  ui->sigview->xAxis->setUpperEnding(QCPLineEnding::esNone);
  ui->sigview->yAxis->setUpperEnding(QCPLineEnding::esNone);
  ui->sigview->xAxis2->setUpperEnding(QCPLineEnding::esNone);
  ui->sigview->yAxis2->setUpperEnding(QCPLineEnding::esNone);
  ui->sigview->xAxis2->setVisible(true);
  ui->sigview->xAxis2->setTicks(false);
  ui->sigview->xAxis2->setTickLabels(false);
  ui->sigview->yAxis2->setVisible(true);
  ui->sigview->yAxis2->setTicks(false);
  ui->sigview->yAxis2->setTickLabels(false);
  handleTimeChange(0);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::handleMenuAction(QAction *action) {
  if (action == ui->actionOpen) {
    FileDialog fileDialog;
    connect(&fileDialog, &FileDialog::fileInfo, this, &MainWindow::handleSigOpen);
    fileDialog.exec();
  }
}

void MainWindow::handleSigOpen(const QString &filePath, int sampleRate) {
  SignalData *data = new Complex64SignalData(filePath, sampleRate, chunkSize, 100);
  data->setup(ui->sigview);
  allSignals.push_back(data);
  timeTicker->setSampleRate(sampleRate);
  handleTimeChange(0);
  ui->sigview->replot();
}

void MainWindow::handleOffsetChange(int value) {
  value = -value;
  auto center = ui->sigview->yAxis->range().center();
  auto range = ui->sigview->yAxis->range().size();
  ui->sigview->yAxis->setRange(center - range / 2 + range * value / 100, center + range / 2 + range * value / 100);
  ui->sigview->replot();
}

void MainWindow::handleScaleChange(int value) {
  auto center = ui->sigview->yAxis->range().center();
  auto range = ui->sigview->yAxis->range().size();
  double d = range / 2;
  d = d - d * value / 100;
  if (d < 0.000001) {
    d = 0.000001;
  }
  ui->sigview->yAxis->setRange(center - d, center + d);
  ui->sigview->replot();
}

void MainWindow::handleTimeChange(int value) {
  auto center = ui->sigview->xAxis->range().center();
  auto range = ui->sigview->xAxis->range().size();
  double d = range / 2;
  d = d - d * value / 100;
  if (d < 0.000001) {
    d = 0.000001;
  }
  if (2 * d > maxChunksVisible * chunkSize) {
    ui->statusBar->showMessage("Zoomed out to maximum!", 2000);
    return;
  }
  ui->timeLabel->setText(Utils::humanReadableTime(2 * d, timeTicker->getSampleRate()));
  ui->sigview->xAxis->setRange(center - d, center + d);
  ui->sigview->replot();
}

void MainWindow::handleDelayChange(int value) {
  value = -value;
  auto center = ui->sigview->xAxis->range().center();
  auto range = ui->sigview->xAxis->range().size();
  ui->sigview->xAxis->setRange(center - range / 2 + range * value / 100, center + range / 2 + range * value / 100);
  ui->sigview->replot();
}

void MainWindow::handlePrintAction() {
  int width = ui->sigview->width();
  int height = ui->sigview->height();
  QString filename = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") + ".pdf";
  ui->sigview->savePdf(filename, width, height, QCP::epAllowCosmetic, "QSigViewer");
  QMessageBox msgBox;
  msgBox.setText("Saved to " + filename);
  msgBox.exec();
}
