#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Voice Studio");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rbPlay_clicked(bool checked)
{
    if (checked) {

    }
    else {
    }
}
