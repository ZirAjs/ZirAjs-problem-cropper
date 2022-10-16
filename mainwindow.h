#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./customscene.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QStringList>
#include <QString>
#include <QResizeEvent>
#include <QRectF>
#include <QList>
#include <QRubberBand>
#include <QMouseEvent>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // fields
    QGraphicsView* graphicsView;
    QVector<CustomScene*> scenes;
    QStringList fileNames;
    QString dirName;
    QRectF baseRect;


    int pageIndex;
    float resizeFactor;


private slots:
    void on_actionOpen_Image_form_png_jpg_triggered();
    void resizeEvent(QResizeEvent* );

    void on_resizeFactor_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_4_pressed();

    void on_pushButton_5_pressed();

    void on_select_toolButton_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void on_pageIndex_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_rubberBand_finished();

    void keyPressEvent(QKeyEvent *event);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    void UpdateView();
    void closeScenes();


};
#endif // MAINWINDOW_H
