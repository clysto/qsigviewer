#ifndef QSIGVIEWER_FILEDIALOG_H
#define QSIGVIEWER_FILEDIALOG_H

#include <signaldata.h>

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class FileDialog;
}
QT_END_NAMESPACE

class FileDialog : public QDialog {
  Q_OBJECT

 public:
  explicit FileDialog(QWidget *parent = nullptr);

  ~FileDialog() override;

 private:
  Ui::FileDialog *ui;

 public slots:

  void openFileSelector();

  void done();

 signals:

  void fileInfo(SignalInfo signalInfo);
};

#endif  // QSIGVIEWER_FILEDIALOG_H
