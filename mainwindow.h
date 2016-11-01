#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QRubberBand>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

#include "datasetobject.h"
#include "graphicsbboxitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void open_dataset_dir();
    void save_image();
    void next_image();
    void previous_image();

private slots:


private:
    void load_dir(QString dirname);
    void setImage(int index);
    Ui::MainWindow *ui;
    QList<DatasetObject> _imgs;
    int _cur_img;
    QGraphicsScene * _scene;
    QGraphicsPixmapItem * _img;
    GraphicsBBoxItem * _rect;
};

#endif // MAINWINDOW_H
