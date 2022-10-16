#include "customscene.h"
#include <QDebug>

#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MAX(a,b) ((((a) > (b))) ? (a) : (b))
#define MIN(a,b) ((((a) < (b))) ? (a) : (b))


CustomScene::CustomScene(QWidget* parent)
{
    this->setParent(parent);
    this->parent = parent;
    isRubberBandActive = false;
    rubberBands = QVector<QGraphicsRectItem*>();
    stateIndex = QList<QPair<int,int>>();
    undoIndex = QList<QPair<int,int>>();
    tempElipsePair = ElipsePair();
//    statusBar = new QStatusBar(this->parent);
//    statusBar->show();
}

CustomScene::~CustomScene(){
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //statusBar->showMessage(QString("(%1,%2)").arg(event->scenePos().x()).arg(event->scenePos().y()));
    if (!isRubberBandActive){
        return;
    }

    origin = event->scenePos().toPoint();

    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(origin.x()-20, origin.y()-20,40,40);
    item->setBrush(Qt::green);
    tempElipsePair.setFirst(item);
    this->addItem(item);

    qDebug() << origin;
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent  *event)
{
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent  *event)
{
    if (!isRubberBandActive){
        return;
    }
    destination = event->scenePos().toPoint();

    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(destination.x()-20, destination.y()-20,40,40);
    item->setBrush(Qt::green);
    tempElipsePair.setSecond(item);
    this->addItem(item);

    QGraphicsRectItem* rubberBand = new QGraphicsRectItem(MIN(origin.x(), destination.x()),
                                                          MIN(origin.y(), destination.y()), ABS(origin.x()- destination.x()), ABS(origin.y()- destination.y()));
    rubberBands.append(rubberBand);
    this->addItem(rubberBand);
    isRubberBandActive = false;


    //state management
    stateIndex.append(QPair<int,int>(rubberBands.size()-1,1));
    if (!undoIndex.isEmpty()){
        undoIndex.clear();
    }

    emit rubberBandFinished();
}

void CustomScene::keyPressEvent(QKeyEvent* e)
{
//    if (e->key() != Qt::Key_Escape){
//        return;
//    }
//    if (!isRubberBandActive){
//        return;
//    }
//    isRubberBandActive = false;
//    emit rubberBandFinished();
}

void CustomScene::unDo(){
    QPair<int,int> lastState = stateIndex.last();
    this->removeItem(rubberBands.at(lastState.first));
    //state management
    undoIndex.append(QPair<int,int>(lastState.first,-lastState.second));
    stateIndex.remove(stateIndex.size()-1);
}



void CustomScene::reDo(){
    if (undoIndex.isEmpty()){
        return;
    }
    QPair<int,int> lastState = undoIndex.last();
    this->addItem(rubberBands.at(lastState.first));
    //state management
    undoIndex.remove(undoIndex.size()-1);
    stateIndex.append(QPair<int,int>(lastState.first,- lastState.second));
}
