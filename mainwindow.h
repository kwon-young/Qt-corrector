#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QGraphicsPixmapItem>
#include <QStringListModel>
#include <QItemSelectionModel>

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
    void update_bbox(const QRect &bbox);
    void on_classComboBox_activated(const QString &classname);
    void setImage(int index);
    void resetImage();
    void img_selection_changed(const QItemSelection &newSelection,const QItemSelection &oldSelection);
    void zoomChange(qreal factor);

private:
    void load_dir(QString dirname);
    void set_windowTitle();

    Ui::MainWindow *ui;
    QList<DatasetObject> _imgs;
    int _cur_img;
    CorrectorGraphicsScene * _scene;
    QGraphicsPixmapItem * _img;
    QStringListModel * _imgs_listmodel;

};

#endif // MAINWINDOW_H
