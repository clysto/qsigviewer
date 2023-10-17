#include <filedialog.h>
#include <mainwindow.h>
#include <spectrumwin.h>
#include <timeticker.h>
#include <ui_MainWindow.h>
#include <utils.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      maxChunksVisible(5),
      chunkSize(10240),
      timeTicker(new TimeTicker(1000)),
      signalData(nullptr) {
  ui->setupUi(this);
  Utils::applyPlotDarkTheme(ui->sigview, true);

  ui->sigview->xAxis->setLabel("Time (s)");
  ui->sigview->yAxis->setLabel("Amplitude");
  ui->sigview->setInteractions(QCP::iRangeDrag);
  ui->sigview->yAxis->setRange(-1, 1);
  ui->sigview->xAxis->setRange(0, maxChunksVisible * chunkSize);
  ui->sigview->xAxis->setTicker(timeTicker);

  auto *hline = new QCPItemLine(ui->sigview);
  hline->start->setType(QCPItemPosition::ptAxisRectRatio);
  hline->start->setCoords(0, 0.5);
  hline->end->setType(QCPItemPosition::ptAxisRectRatio);
  hline->end->setCoords(1, 0.5);
  hline->setPen(QPen(Qt::white));
  auto *vline = new QCPItemLine(ui->sigview);
  vline->start->setType(QCPItemPosition::ptAxisRectRatio);
  vline->start->setCoords(0.5, 0);
  vline->end->setType(QCPItemPosition::ptAxisRectRatio);
  vline->end->setCoords(0.5, 1);
  vline->setPen(QPen(Qt::white));
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

void MainWindow::handleSigOpen(const SignalInfo &signalInfo) {
  delete signalData;
  if (signalInfo.type == SignalInfo::Type::Float32) {
    signalData = new Float32SignalData(ui->sigview, signalInfo, chunkSize, 100);
  } else if (signalInfo.type == SignalInfo::Type::Complex64) {
    signalData = new Complex64SignalData(ui->sigview, signalInfo, chunkSize, 100);
  }
  timeTicker->setSampleRate(signalInfo.sampleRate);
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

void MainWindow::handlePsdOpen() {
  QVector<double> frequency;
  QVector<double> psd;
  signalData->currentPSD(frequency, psd);
  auto *win = new SpectrumWin(this, frequency, psd);
  win->show();
}
