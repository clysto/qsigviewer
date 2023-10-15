#include <filedialog.h>
#include <mainwindow.h>
#include <ui_MainWindow.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->sigview->setBackground(QBrush(QColor(0, 0, 0)));
  ui->sigview->xAxis->setBasePen(QPen(Qt::white, 1));
  ui->sigview->yAxis->setBasePen(QPen(Qt::white, 1));
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
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::handleMenuAction(QAction *action) {
  if (action == ui->actionOpen) {
    FileDialog fileDialog;
    connect(&fileDialog, SIGNAL(fileInfo(QString, int)), this, SLOT(handleSigOpen(QString, int)));
    fileDialog.exec();
  }
}

void MainWindow::handleSigOpen(const QString &filePath, int sampleRate) {
  SignalData signalData(sampleRate, filePath);
  allSignals.push_back(signalData);
  ui->sigview->clearGraphs();
  ui->sigview->addGraph();
  ui->sigview->graph(0)->setData(signalData.getTime(), signalData.getData());
  ui->sigview->xAxis->setRange(0, signalData.getTime()[1000]);
  ui->sigview->yAxis->setRange(-1, 1);
  ui->sigview->graph(0)->setPen(QPen(QColor(255, 255, 0)));
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
