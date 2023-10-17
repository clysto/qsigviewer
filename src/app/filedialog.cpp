#include <filedialog.h>
#include <ui_FileDialog.h>

#include <QFileDialog>
#include <QMessageBox>

FileDialog::FileDialog(QWidget *parent) : QDialog(parent), ui(new Ui::FileDialog) {
  ui->setupUi(this);
  ui->typeComboBox->addItem("Float32");
  ui->typeComboBox->addItem("Complex64");
}

FileDialog::~FileDialog() { delete ui; }

void FileDialog::openFileSelector() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("All Files (*)"));
  if (!fileName.isEmpty()) {
    ui->filePathLineEdit->setText(fileName);
  }
}

void FileDialog::done() {
  if (ui->filePathLineEdit->text().isEmpty()) {
    QMessageBox::warning(this, "Warning", "Please select a file!");
    return;
  }
  SignalInfo signalInfo;
        signalInfo.path = ui->filePathLineEdit->text();
        signalInfo.sampleRate = ui->sampleRateSpinBox->value();
        signalInfo.type = ui->typeComboBox->currentIndex() == 0 ? SignalInfo::Float32 : SignalInfo::Complex64;
        signalInfo.centerFrequency = ui->centerFreqSpinBox->value();
  emit fileInfo(signalInfo);
  accept();
}