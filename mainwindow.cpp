#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QDebug>


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// field variables
	graphicsView = ui->graphicsView;
	scenes = QVector<CustomScene*>();

	resizeFactor = 1;     // default value
    this->dpi = 350; //default

    // focus policy
    this->setFocusPolicy(Qt::StrongFocus);
    ui->graphicsView->setFocusProxy(this);
    this->setWindowIcon(QPixmap(":/resources/logo_256.ico"));
}

MainWindow::~MainWindow()
{
	closeScenes();
	delete ui;
}

void MainWindow::UpdateView() {
	if (scenes.isEmpty()) {
		return;
	}
	QRectF sceneRect = scenes.at(pageIndex)->sceneRect();
	QRectF rect(sceneRect.left(), sceneRect.top(), sceneRect.width() / resizeFactor, sceneRect.height() / resizeFactor);
	graphicsView->fitInView(rect, Qt::KeepAspectRatio);
}


void MainWindow::on_actionOpen_Image_form_png_jpg_triggered()
{
	fileNames = QFileDialog::getOpenFileNames(this,
		tr("Open Images"), "C:/Users/junse/Desktop/ProblemCropper/testfolder", tr("Image Files (*.png *.jpg)"));
    if(fileNames.isEmpty()){
        //exit
        return;
    }
	pageIndex = 0;
	dirName = fileNames[0].section('/', -2, -2);

    for (int i=scenes.size()-1 ; i>=0; i--){
        auto temp = scenes.at(i);
        scenes.remove(i);
        delete temp;
    }

	// set scenes
	for (int i = 0; i < fileNames.size(); i++) {
		QImage image(fileNames[i]);
		QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        CustomScene* tempScene = new CustomScene(this);
		tempScene->addItem(item);
		tempScene->image = image;
		connect(tempScene, &CustomScene::rubberBandFinished, this, &MainWindow::on_rubberBand_finished);
		scenes.append(tempScene);
	}
	graphicsView->setScene(scenes.at(pageIndex));

	// ui
	ui->pageNumber_label->setText(QString(" / ") + QString::number((int)scenes.size()));
	ui->pageIndex_lineEdit->setText(QString::number(pageIndex + 1));
	ui->select_toolButton->setEnabled(true);
	ui->groupBox->setEnabled(true);
	ui->toolButton_4->setEnabled(true);
	ui->toolButton_3->setEnabled(true);
	ui->widget->setEnabled(true);
	ui->pushButton->setEnabled(true);
	UpdateView();
}

void MainWindow::on_actionOpen_Image_from_Pdf_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Images"), "C:/Users/junse/Desktop/ProblemCropper/testfolder", tr("Image Files (*.pdf)"));
    if(fileName.isEmpty() || fileName.isNull()){
        //exit
        return;
    }
    pageIndex = 0;
    dirName = fileName.section('/', -2, -2);

    /*setup directory*/
    QDir dir("./temp");
    if (!dir.exists()){
        dir.mkpath(".");
        dir.setPath("./temp");
    }
    else{
        /*empty the directory if exists*/
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);
        }
    }

    /*make png files out of pdf*/
    QProcess myProcess(this);
    QString command = QString("./pdftopng.exe -r %1 \"%2\" \"%3/%4/temp\"").arg(dpi).arg(fileName, QDir::currentPath(), dir.dirName());
    QMessageBox* msBox = new QMessageBox(this);
    msBox->setWindowTitle("reading pdf...\nIt may take some time");
    msBox->show();
    myProcess.start(command);
    myProcess.waitForFinished(-1);
    myProcess.close();
    msBox->close();
    msBox->deleteLater();
    qDebug() << command;
    // ./temp가 비었는지 확인
    if (dir.isEmpty(QDir::NoDotAndDotDot|QDir::AllEntries)){
        QMessageBox::warning(this,"Error", "Cannot convert pdf to png.\nMaybe missing 'pdftopng.exe'?\nAsk the developer for further help.",QMessageBox::Ok);
        return;
    }

    // 기존에 존재하던 scenes 삭제
    for (int i=scenes.size()-1 ; i>=0; i--){
        auto temp = scenes.at(i);
        scenes.remove(i);
        delete temp;
    }

    // set scenes
    foreach(QString dirFile, dir.entryList(QDir::AllEntries|QDir::NoDotAndDotDot))
    {
        QImage image(dir.absoluteFilePath(dirFile));
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        CustomScene* tempScene = new CustomScene(this);
        tempScene->addItem(item);
        tempScene->image = image;
        connect(tempScene, &CustomScene::rubberBandFinished, this, &MainWindow::on_rubberBand_finished);
        scenes.append(tempScene);
    }

    if (scenes.size()==0){
        QMessageBox::warning(this,"Error", "Cannot load pdf to scene.\nAsk the developer for further help.",QMessageBox::Ok);
        return;
    }
    ui->pageIndex_lineEdit->setText(QString::number(pageIndex + 1));

    // ui
    ui->pageNumber_label->setText(QString(" / ") + QString::number((int)scenes.size()));
    ui->select_toolButton->setEnabled(true);
    ui->groupBox->setEnabled(true);
    ui->toolButton_4->setEnabled(true);
    ui->toolButton_3->setEnabled(true);
    ui->widget->setEnabled(true);
    ui->pushButton->setEnabled(true);
    UpdateView();
}

