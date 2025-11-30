#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void enterCom();
    void startConversion();
    void plot();
    void clearLog();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;

    int conversionNumber = 0;
    QString logDir;
    QString outputPath;

    QChart *chart;
};
#endif // MAINWINDOW_H
