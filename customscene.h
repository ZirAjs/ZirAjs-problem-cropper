#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QVector>
#include <QList>
#include <QPoint>
#include <QRect>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QGraphicsRectItem>
#include <QPair>
#include <QKeyEvent>
#include <QStatusBar>
#include <QImage>
#include <QRubberBand>

class ElipsePair {
  private:
    // Private attribute
    QGraphicsEllipseItem* first;
    QGraphicsEllipseItem* second;

  public:
    // Setter
    void setFirst(QGraphicsEllipseItem* val) {
      if (first){
          delete first;
      }
      first = val;
    }
    void setSecond(QGraphicsEllipseItem* val) {
        if (second){
            delete second;
        }
        second = val;
      }

    // Getter
    QGraphicsEllipseItem* getFirst() {
      return first;
    }

    QGraphicsEllipseItem* getSecond() {
      return second;
    }

    ~ElipsePair(){
        if (first){
            delete first;
        }
        if (second){
            delete second;
        }
    }
};

class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CustomScene(QWidget* =nullptr);
    ~CustomScene();

    QWidget* parent;

    QVector<QGraphicsRectItem*> rubberBands;
    QVector<QRubberBand*> visualRubberBands;
    QRubberBand* visualRubberBand;

    QPoint origin;
    QPoint destination;
    QImage image;

    //QStatusBar* statusBar;

    bool isRubberBandActive;
    bool isVisualRubberBandActive;

    ElipsePair tempElipsePair;

    // recordes state, used for undo redo
    // stored as (item_index, mark)
    // making object appear on the screen is marked as 1
    // making object disappear on the screen is marked as -1
    // redo and undo will add or delete state when undo pressed, last stateindex will be moved to undoindex and if redo is pressed, last undoindex will be move to stateindex
    // undoindex will be cleared when state-changes are made by user
    QList<QPair<int,int>> stateIndex;
    QList<QPair<int,int>> undoIndex;

    void unDo();
    void reDo();

signals:
    void rubberBandFinished();

public slots:

    void mousePressEvent(QGraphicsSceneMouseEvent *);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *);

};

#endif // CUSTOMSCENE_H
