#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_GoBtn_clicked(bool);
    void on_ReconstructBtn_toggled(bool checked);
    void on_PBKDF2_toggled(bool checked);
    void on_Rand128_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<uint8_t> readHexEnthropy() const;
    void distributeSecret(const std::vector<uint8_t> & rawenthropy) const;
    void mergeSecret() const;
};

#endif // MAINWINDOW_H
