#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
//-----------------------
//file manipulation
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QVector>
//-----------------------
#include "backpropagation.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_Shuffle_Data_clicked();

    void on_pushButton_Read_File_clicked();

    void on_horizScrollBar_LearningRate_valueChanged(int value);

   // void on_pushButton_Train_Network_clicked();

    void on_pushButton_Classify_Test_Pattern_clicked();

    void on_pushButton_Train_Network_Max_Epochs_clicked();

    void on_pushButton_Initialise_Network_clicked();

    void on_pushButton_Test_All_Patterns_clicked();

    void on_pushButton_Save_Weights_clicked();

    void on_pushButton_Load_Weights_clicked();

    void on_pushButton_testNetOnTrainingSet_clicked();

    void on_horizScrollBar_LearningRate_actionTriggered(int action);

    void on_spinBox_training_Epochs_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    //-------------------------
    Backpropagation *bp;

    void logEpochInfoToFile(int epoch, double pg, double mse);
};

#endif // MAINWINDOW_H
