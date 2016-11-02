#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDir>
#include <QDirIterator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _scene = new CorrectorGraphicsScene();
    ui->graphicsView->setScene(_scene);
    _img = new QGraphicsPixmapItem();
    _scene->addItem(_img);
    connect(_scene, SIGNAL(bboxChanged(QRect)), this, SLOT(update_bbox(QRect)));
    ui->classComboBox->addItems(DatasetObject::classnames);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _scene;
    delete _img;
}

void MainWindow::open_dataset_dir()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Open directory"),
                                                        QString("/home/kwon-young/prog/save_dmos"));
    if(!dirname.isNull()) {
        load_dir(dirname);
    }
}

void MainWindow::load_dir(QString dirname)
{
  _imgs.clear();
  QDirIterator it(dirname, QStringList() << "*.jpg", QDir::Files);
  QDir rootdir(dirname);
  while (it.hasNext()) {
    QString filename = rootdir.absoluteFilePath(it.next());
    if (rootdir.exists(filename))
        _imgs.append(DatasetObject(filename));
  }
  qDebug() << "opened " << _imgs.size() << " images.";
  setImage(0);
}

void MainWindow::setImage(int index)
{
    _cur_img = index;
    _imgs[index].loadImg();
    _img->setPixmap(QPixmap::fromImage(_imgs[index].getImg()));
    _img->setZValue(1);
    _img->show();
    QRect bbox = _imgs[_cur_img].rel_bbox();
    if (!bbox.isNull()) {
      _scene->setRect(bbox);
      _scene->showRect();
    } else {
        _scene->hideRect();
    }
    ui->classComboBox->setCurrentIndex(DatasetObject::classnames.indexOf(_imgs[index].classname()));
}

void MainWindow::save_image()
{
    // also set the class name
    _imgs[_cur_img].saveImg();
}

void MainWindow::next_image()
{
    if (_cur_img < _imgs.size()-1)
      setImage(++_cur_img);
}

void MainWindow::previous_image()
{
    if (_cur_img > 0)
        setImage(--_cur_img);
}

void MainWindow::update_bbox(const QRect &bbox)
{
    _imgs[_cur_img].setRel_bbox(bbox);
}

void MainWindow::on_classComboBox_activated(const QString &classname)
{
    _imgs[_cur_img].setClassname(classname);
}
