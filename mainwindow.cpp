#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // field variables
    graphicsView = ui->graphicsView;
    scenes = QVector<CustomScene*>();

    resizeFactor = 1;     // default value
}

MainWindow::~MainWindow()
{
    closeScenes();
    delete ui;
}

void MainWindow::UpdateView(){
    if (scenes.isEmpty()){
        return;
    }
    QRectF sceneRect = scenes.at(pageIndex)->sceneRect();
    QRectF rect(sceneRect.left(),sceneRect.top(),sceneRect.width()/resizeFactor,sceneRect.height()/resizeFactor);
    graphicsView->fitInView(rect,Qt::KeepAspectRatio);
}


void MainWindow::on_actionOpen_Image_form_png_jpg_triggered()
{
    fileNames = QFileDialog::getOpenFileNames(this,
        tr("Open Images"), "C:/Users/junse/Desktop/ProblemCropper/testfolder", tr("Image Files (*.png *.jpg)"));
    pageIndex = 0;
    dirName = fileNames[0].section('/',-2,-2);

    // set scenes
    for (int i=0; i<fileNames.size(); i++){
        QImage image(fileNames[i]);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        CustomScene* tempScene = new CustomScene();
        tempScene->addItem(item);
        tempScene->image = image;
        connect(tempScene, &CustomScene::rubberBandFinished, this, &MainWindow::on_rubberBand_finished);
        scenes.append(tempScene);
    }
    graphicsView->setScene(scenes.at(pageIndex));

    // ui
    ui->pageNumber_label->setText(QString(" / ")+QString::number((int)scenes.size()));
    ui->pageIndex_lineEdit->setText(QString::number(pageIndex+1));
    UpdateView();
}

void MainWindow::closeScenes(){
    for (CustomScene* scene: scenes){
        delete scene;
    }
}


void MainWindow::resizeEvent(QResizeEvent *) {
    UpdateView();
}


void MainWindow::keyPressEvent(QKeyEvent *e){
    if (e->key()==Qt::Key_Control){
        on_select_toolButton_clicked();
    }
    if (e->key()==Qt::Key_Escape){
        scenes.at(pageIndex)->isRubberBandActive = false;
        on_rubberBand_finished();
    }
}



void MainWindow::on_resizeFactor_lineEdit_textChanged(const QString &arg1)
{
    resizeFactor = ui->resizeFactor_lineEdit->text().toFloat()/100;
    UpdateView();
}


void MainWindow::on_pushButton_4_pressed()
{
    ui->resizeFactor_lineEdit->setText(QString::number(ui->resizeFactor_lineEdit->text().toInt()+10));
}


void MainWindow::on_pushButton_5_pressed()
{
    ui->resizeFactor_lineEdit->setText(QString::number(ui->resizeFactor_lineEdit->text().toInt()-10));
}



void MainWindow::on_select_toolButton_clicked()
{
    if (!scenes.at(pageIndex)->isRubberBandActive){
        scenes.at(pageIndex)->isRubberBandActive = true;
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        ui->select_toolButton->setEnabled(false);
        this->cursor().setShape(Qt::CrossCursor);
    }
}


void MainWindow::on_toolButton_3_clicked()
{
    scenes.at(pageIndex)->unDo();
}


void MainWindow::on_toolButton_4_clicked()
{
    scenes.at(pageIndex)->reDo();
}

// this will handle all of the page changing stuffs.
void MainWindow::on_pageIndex_lineEdit_textChanged(const QString &arg1)
{
    // ui index starts from 1, so I have to subtract 1.
    pageIndex = ui->pageIndex_lineEdit->text().toInt()-1;
    graphicsView->setScene(scenes.at(pageIndex));
}


void MainWindow::on_pushButton_3_clicked()
{
    int temp = ui->pageIndex_lineEdit->text().toInt();
    temp++;
    //check boundaries
    if (temp<=scenes.size()){
        ui->pageIndex_lineEdit->setText(QString::number(temp));
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    int temp = ui->pageIndex_lineEdit->text().toInt();
    temp--;
    //check boundaries
    if (temp>0){
        ui->pageIndex_lineEdit->setText(QString::number(temp));
    }
}

void MainWindow::on_rubberBand_finished()
{
    ui->select_toolButton->setEnabled(true);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    this->cursor().setShape(Qt::ArrowCursor);
}


void MainWindow::on_pushButton_clicked()
{
    //name of imagefile.
    int tempNumber=1;
    QString dirPath = QFileDialog::getExistingDirectory(this,
            tr("Open Images"), "../../");
    dirPath.append("/"+dirName+"%1.png");

    for (auto scene: scenes) {
        auto state = calculateState(scene->stateIndex);
        for (int i = 0; i < scene->rubberBands.size(); i++){
            if (state.at(i)<=0){
                continue;
            }
            QGraphicsRectItem* item = scene->rubberBands.at(i);
            QImage image = scenes.at(i)->image.copy(item->rect().toRect());
            image.save(dirPath.arg(tempNumber));
            qDebug() << dirPath.arg(tempNumber);
            tempNumber++;
        }
    }
}

QList<int> MainWindow::calculateState(QList<QPair<int,int>> stateIndex){
    QList<int> result = QList<int>();
    for(auto state: stateIndex){
        if (state.first>=result.size()){
            result.append(1);
        }
        else{
            result[state.first] += state.second;
        }
    }
    return result;
}

