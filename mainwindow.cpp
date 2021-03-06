#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globalVariables.h"


//--------------------------------------

LetterStructure letters[20001];
LetterStructure testPattern;

bool patternsLoadedFromFile;
int MAX_EPOCHS;
double LEARNING_RATE;
//int NUMBER_OF_PATTERNS;

///////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //---------------------------------------
    //initialisation of global variables
    //

    //NUMBER_OF_PATTERNS = 20000;

    LEARNING_RATE=0.2;
    patternsLoadedFromFile = false;
    MAX_EPOCHS = 50;

    bp = new Backpropagation;

    //---------------------------------------
    //initialise widgets

    ui->spinBox_training_Epochs->setValue(MAX_EPOCHS);
    ui->horizScrollBar_LearningRate->setValue(int(LEARNING_RATE*100));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logEpochInfoToFile(int epoch, double pg, double mse){
    if(epoch == 0){
        QFile file("../Assignment2/epoch_log.csv");
        if(file.open(QIODevice::WriteOnly)){
            QTextStream out(&file);
            out << "Percent Good" << ",";
            out << "Mean Squared Error" << "," << endl;
            out << pg << ",";
            out << mse << "," << endl;

        }else{
            qDebug() << "Something went wrong opening epoch log file";
        }

        file.close();
    }else{
        QFile file("../Assignment2/epoch_log.csv");
        if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
            QTextStream out(&file);
            out << pg << ",";
            out << mse << "," << endl;

        }else{
            qDebug() << "Something went wrong opening epoch log file";
        }

        file.close();
    }
}

void setOutputsToZero(int *outputArr, unsigned size, int letterToSet){
    for(unsigned i = 0; i < size; i++){
        outputArr[i] = 0;
    }
    outputArr[letterToSet] = 1;
}

