#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// --- COLORS FOR SERIES ---
static const QVector<QColor> seriesColors = {
    QColor("#1f77b4"),
    QColor("#ff7f0e"),
    QColor("#2ca02c"),
    QColor("#d62728"),
    QColor("#9467bd"),
    QColor("#8c564b"),
    QColor("#e377c2")
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->chartView->setRubberBand(QChartView::RectangleRubberBand);
    ui->chartView->setDragMode(QGraphicsView::ScrollHandDrag);
    setWindowTitle("Diode characteristic plotter");
    resize(1000, 600);

    // ----- LOG DIRECTORY -----
    QString baseDir =
#ifdef QT_NO_DEBUG
        QCoreApplication::applicationDirPath();
#else
        QDir::currentPath();
#endif

    logDir = baseDir + "/Log";
    QDir().mkpath(logDir);
    outputPath = logDir + "/output_0.txt";

    // ----- CHART -----
    chart = new QChart();
    chart->setTitle("Diode I-V characteristic");
    chart->legend()->setVisible(true);
    chart->legend()->setFont(QFont("Arial", 12));

    auto *axisX = new QValueAxis();
    auto *axisY = new QValueAxis();
    axisX->setTitleText("Voltage, V");
    axisY->setTitleText("Current, A");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    // ----- BUTTONS -----
    connect(ui->buttonCom,  &QPushButton::clicked, this, &MainWindow::enterCom);
    connect(ui->buttonConv, &QPushButton::clicked, this, &MainWindow::startConversion);
    connect(ui->buttonPlot, &QPushButton::clicked, this, &MainWindow::plot);
    connect(ui->buttonClearLog, &QPushButton::clicked, this, &MainWindow::clearLog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enterCom()
{
    QString port = ui->portInput->text().trimmed();
    if (port.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Enter COM port!");
        return;
    }

    serial.setPortName(port);
    serial.setBaudRate(9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!serial.open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, "Error", "Cannot open COM port!");
        return;
    }

    ui->buttonCom->setEnabled(false);
}

void MainWindow::startConversion()
{
    try {
        serial.clear(QSerialPort::AllDirections);
        serial.flush();

        serial.write("\xAA", 1);
        serial.flush();

        QFile f(outputPath);
        f.open(QIODevice::Append | QIODevice::Text);
        QTextStream out(&f);

        while (true) {
            if (!serial.waitForReadyRead(2000)) {
                QMessageBox::warning(this, "Warning", "Timeout waiting for data");
                break;
            }
            QByteArray b = serial.read(1);
            if (b.isEmpty()) continue;

            if (b[0] == char(0xC0))
                break;

            if (b[0] != char(0xAA))
                continue;

            QByteArray packet;
            while (packet.size() < 9) {
                if (!serial.waitForReadyRead(2000)) {
                    qDebug() << "Timeout while reading packet";
                    break;
                }
                packet += serial.read(9 - packet.size());
            }
            if (packet.size() != 9) continue;

            QByteArray full = b + packet.left(8);
            quint8 crc = packet[8];

            quint8 calc = 0;
            for (auto c : full) calc ^= c;

            if (calc != crc)
                continue;

            float x, y;
            memcpy(&x, packet.data(), 4);
            memcpy(&y, packet.data() + 4, 4);

            out << QString::number(x, 'f', 6) << "\t"
                << QString::number(y, 'f', 6) << "\n";
        }

        f.close();

        QMessageBox::information(this, "Info", "Conversion completed!");

        conversionNumber++;
        outputPath = logDir + QString("/output_%1.txt").arg(conversionNumber);

    } catch (...) {
        QMessageBox::critical(this, "Error", "Conversion failed!");
    }
}

void MainWindow::plot()
{
    chart->removeAllSeries();

    for (int i = 0; i < conversionNumber; i++) {
        QString filename = logDir + QString("/output_%1.txt").arg(i);

        QFile f(filename);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        QList<QPointF> points;
        QTextStream in(&f);

        while (!in.atEnd()) {
            double x, y;
            in >> x >> y;
            points.append(QPointF(x, y));
        }

        std::sort(points.begin(), points.end(), [](const QPointF &a, const QPointF &b){
                return a.x() < b.x();
            }
        );

        QLineSeries *s = new QLineSeries();
        s->setName(QString("output_%1").arg(i));
        s->replace(points);

        QPen pen(seriesColors[i % seriesColors.size()]);
        pen.setWidth(2);
        s->setPen(pen);

        chart->addSeries(s);
        s->attachAxis(chart->axisX());
        s->attachAxis(chart->axisY());

        double minX =  std::numeric_limits<double>::max();
        double maxX = -std::numeric_limits<double>::max();
        double minY =  std::numeric_limits<double>::max();
        double maxY = -std::numeric_limits<double>::max();

        for (auto *s : chart->series()) {
            auto *line = qobject_cast<QLineSeries*>(s);
            if (!line) continue;

            for (const QPointF &p : line->points()) {
                minX = std::min(minX, p.x());
                maxX = std::max(maxX, p.x());
                minY = std::min(minY, p.y());
                maxY = std::max(maxY, p.y());
            }
        }

        chart->axisX()->setRange(minX, maxX);
        chart->axisY()->setRange(minY, maxY);
    }
}

void MainWindow::clearLog()
{
    conversionNumber = 0;

    QDir dir(logDir);
    dir.removeRecursively();
    QDir().mkdir(logDir);

    outputPath = logDir + "/output_0.txt";

    QMessageBox::information(this, "Info", "Log cleared!");
}
