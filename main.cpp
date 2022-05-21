#include <QApplication>

#include <QObject>

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>

#include <QPen>
#include <QColor>
#include <QPixmap>
#include <QTimer>

// RGB (red green blue) can represent more 16 millions different colors
// (255, 0, 0) stands for red
// (0, 0, 255) stands for blue
const QPen redPen(QColor(255, 0, 0));
const QPen bluePen(QColor(0, 0, 255));

constexpr int SceneHeight = 500;
constexpr int SceneWidth = 500;

void drawAxes(QGraphicsScene * scene, const QPen & pen); // realization is at the bottom of the file

/*
 * Draw a rectangle of width=50 and height=50 whose left top corner is
 * shifted along x axis by 40 and along y axis by 10.
 */
void example1() {
    QGraphicsScene * scene = new QGraphicsScene(0,0,SceneWidth, SceneHeight);

    // Don't bother with this function. It is only needed to demonstrate the coordinates.
    drawAxes(scene, redPen);

    scene->addRect(40,10,50,50, bluePen);

    QGraphicsView * view = new QGraphicsView(scene);
    view->show();
}

/*
 * Draw a rectangle of width=50 and height=50 whose left top corner is
 * shifted along x axis by 40 and along y axis by 10.
 *
 * It draws absolutely the same picture as the first example, but
 * this time we use our own custom class to represent the rectangle.
 */
void example2() {
    class MyRect : public QGraphicsRectItem {
    public:
        MyRect() {
            setPen(bluePen);
            setRect(40,10,50,50);
        }
    };

    QGraphicsScene * scene = new QGraphicsScene(0,0,SceneWidth, SceneHeight);

    drawAxes(scene, redPen);

    MyRect * rect = new MyRect();
    scene->addItem(rect);

    QGraphicsView * view = new QGraphicsView(scene);
    view->show();
}

/*
 * Put a picture on the scene. Use QT resource system and
 * QGraphicsPixmapItem.
 */
void example3() {
    QGraphicsScene * scene = new QGraphicsScene(0,0,SceneWidth, SceneHeight);

    drawAxes(scene, redPen);

    QGraphicsPixmapItem * pixmap = new QGraphicsPixmapItem();
    pixmap->setPixmap(QPixmap(":/img/res/smile.png"));
    pixmap->setScale(0.5);
    scene->addItem(pixmap);

    QGraphicsView * view = new QGraphicsView(scene);
    view->show();
}

/*
 * Create a rectangle that moves every time the timer timeouts.
 */
void example4() {
    QGraphicsScene * scene = new QGraphicsScene(0,0,SceneWidth, SceneHeight);
    drawAxes(scene, redPen);

    QGraphicsRectItem * rect = new QGraphicsRectItem(0,0,50,50);
    rect->setPen(bluePen);

    scene->addItem(rect);

    QTimer * timer = new QTimer;

    // !!!important to copy the pointer, otherwise an error occurs
    QObject::connect(timer, &QTimer::timeout, [rect]() {
        // moveBy is equivalent to calling setPos(pos() + QPointF(dx, dy))
        rect->moveBy(10,10);
    });

    timer->start(200);

    QGraphicsView * view = new QGraphicsView(scene);
    view->show();
}

/*
 * Draw a rect that bounce back every time it hits a border
 * of a scene.
 */
void example5() {
    QGraphicsScene * scene = new QGraphicsScene(0,0,SceneWidth, SceneHeight);
    drawAxes(scene, redPen);

    QGraphicsRectItem * rect = new QGraphicsRectItem(0,0,50,50);
    rect->setBrush(Qt::GlobalColor::red);

    scene->addItem(rect);
    scene->setBackgroundBrush(QBrush(QColor(Qt::GlobalColor::darkBlue)));

    QTimer * timer = new QTimer;

    std::pair<double, double> * direction = new std::pair{1.5, 3.5};
    // !!!it is important to copy the pointer, otherwise an error occurs
    QObject::connect(timer, &QTimer::timeout, [rect, scene, direction]() {
        // rect hits right or left border
        if (rect->x() + rect->rect().width() + direction->first > scene->sceneRect().width()
            || rect->x() + direction->first < 0) {
            direction->first *= -1;
            rect->setBrush(QBrush(QColor(rand() % 255, rand() % 255, rand() % 255)));
        }
        // rect hits top or bottom border
        if (rect->y() + rect->rect().height() + direction->second > scene->sceneRect().height()
            || rect->y() + direction->second < 0) {
            direction->second *= -1;
            rect->setBrush(QBrush(QColor(rand() % 255, rand() % 255, rand() % 255)));
        }

        rect->moveBy(direction->first, direction->second);
    });

    timer->start(1000/60);

    QGraphicsView * view = new QGraphicsView(scene);
    view->show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    example5();
    return a.exec();
}

/*
 * Draw axes on the scene using the pen provided.
 */
void drawAxes(QGraphicsScene * scene, const QPen & pen) {
    scene->addLine(-scene->sceneRect().width(),0,scene->sceneRect().width(),0,pen);
    scene->addLine(0,-scene->sceneRect().height(),0,scene->sceneRect().height(),pen);
    for (int i = 10; i < scene->sceneRect().width(); i += 10) {
        scene->addLine(i, -5, i, 5, pen);
        scene->addLine(-i, -5, -i, 5, pen);

        scene->addLine(-5, i, 5, i, pen);
        scene->addLine(-5, -i, 5, -i, pen);
    }
};
