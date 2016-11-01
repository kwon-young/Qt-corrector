#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QGraphicsPixmapItem>

#include "datasetobject.h"
#include "correctorgraphicsscene.h"

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
    CorrectorGraphicsScene * _scene;
    QGraphicsPixmapItem * _img;
};

#endif // MAINWINDOW_H
