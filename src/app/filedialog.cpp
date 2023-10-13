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

[[maybe_unused]] void FileDialog::openFileSelector() {
  QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty()) {
    ui->filePathLineEdit->setText(fileName);
  }
}

void FileDialog::done() {
  if (ui->filePathLineEdit->text().isEmpty()) {
    QMessageBox::warning(this, "Warning", "Please select a file!");
    return;
  }
  emit fileInfo(ui->filePathLineEdit->text(), ui->sampleRateSpinBox->value());
  accept();
}