void MainWindow::on_pushButton_Shuffle_Data_clicked()
{
    QString lineOfData, msg;

    QString readFileName = "complete_data_set.txt";
    QString saveFileName = "complete_data_set_shuffled.txt";

    QVector<QString> data;

    lineOfData = "Reading file " + readFileName + "for shuffling\n";
    msg.append(lineOfData);

     QFile file(readFileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Data file does not exist!";
        lineOfData = "File " + readFileName + " does not exist!\n";
        msg.append(lineOfData);
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    while(!line.isNull()){
        line = in.readLine();
        data.push_back(line);
    }

    lineOfData.sprintf("Number of data points to shuffle: %d\n", data.size());
    msg.append(lineOfData);
    file.close();

    int same = 10000;
    while(same > 0){
        same = 0;
        for(int i = 0; i < data.size()-1; i++){
            if(data[i][0] == data[i+1][0]){
                QString temp;
                temp = data[i];
                data[i] = data[0];
                data[0] = temp;
                same ++;
            }
        }
        lineOfData.sprintf("Number of repeating examples: %d\n", same);
        msg.append(lineOfData);
    }

    qDebug() << "File has been shuffled";
    lineOfData = "File has been shuffled\n";
    msg.append(lineOfData);

    QFile wFile(saveFileName);
    if(!wFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Something went wrong shuffling the data!";
        return;
    }

    QTextStream out(&wFile);
    for(QString i : data){
        out << i << "\n";
    }
    wFile.close();

    lineOfData = "Wrote Results to: " + saveFileName;
    msg.append(lineOfData);

    ui->plainTextEdit_results->setPlainText(msg);
    qApp->processEvents();
}

void MainWindow::on_pushButton_Read_File_clicked()
{
    qDebug() << "\nReading file...";
    QFile file("complete_data_set_shuffled.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!file.exists()){
        patternsLoadedFromFile=false;

        qDebug() << "Data file does not exist!";
        return;
    }

    QTextStream in(&file);



    char t;
    char characterSymbol;
    QString line;

    int counterForLetter[26];
    for(int i = 0; i < 26; i ++) counterForLetter[i] = 0;
    QString lineOfData;
    QString msg;
    int i=0;
    //while(i < NUMBER_OF_TRAINING_PATTERNS){
    while(i < NUMBER_OF_PATTERNS){

        //e.g. T,2,8,3,5,1,8,13,0,6,6,10,8,0,8,0,8
        in >> characterSymbol >> t >> letters[i].f[0] >> t >>  letters[i].f[1] >> t >>  letters[i].f[2] >> t >>  letters[i].f[3] >> t >>  letters[i].f[4] >> t >>  letters[i].f[5] >> t >>  letters[i].f[6] >> t >>  letters[i].f[7] >> t >>  letters[i].f[8] >> t >>  letters[i].f[9] >> t >>  letters[i].f[10] >> t >>  letters[i].f[11] >> t >> letters[i].f[12] >> t >> letters[i].f[13] >> t >> letters[i].f[14] >> t >> letters[i].f[15];
        line = in.readLine();

//        for(int i = 0; i < 16; i++){
//            letters[i].f[i] = letters[i].f[i]/16;
//        }

        if(characterSymbol == 'A'){
            letters[i].symbol = LETTER_A;
            setOutputsToZero(letters[i].outputs, 26, LETTER_A);
            counterForLetter[LETTER_A]++;
        } else if(characterSymbol == 'B'){
            letters[i].symbol = LETTER_B;
            setOutputsToZero(letters[i].outputs, 26, LETTER_B);
            counterForLetter[LETTER_B]++;
        } else if(characterSymbol == 'C'){
            letters[i].symbol = LETTER_C;
            setOutputsToZero(letters[i].outputs, 26, LETTER_C);
            counterForLetter[LETTER_C]++;
        }else if(characterSymbol == 'D'){
            letters[i].symbol = LETTER_D;
            setOutputsToZero(letters[i].outputs, 26, LETTER_D);
            counterForLetter[LETTER_D]++;

        }else if(characterSymbol == 'E'){

            letters[i].symbol = LETTER_E;
            setOutputsToZero(letters[i].outputs, 26, LETTER_E);
            counterForLetter[LETTER_E]++;

        }else if(characterSymbol == 'F'){

            letters[i].symbol = LETTER_F;
            setOutputsToZero(letters[i].outputs, 26, LETTER_F);
            counterForLetter[LETTER_F]++;

        }else if(characterSymbol == 'G'){

            letters[i].symbol = LETTER_G;
            setOutputsToZero(letters[i].outputs, 26, LETTER_G);
            counterForLetter[LETTER_G]++;

        }else if(characterSymbol == 'H'){

            letters[i].symbol = LETTER_H;
            setOutputsToZero(letters[i].outputs, 26, LETTER_H);
            counterForLetter[LETTER_H]++;

        }else if(characterSymbol == 'I'){

            letters[i].symbol = LETTER_I;
            setOutputsToZero(letters[i].outputs, 26, LETTER_I);
            counterForLetter[LETTER_I]++;

        }else if(characterSymbol == 'J'){

            letters[i].symbol = LETTER_J;
            setOutputsToZero(letters[i].outputs, 26, LETTER_J);
            counterForLetter[LETTER_J]++;

        }else if(characterSymbol == 'K'){

            letters[i].symbol = LETTER_K;
            setOutputsToZero(letters[i].outputs, 26, LETTER_K);
            counterForLetter[LETTER_K]++;

        }else if(characterSymbol == 'L'){

            letters[i].symbol = LETTER_L;
            setOutputsToZero(letters[i].outputs, 26, LETTER_L);
            counterForLetter[LETTER_L]++;

        }else if(characterSymbol == 'M'){

            letters[i].symbol = LETTER_M;
            setOutputsToZero(letters[i].outputs, 26, LETTER_M);
            counterForLetter[LETTER_M]++;

        }else if(characterSymbol == 'N'){

            letters[i].symbol = LETTER_N;
            setOutputsToZero(letters[i].outputs, 26, LETTER_N);
            counterForLetter[LETTER_N]++;

        }else if(characterSymbol == 'O'){

            letters[i].symbol = LETTER_O;
            setOutputsToZero(letters[i].outputs, 26, LETTER_O);
            counterForLetter[LETTER_O]++;

        }else if(characterSymbol == 'P'){

            letters[i].symbol = LETTER_P;
            setOutputsToZero(letters[i].outputs, 26, LETTER_P);
            counterForLetter[LETTER_P]++;

        }else if(characterSymbol == 'Q'){

            letters[i].symbol = LETTER_Q;
            setOutputsToZero(letters[i].outputs, 26, LETTER_Q);
            counterForLetter[LETTER_Q]++;

        }else if(characterSymbol == 'R'){

            letters[i].symbol = LETTER_R;
            setOutputsToZero(letters[i].outputs, 26, LETTER_R);
            counterForLetter[LETTER_R]++;

        }else if(characterSymbol == 'S'){

            letters[i].symbol = LETTER_S;
            setOutputsToZero(letters[i].outputs, 26, LETTER_S);
            counterForLetter[LETTER_S]++;

        }else if(characterSymbol == 'T'){

            letters[i].symbol = LETTER_T;
            setOutputsToZero(letters[i].outputs, 26, LETTER_T);
            counterForLetter[LETTER_T]++;

        }else if(characterSymbol == 'U'){

            letters[i].symbol = LETTER_U;
            setOutputsToZero(letters[i].outputs, 26, LETTER_U);
            counterForLetter[LETTER_U]++;

        }else if(characterSymbol == 'V'){

            letters[i].symbol = LETTER_V;
            setOutputsToZero(letters[i].outputs, 26, LETTER_V);
            counterForLetter[LETTER_V]++;

        }else if(characterSymbol == 'W'){

            letters[i].symbol = LETTER_W;
            setOutputsToZero(letters[i].outputs, 26, LETTER_W);
            counterForLetter[LETTER_W]++;

        }else if(characterSymbol == 'X'){

            letters[i].symbol = LETTER_X;
            setOutputsToZero(letters[i].outputs, 26, LETTER_X);
            counterForLetter[LETTER_X]++;

        }else if(characterSymbol == 'Y'){

            letters[i].symbol = LETTER_Y;
            setOutputsToZero(letters[i].outputs, 26, LETTER_Y);
            counterForLetter[LETTER_Y]++;

        }else if(characterSymbol == 'Z'){

            letters[i].symbol = LETTER_Z;
            setOutputsToZero(letters[i].outputs, 26, LETTER_Z);
            counterForLetter[LETTER_Z]++;
        }

        if((i % 50==0) || (i == NUMBER_OF_PATTERNS-1)){
            msg.clear();
            lineOfData.sprintf("number of patterns for Letter A = %d\n", counterForLetter[LETTER_A]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter B = %d\n", counterForLetter[LETTER_B]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter C = %d\n", counterForLetter[LETTER_C]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter D = %d\n", counterForLetter[LETTER_D]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter E = %d\n", counterForLetter[LETTER_E]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter F = %d\n", counterForLetter[LETTER_F]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter G = %d\n", counterForLetter[LETTER_G]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter H = %d\n", counterForLetter[LETTER_H]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter I = %d\n", counterForLetter[LETTER_I]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter J = %d\n", counterForLetter[LETTER_J]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter K = %d\n", counterForLetter[LETTER_K]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter L = %d\n", counterForLetter[LETTER_L]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter M = %d\n", counterForLetter[LETTER_M]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter N = %d\n", counterForLetter[LETTER_N]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter O = %d\n", counterForLetter[LETTER_O]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter P = %d\n", counterForLetter[LETTER_P]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter Q = %d\n", counterForLetter[LETTER_Q]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter R = %d\n", counterForLetter[LETTER_R]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter S = %d\n", counterForLetter[LETTER_S]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter T = %d\n", counterForLetter[LETTER_T]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter U = %d\n", counterForLetter[LETTER_U]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter V = %d\n", counterForLetter[LETTER_V]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter W = %d\n", counterForLetter[LETTER_W]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter X = %d\n", counterForLetter[LETTER_X]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter Y = %d\n", counterForLetter[LETTER_Y]);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter Z = %d\n", counterForLetter[LETTER_Z]);
            msg.append(lineOfData);

            ui->plainTextEdit_results->setPlainText(msg);
        }
        i++;
    }
    qApp->processEvents();

    msg.append("done.");

    ui->plainTextEdit_results->setPlainText(msg);
    qApp->processEvents();

    patternsLoadedFromFile=true;

}

void MainWindow::on_horizScrollBar_LearningRate_valueChanged(int value)
{
    ui->lcdNumber_LearningRate->setSegmentStyle(QLCDNumber::Filled);
    ui->lcdNumber_LearningRate->display(value/1000.0);
    LEARNING_RATE = value/1000.0;
}

//void MainWindow::on_pushButton_Train_Network_clicked()
//{

//}

void MainWindow::on_pushButton_Classify_Test_Pattern_clicked()
{

    char characterSymbol, t;
    QString *q;
    double* classificationResults;

    double* outputs;
    outputs = new double[OUTPUT_NEURONS];

//    delete q;
//    delete classificationResults;



    classificationResults = new double[OUTPUT_NEURONS];

    //QTextStream line;
    q = new QString(ui->plainTextEdit_Input_Pattern->toPlainText());

    QTextStream line(q);

    line >> characterSymbol >> t >> testPattern.f[0] >> t >>  testPattern.f[1] >> t >>  testPattern.f[2] >> t >>  testPattern.f[3] >> t >>  testPattern.f[4] >> t >>  testPattern.f[5] >> t >>  testPattern.f[6] >> t >>  testPattern.f[7] >> t >>  testPattern.f[8] >> t >>  testPattern.f[9] >> t >>  testPattern.f[10] >> t >>  testPattern.f[11] >> t >> testPattern.f[12] >> t >> testPattern.f[13] >> t >> testPattern.f[14] >> t >> testPattern.f[15];

    if(characterSymbol == 'A'){
        testPattern.symbol = LETTER_A;
        setOutputsToZero(testPattern.outputs,26,LETTER_A);
    }else if(characterSymbol == 'B'){
        testPattern.symbol = LETTER_B;
        setOutputsToZero(testPattern.outputs,26,LETTER_B);
    }else if(characterSymbol == 'C'){
        testPattern.symbol = LETTER_C;
        setOutputsToZero(testPattern.outputs,26,LETTER_C);
    }else if(characterSymbol == 'D'){
        testPattern.symbol = LETTER_D;
        setOutputsToZero(testPattern.outputs,26,LETTER_D);
    }else if(characterSymbol == 'E'){
        testPattern.symbol = LETTER_E;
        setOutputsToZero(testPattern.outputs,26,LETTER_E);
    }else if(characterSymbol == 'F'){
        testPattern.symbol = LETTER_F;
        setOutputsToZero(testPattern.outputs,26,LETTER_F);
    }else if(characterSymbol == 'G'){
        testPattern.symbol = LETTER_G;
        setOutputsToZero(testPattern.outputs,26,LETTER_G);
    }else if(characterSymbol == 'H'){
        testPattern.symbol = LETTER_H;
        setOutputsToZero(testPattern.outputs,26,LETTER_H);
    }else if(characterSymbol == 'I'){
        testPattern.symbol = LETTER_I;
        setOutputsToZero(testPattern.outputs,26,LETTER_I);
    }else if(characterSymbol == 'J'){
        testPattern.symbol = LETTER_J;
        setOutputsToZero(testPattern.outputs,26,LETTER_J);
    }else if(characterSymbol == 'K'){
        testPattern.symbol = LETTER_K;
        setOutputsToZero(testPattern.outputs,26,LETTER_K);
    }else if(characterSymbol == 'L'){
        testPattern.symbol = LETTER_L;
        setOutputsToZero(testPattern.outputs,26,LETTER_L);
    }else if(characterSymbol == 'M'){
        testPattern.symbol = LETTER_M;
        setOutputsToZero(testPattern.outputs,26,LETTER_M);
    }else if(characterSymbol == 'N'){
        testPattern.symbol = LETTER_N;
        setOutputsToZero(testPattern.outputs,26,LETTER_N);
    }else if(characterSymbol == 'O'){
        testPattern.symbol = LETTER_O;
        setOutputsToZero(testPattern.outputs,26,LETTER_O);
    }else if(characterSymbol == 'P'){
        testPattern.symbol = LETTER_P;
        setOutputsToZero(testPattern.outputs,26,LETTER_P);
    }else if(characterSymbol == 'Q'){
        testPattern.symbol = LETTER_Q;
        setOutputsToZero(testPattern.outputs,26,LETTER_Q);
    }else if(characterSymbol == 'R'){
        testPattern.symbol = LETTER_R;
        setOutputsToZero(testPattern.outputs,26,LETTER_R);
    }else if(characterSymbol == 'S'){
        testPattern.symbol = LETTER_S;
        setOutputsToZero(testPattern.outputs,26,LETTER_S);
    }else if(characterSymbol == 'T'){
        testPattern.symbol = LETTER_T;
        setOutputsToZero(testPattern.outputs,26,LETTER_T);
    }else if(characterSymbol == 'U'){
        testPattern.symbol = LETTER_U;
        setOutputsToZero(testPattern.outputs,26,LETTER_U);
    }else if(characterSymbol == 'V'){
        testPattern.symbol = LETTER_V;
        setOutputsToZero(testPattern.outputs,26,LETTER_V);
    }else if(characterSymbol == 'W'){
        testPattern.symbol = LETTER_W;
        setOutputsToZero(testPattern.outputs,26,LETTER_W);
    }else if(characterSymbol == 'X'){
        testPattern.symbol = LETTER_X;
        setOutputsToZero(testPattern.outputs,26,LETTER_X);
    }else if(characterSymbol == 'Y'){
        testPattern.symbol = LETTER_Y;
        setOutputsToZero(testPattern.outputs,26,LETTER_Y);
    }else if(characterSymbol == 'Z'){
        testPattern.symbol = LETTER_Z;
        setOutputsToZero(testPattern.outputs,26,LETTER_Z);
    }


    //---------------------------------
    classificationResults = bp->testNetwork(testPattern);

    ui->lcdNumber_A->display(classificationResults[0]);
    ui->lcdNumber_B->display(classificationResults[1]);
    ui->lcdNumber_C->display(classificationResults[2]);
    ui->lcdNumber_D->display(classificationResults[3]);
    ui->lcdNumber_E->display(classificationResults[4]);
    ui->lcdNumber_F->display(classificationResults[5]);
    ui->lcdNumber_G->display(classificationResults[6]);
    ui->lcdNumber_H->display(classificationResults[7]);
    ui->lcdNumber_I->display(classificationResults[8]);
    ui->lcdNumber_J->display(classificationResults[9]);
    ui->lcdNumber_K->display(classificationResults[10]);
    ui->lcdNumber_L->display(classificationResults[11]);
    ui->lcdNumber_M->display(classificationResults[12]);
    ui->lcdNumber_N->display(classificationResults[13]);
    ui->lcdNumber_O->display(classificationResults[14]);
    ui->lcdNumber_P->display(classificationResults[15]);
    ui->lcdNumber_Q->display(classificationResults[16]);
    ui->lcdNumber_R->display(classificationResults[17]);
    ui->lcdNumber_S->display(classificationResults[18]);
    ui->lcdNumber_T->display(classificationResults[19]);
    ui->lcdNumber_U->display(classificationResults[20]);
    ui->lcdNumber_V->display(classificationResults[21]);
    ui->lcdNumber_W->display(classificationResults[22]);
    ui->lcdNumber_X->display(classificationResults[23]);
    ui->lcdNumber_Y->display(classificationResults[24]);
    ui->lcdNumber_Z->display(classificationResults[25]);



    //-----------------------------------------------------------
    for(int k=0; k < OUTPUT_NEURONS; k++){
        if(testPattern.outputs[k] == 1){
            outputs[k] = 1;
        }else{
            outputs[k] = 0;
        }
    }
    //-----------------------------------------------------------
     QString textClassification;
     switch(bp->action(outputs)){
        case 0:
            textClassification = "letter A";
            break;
        case 1:
            textClassification = "letter B";
            break;
        case 2:
            textClassification = "letter C";
            break;
        case 3:
            textClassification = "letter D";
            break;
        case 4:
            textClassification = "letter E";
            break;
        case 5:
            textClassification = "letter F";
            break;
        case 6:
            textClassification = "letter G";
            break;
        case 7:
            textClassification = "letter H";
            break;
        case 8:
            textClassification = "letter I";
            break;
        case 9:
            textClassification = "letter J";
            break;
        case 10:
            textClassification = "letter K";
            break;
        case 11:
            textClassification = "letter L";
            break;
        case 12:
            textClassification = "letter M";
            break;
        case 13:
            textClassification = "letter N";
            break;
        case 14:
            textClassification = "letter O";
            break;
        case 15:
            textClassification = "letter P";
            break;
        case 16:
            textClassification = "letter Q";
            break;
        case 17:
            textClassification = "letter R";
            break;
        case 18:
            textClassification = "letter S";
            break;
        case 19:
            textClassification = "letter T";
            break;
        case 20:
            textClassification = "letter U";
            break;
        case 21:
            textClassification = "letter V";
            break;
        case 22:
            textClassification = "letter W";
            break;
        case 23:
            textClassification = "letter X";
            break;
        case 24:
            textClassification = "letter Y";
            break;
        case 25:
            textClassification = "letter Z";
            break;
     };

    if (bp->action(classificationResults) == bp->action(outputs)) {
        qDebug() << "correct classification.";
        ui->label_Classification->setText(textClassification + ", - Correct classification!");
    } else {
        qDebug() << "incorrect classification.";
        ui->label_Classification->setText(textClassification + ", -XXX- Incorrect classification.");
    }

}

void MainWindow::on_pushButton_Train_Network_Max_Epochs_clicked()
{
    double SSE = 0.0;
    QString msg;

    double percent_good;

    if(!patternsLoadedFromFile) {
        msg.clear();
        msg.append("\nMissing training patterns.  Load data set first.\n");
        ui->plainTextEdit_results->setPlainText(msg);
        return;
    }

    MAX_EPOCHS = ui->spinBox_training_Epochs->value();
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    for(int i=0; i < MAX_EPOCHS; i++){
      msg.clear();
      msg.append("\nTraining in progress...\n");
      msg.append("\nEpoch=");
      msg.append(QString::number(i));
      ui->plainTextEdit_results->setPlainText(msg);

      SSE = bp->trainNetwork(1);
      double pg = bp->getError_PG();
      double mse = bp->getError_MSE();

      ui->lcdNumber_SSE->display(bp->getError_SSE());
      ui->lcdNumber_MSE->display(QString::number(mse));
      ui->lcdNumber_Correct->display(bp->getCorrect());
      ui->lcdNumber_percentageOfGoodClassification->display(QString::number(pg));
      qApp->processEvents();

      logEpochInfoToFile(i, pg, mse);

      update();

      if(i % 5 ==0){
         bp->saveWeights("../Assignment2/data/" + ui->plainTextEdit_saveWeightsAs->toPlainText());

         ui->plainTextEdit_results->setPlainText("Weights saved into file.");
         qApp->processEvents();
      }

      if(pg > ui->doubleSpinBox_percent_good->value()){
          bp->saveWeights(ui->plainTextEdit_saveWeightsAs->toPlainText());
          ui->plainTextEdit_results->setPlainText("Weights saved into file.");
          break;
      }

    }
    QApplication::restoreOverrideCursor();

}

void MainWindow::on_pushButton_Initialise_Network_clicked()
{
    bp->initialise();
}

void MainWindow::on_pushButton_Test_All_Patterns_clicked()
{
    char characterSymbol, t;
    QString *q;
    double* classificationResults;
    double* outputs;
    int correctClassifications=0;

    classificationResults = new double[OUTPUT_NEURONS];
    outputs = new double[OUTPUT_NEURONS];

    for(int i=NUMBER_OF_TRAINING_PATTERNS; i < NUMBER_OF_PATTERNS; i++){

            characterSymbol = letters[i].symbol;
            for(int j=0; j < INPUT_NEURONS; j++){
                testPattern.f[j] = letters[i].f[j];
            }

            if(characterSymbol == LETTER_A){
                testPattern.symbol = LETTER_A;
                setOutputsToZero(testPattern.outputs,26,LETTER_A);
            }else if(characterSymbol == LETTER_B){
                testPattern.symbol = LETTER_B;
                setOutputsToZero(testPattern.outputs,26,LETTER_B);
            }else if(characterSymbol == LETTER_C){
                testPattern.symbol = LETTER_C;
                setOutputsToZero(testPattern.outputs,26,LETTER_C);
            }else if(characterSymbol == LETTER_D){
                testPattern.symbol = LETTER_D;
                setOutputsToZero(testPattern.outputs,26,LETTER_D);
            }else if(characterSymbol == LETTER_E){
                testPattern.symbol = LETTER_E;
                setOutputsToZero(testPattern.outputs,26,LETTER_E);
            }else if(characterSymbol == LETTER_F){
                testPattern.symbol = LETTER_F;
                setOutputsToZero(testPattern.outputs,26,LETTER_F);
            }else if(characterSymbol == LETTER_G){
                testPattern.symbol = LETTER_G;
                setOutputsToZero(testPattern.outputs,26,LETTER_G);
            }else if(characterSymbol == LETTER_H){
                testPattern.symbol = LETTER_H;
                setOutputsToZero(testPattern.outputs,26,LETTER_H);
            }else if(characterSymbol == LETTER_I){
                testPattern.symbol = LETTER_I;
                setOutputsToZero(testPattern.outputs,26,LETTER_I);
            }else if(characterSymbol == LETTER_J){
                testPattern.symbol = LETTER_J;
                setOutputsToZero(testPattern.outputs,26,LETTER_J);
            }else if(characterSymbol == LETTER_K){
                testPattern.symbol = LETTER_K;
                setOutputsToZero(testPattern.outputs,26,LETTER_K);
            }else if(characterSymbol == LETTER_L){
                testPattern.symbol = LETTER_L;
                setOutputsToZero(testPattern.outputs,26,LETTER_L);
            }else if(characterSymbol == LETTER_M){
                testPattern.symbol = LETTER_M;
                setOutputsToZero(testPattern.outputs,26,LETTER_M);
            }else if(characterSymbol == LETTER_N){
                testPattern.symbol = LETTER_N;
                setOutputsToZero(testPattern.outputs,26,LETTER_N);
            }else if(characterSymbol == LETTER_O){
                testPattern.symbol = LETTER_O;
                setOutputsToZero(testPattern.outputs,26,LETTER_O);
            }else if(characterSymbol == LETTER_P){
                testPattern.symbol = LETTER_P;
                setOutputsToZero(testPattern.outputs,26,LETTER_P);
            }else if(characterSymbol == LETTER_Q){
                testPattern.symbol = LETTER_Q;
                setOutputsToZero(testPattern.outputs,26,LETTER_Q);
            }else if(characterSymbol == LETTER_R){
                testPattern.symbol = LETTER_R;
                setOutputsToZero(testPattern.outputs,26,LETTER_R);
            }else if(characterSymbol == LETTER_S){
                testPattern.symbol = LETTER_S;
                setOutputsToZero(testPattern.outputs,26,LETTER_S);
            }else if(characterSymbol == LETTER_T){
                testPattern.symbol = LETTER_T;
                setOutputsToZero(testPattern.outputs,26,LETTER_T);
            }else if(characterSymbol == LETTER_U){
                testPattern.symbol = LETTER_U;
                setOutputsToZero(testPattern.outputs,26,LETTER_U);
            }else if(characterSymbol == LETTER_V){
                testPattern.symbol = LETTER_V;
                setOutputsToZero(testPattern.outputs,26,LETTER_V);
            }else if(characterSymbol == LETTER_W){
                testPattern.symbol = LETTER_W;
                setOutputsToZero(testPattern.outputs,26,LETTER_W);
            }else if(characterSymbol == LETTER_X){
                testPattern.symbol = LETTER_X;
                setOutputsToZero(testPattern.outputs,26,LETTER_X);
            }else if(characterSymbol == LETTER_Y){
                testPattern.symbol = LETTER_Y;
                setOutputsToZero(testPattern.outputs,26,LETTER_Y);
            }else if(characterSymbol == LETTER_Z){
                testPattern.symbol = LETTER_Z;
                setOutputsToZero(testPattern.outputs,26,LETTER_Z);
            }

            //---------------------------------
            classificationResults = bp->testNetwork(testPattern);

            for(int k=0; k < OUTPUT_NEURONS; k++){
                if(testPattern.outputs[k] == 1){
                    outputs[k] = 1;
                }else{
                    outputs[k] = 0;
                }
            }

            if (bp->action(classificationResults) == bp->action(outputs)) {
                 correctClassifications++;
            }
        }


      qDebug() << "TEST SET: correctClassifications = " << correctClassifications;

      double correctPercent;
      correctPercent = 100.0 * (double(correctClassifications)/double(NUMBER_OF_TEST_PATTERNS));

      QString s;
      s.append("Correct Classifications: ");
      s.append(QString::number(correctClassifications));
      s.append("\t Percent Correct: ");
      s.append(QString::number(correctPercent));

      ui->lineEdit_test_result->setText(s.toUtf8().constData());

}

void MainWindow::on_pushButton_Save_Weights_clicked()
{
    bp->saveWeights(ui->plainTextEdit_saveWeightsAs->toPlainText());

    QString msg;
    QString lineOfText;

    lineOfText = "weights saved to file: " + ui->plainTextEdit_saveWeightsAs->toPlainText();

    msg.append(lineOfText);

    ui->plainTextEdit_results->setPlainText(msg);

}

void MainWindow::on_pushButton_Load_Weights_clicked()
{
   bp->loadWeights("../Assignment2/data/" + ui->plainTextEdit_fileNameLoadWeights->toPlainText());

   QString msg;

   msg.append("weights loaded.\n");

   ui->plainTextEdit_results->setPlainText(msg);


}

void MainWindow::on_pushButton_testNetOnTrainingSet_clicked()
{
    char characterSymbol, t;
    QString *q;
    double* classificationResults;
    double* outputs;
    int correctClassifications=0;

    classificationResults = new double[OUTPUT_NEURONS];
    outputs = new double[OUTPUT_NEURONS];

    for(int i=0; i < NUMBER_OF_TRAINING_PATTERNS; i++){

            characterSymbol = letters[i].symbol;
            for(int j=0; j < INPUT_NEURONS; j++){
                testPattern.f[j] = letters[i].f[j];
            }

            if(characterSymbol == LETTER_A){
                testPattern.symbol = LETTER_A;
                setOutputsToZero(testPattern.outputs,26,LETTER_A);
            }else if(characterSymbol == LETTER_B){
                testPattern.symbol = LETTER_B;
                setOutputsToZero(testPattern.outputs,26,LETTER_B);
            }else if(characterSymbol == LETTER_C){
                testPattern.symbol = LETTER_C;
                setOutputsToZero(testPattern.outputs,26,LETTER_C);
            }else if(characterSymbol == LETTER_D){
                testPattern.symbol = LETTER_D;
                setOutputsToZero(testPattern.outputs,26,LETTER_D);
            }else if(characterSymbol == LETTER_E){
                testPattern.symbol = LETTER_E;
                setOutputsToZero(testPattern.outputs,26,LETTER_E);
            }else if(characterSymbol == LETTER_F){
                testPattern.symbol = LETTER_F;
                setOutputsToZero(testPattern.outputs,26,LETTER_F);
            }else if(characterSymbol == LETTER_G){
                testPattern.symbol = LETTER_G;
                setOutputsToZero(testPattern.outputs,26,LETTER_G);
            }else if(characterSymbol == LETTER_H){
                testPattern.symbol = LETTER_H;
                setOutputsToZero(testPattern.outputs,26,LETTER_H);
            }else if(characterSymbol == LETTER_I){
                testPattern.symbol = LETTER_I;
                setOutputsToZero(testPattern.outputs,26,LETTER_I);
            }else if(characterSymbol == LETTER_J){
                testPattern.symbol = LETTER_J;
                setOutputsToZero(testPattern.outputs,26,LETTER_J);
            }else if(characterSymbol == LETTER_K){
                testPattern.symbol = LETTER_K;
                setOutputsToZero(testPattern.outputs,26,LETTER_K);
            }else if(characterSymbol == LETTER_L){
                testPattern.symbol = LETTER_L;
                setOutputsToZero(testPattern.outputs,26,LETTER_L);
            }else if(characterSymbol == LETTER_M){
                testPattern.symbol = LETTER_M;
                setOutputsToZero(testPattern.outputs,26,LETTER_M);
            }else if(characterSymbol == LETTER_N){
                testPattern.symbol = LETTER_N;
                setOutputsToZero(testPattern.outputs,26,LETTER_N);
            }else if(characterSymbol == LETTER_O){
                testPattern.symbol = LETTER_O;
                setOutputsToZero(testPattern.outputs,26,LETTER_O);
            }else if(characterSymbol == LETTER_P){
                testPattern.symbol = LETTER_P;
                setOutputsToZero(testPattern.outputs,26,LETTER_P);
            }else if(characterSymbol == LETTER_Q){
                testPattern.symbol = LETTER_Q;
                setOutputsToZero(testPattern.outputs,26,LETTER_Q);
            }else if(characterSymbol == LETTER_R){
                testPattern.symbol = LETTER_R;
                setOutputsToZero(testPattern.outputs,26,LETTER_R);
            }else if(characterSymbol == LETTER_S){
                testPattern.symbol = LETTER_S;
                setOutputsToZero(testPattern.outputs,26,LETTER_S);
            }else if(characterSymbol == LETTER_T){
                testPattern.symbol = LETTER_T;
                setOutputsToZero(testPattern.outputs,26,LETTER_T);
            }else if(characterSymbol == LETTER_U){
                testPattern.symbol = LETTER_U;
                setOutputsToZero(testPattern.outputs,26,LETTER_U);
            }else if(characterSymbol == LETTER_V){
                testPattern.symbol = LETTER_V;
                setOutputsToZero(testPattern.outputs,26,LETTER_V);
            }else if(characterSymbol == LETTER_W){
                testPattern.symbol = LETTER_W;
                setOutputsToZero(testPattern.outputs,26,LETTER_W);
            }else if(characterSymbol == LETTER_X){
                testPattern.symbol = LETTER_X;
                setOutputsToZero(testPattern.outputs,26,LETTER_X);
            }else if(characterSymbol == LETTER_Y){
                testPattern.symbol = LETTER_Y;
                setOutputsToZero(testPattern.outputs,26,LETTER_Y);
            }else if(characterSymbol == LETTER_Z){
                testPattern.symbol = LETTER_Z;
                setOutputsToZero(testPattern.outputs,26,LETTER_Z);
            }

            //---------------------------------
            classificationResults = bp->testNetwork(testPattern);

            for(int k=0; k < OUTPUT_NEURONS; k++){
               outputs[k] = testPattern.outputs[k];
            }

            if (bp->action(classificationResults) == bp->action(outputs)) {
                 correctClassifications++;
            }

        }
      qDebug() << "TRAINING SET: correctClassifications = " << correctClassifications;

      double correctPercent;
      correctPercent = 100.0 * (double(correctClassifications)/double(NUMBER_OF_TRAINING_PATTERNS));

      QString s;
      s.append("Correct Classifications: ");
      s.append(QString::number(correctClassifications));
      s.append("\t Percent Correct: ");
      s.append(QString::number(correctPercent));

      ui->lineEdit_training_result->setText(s.toUtf8().constData());
}

void MainWindow::on_horizScrollBar_LearningRate_actionTriggered(int action)
{

}