void MainWindow::closeScenes() {
	for (CustomScene* scene : scenes) {
		delete scene;
	}
}


void MainWindow::resizeEvent(QResizeEvent*) {
	UpdateView();
}


void MainWindow::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_T && ui->select_toolButton->isEnabled()) {
		on_select_toolButton_clicked();
	}
    if (e->key() == Qt::Key_Escape && !ui->select_toolButton->isEnabled()) {
		scenes.at(pageIndex)->isRubberBandActive = false;
        scenes.at(pageIndex)->isVisualRubberBandActive = false;
		on_rubberBand_finished();
	}
    if (e->matches(QKeySequence::Undo) && ui->toolButton_3->isEnabled()){
        on_toolButton_3_clicked();
    }
    if (e->matches(QKeySequence::Redo) && ui->toolButton_4->isEnabled()){
        on_toolButton_4_clicked();
    }
}



void MainWindow::on_resizeFactor_lineEdit_textChanged(const QString& arg1)
{
	resizeFactor = ui->resizeFactor_lineEdit->text().toFloat() / 100;
	UpdateView();
}


void MainWindow::on_pushButton_4_pressed()
{
	ui->resizeFactor_lineEdit->setText(QString::number(ui->resizeFactor_lineEdit->text().toInt() + 10));
}


void MainWindow::on_pushButton_5_pressed()
{
	ui->resizeFactor_lineEdit->setText(QString::number(ui->resizeFactor_lineEdit->text().toInt() - 10));
}



void MainWindow::on_select_toolButton_clicked()
{
	if (!scenes.at(pageIndex)->isRubberBandActive) {
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
void MainWindow::on_pageIndex_lineEdit_textChanged(const QString& arg1)
{
	// ui index starts from 1, so I have to subtract 1.
	pageIndex = ui->pageIndex_lineEdit->text().toInt() - 1;
	graphicsView->setScene(scenes.at(pageIndex));
}


void MainWindow::on_pushButton_3_clicked()
{
	int temp = ui->pageIndex_lineEdit->text().toInt();
	temp++;
	//check boundaries
	if (temp <= scenes.size()) {
		ui->pageIndex_lineEdit->setText(QString::number(temp));
	}
}


void MainWindow::on_pushButton_2_clicked()
{
	int temp = ui->pageIndex_lineEdit->text().toInt();
	temp--;
	//check boundaries
	if (temp > 0) {
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
	int tempNumber = 1;
	QString dirPath = QFileDialog::getExistingDirectory(this,
        tr("Open Images"), "../../");
    if(dirPath.isEmpty() || dirPath.isNull()){
        return;
    }
    QString realDirPath = dirPath;
    realDirPath.detach();
	dirPath.append("/" + dirName + "_%1.png");

    for (auto scene : scenes) {
        auto state = calculateState(scene->stateIndex);
        for (int i = 0; i < scene->rubberBands.size(); i++) {
            if (state.at(i) <= 0) {
                continue;
            }
			QGraphicsRectItem* item = scene->rubberBands.at(i);
            QImage image = scene->image.copy(item->rect().toRect());
			image.save(dirPath.arg(tempNumber));
			qDebug() << dirPath.arg(tempNumber);
			tempNumber++;
		}
	}
    QMessageBox::information(this, "info", "Images are successfully saved!", QMessageBox::Ok);
    QProcess::startDetached(QString("explorer /root,\"%1\"").arg(realDirPath));
}

QList<int> MainWindow::calculateState(QList<QPair<int, int>> stateIndex) {
	QList<int> result = QList<int>();
    result.append(0);
	for (auto state : stateIndex) {
        while (state.first+1 > result.size()){
            result.append(0);
        }

        result[state.first] += state.second;
	}
	return result;
}





void MainWindow::on_actionProcess_and_save_cropped_images_triggered()
{
    on_pushButton_clicked();
}


void MainWindow::on_actionoutput_image_dpi_setting_triggered()
{
    bool ok;
    int dpi = QInputDialog::getInt(this, "Set DPI", "DPI should be between 50 and 400\nDefault DPI is 350.",this->dpi,50,400, 10, &ok);
    if (ok){
        this->dpi = dpi;
    }

}


void MainWindow::on_actionmanual_triggered()
{
    QDesktopServices::openUrl(manualUrl);
}


void MainWindow::on_actionAbout_QT_triggered()
{
    QApplication::aboutQt();
}


void MainWindow::on_actionSource_Code_triggered()
{
    QDesktopServices::openUrl(SourceCode);
}


void MainWindow::on_actionAbout_triggered()
{

    QDesktopServices::openUrl(Blog);
}

