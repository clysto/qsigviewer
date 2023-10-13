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
  ui->sigview->xAxis->setLabel("Time");
  ui->sigview->yAxis->setLabel("Amplitude");
  ui->sigview->xAxis->setLabelColor(Qt::white);
  ui->sigview->yAxis->setLabelColor(Qt::white);
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
  ui->sigview->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
  ui->sigview->replot();
}

void MainWindow::handleOffsetChange(int value) { qDebug() << value; }

void MainWindow::handleScaleChange(int value) { qDebug() << value; }
