#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QModelIndex>

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
    ui->indexSpinBox->setMinimum(0);
    ui->indexSpinBox->setMaximum(_imgs.size());
    ui->indexSpinBox->setSingleStep(1);
    _imgs_listmodel = new QStringListModel();
    ui->imgListView->setModel(_imgs_listmodel);
    QItemSelectionModel * selectmodel = ui->imgListView->selectionModel();
    connect(selectmodel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(img_selection_changed(QItemSelection,QItemSelection)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _scene;
    //delete _img;
    delete _imgs_listmodel;
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
  ui->indexSpinBox->setMaximum(_imgs.size());
  QStringList img_names;
  for (auto img : _imgs)
      img_names.append(img.basename());
  _imgs_listmodel->setStringList(img_names);
  setImage(0);
}

void MainWindow::set_windowTitle()
{
    QString title = _imgs[_cur_img].basename();
    if (_imgs[_cur_img].isModified())
        title += " *";
    setWindowTitle(title);
}

void MainWindow::setImage(int index)
{
    _cur_img = index;
    _imgs[index].loadImg();
    _img->setPixmap(QPixmap::fromImage(_imgs[index].getImg()));
    _img->setZValue(1);
    _img->show();
    QRect bbox = _imgs[_cur_img].rel_bbox();
    _scene->setRect(bbox);
    if (!bbox.isNull()) {
      _scene->showRect();
    } else {
        _scene->hideRect();
    }
    ui->classComboBox->setCurrentIndex(DatasetObject::classnames.indexOf(_imgs[index].classname()));
    ui->indexSpinBox->setValue(index);
    ui->imgListView->setCurrentIndex(_imgs_listmodel->index(index, 0));
    set_windowTitle();
}

void MainWindow::img_selection_changed(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    const QModelIndex index = ui->imgListView->selectionModel()->currentIndex();
    setImage(index.row());
}

void MainWindow::save_image()
{
    // also set the class name
    _imgs[_cur_img].saveImg();
    QStringList sl = _imgs_listmodel->stringList();
    sl[_cur_img] = _imgs[_cur_img].basename();
    _imgs_listmodel->setStringList(sl);
    ui->imgListView->setCurrentIndex(_imgs_listmodel->index(_cur_img, 0));
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
    set_windowTitle();
}

void MainWindow::on_classComboBox_activated(const QString &classname)
{
    _imgs[_cur_img].setClassname(classname);
    set_windowTitle();
}
