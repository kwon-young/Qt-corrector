#include "datasetobject.h"

#include <QStringList>
#include <QPoint>
#include <QSize>
#include <QImage>
#include <QImageReader>
#include <QFileInfo>
#include <QFile>

QString DatasetObject::_sep_info = "_";
QString DatasetObject::_sep_bbox = "x";
int DatasetObject::page_idx = 0;
int DatasetObject::class_idx = 1;
int DatasetObject::thumb_bbox_idx = 2;
int DatasetObject::symbol_bbox_idx = 3;
QStringList DatasetObject::classnames = QStringList()
        << "JUNK" << "DIESE" << "BECARRE" << "BEMOL" << "DDIESE";

DatasetObject::DatasetObject(const QString &filename):
    _filename(filename),
    _backupname(filename)
{
}

QString DatasetObject::pagename() const
{
    return getInfo(page_idx);
}

void DatasetObject::setPagename(const QString &pagename)
{
    setInfo(pagename, page_idx);
}

QString DatasetObject::thumb_bbox_str() const
{
    return getInfo(thumb_bbox_idx);
}

void DatasetObject::setThumb_bbox_str(const QString &thumb_bbox_str)
{
    setInfo(thumb_bbox_str, thumb_bbox_idx);
}

QString DatasetObject::symbol_bbox_str() const
{
    return getInfo(symbol_bbox_idx);
}

void DatasetObject::setSymbol_bbox_str(const QString &symbol_bbox_str)
{
    setInfo(symbol_bbox_str, symbol_bbox_idx);
}

QRect DatasetObject::thumb_bbox() const
{
    QString thumb = thumb_bbox_str();
    return str2bbox(thumb);
}

void DatasetObject::setThumb_bbox(const QRect &thumb_bbox)
{
    setInfo(bbox2str(thumb_bbox), thumb_bbox_idx);
}

QRect DatasetObject::symbol_bbox() const
{
    QString symbol = symbol_bbox_str();
    return str2bbox(symbol);
}

void DatasetObject::setSymbol_bbox(const QRect &symbol_bbox)
{
    setInfo(bbox2str(symbol_bbox), symbol_bbox_idx);
}

void DatasetObject::loadImg()
{
    if (_img.isNull()) {
      QImageReader reader(_filename);
      reader.setAutoTransform(true);
      _img = reader.read();
    }
}

bool DatasetObject::saveImg()
{
    bool res = QFile::rename(_backupname, _filename);
    _backupname = _filename;
    return res;
}

QString DatasetObject::getInfo(int index) const
{
    QString tail = QFileInfo(_filename).baseName();
    QStringList infos = tail.split(_sep_info);
    return infos[index];
}

void DatasetObject::setInfo(const QString &info, int index)
{
    QFileInfo fi(_filename);
    QString head = fi.dir().path();
    QString basename = fi.baseName();
    QString ext = fi.completeSuffix();
    QStringList infos = basename.split(_sep_info);
    infos[index] = info;
    basename = infos.join(_sep_info);
    _filename = head + QDir::separator() + basename + "." + ext;
}

QRect DatasetObject::rel_bbox() const
{
    QRect rect;
    //if (classname() != classnames[0]) {
    if (!symbol_bbox().isEmpty()) {
      QPoint topleft = symbol_bbox().topLeft() - thumb_bbox().topLeft();
      rect.setTopLeft(topleft);
      rect.setSize(symbol_bbox().size());
    }
    return rect;
}

void DatasetObject::setRel_bbox(const QRect &bbox)
{
    QRect mybbox = bbox;
    if (!bbox.isEmpty())
        mybbox.translate(thumb_bbox().topLeft());
    setSymbol_bbox(mybbox);
}

bool DatasetObject::isModified() const
{
    return _filename.compare(_backupname) != 0;
}

QRect DatasetObject::str2bbox(const QString &bbox_str)
{
    QStringList coords = bbox_str.split(_sep_bbox);
    QPoint topleft(coords[0].toInt(), coords[1].toInt());
    QPoint bottomright(coords[2].toInt(), coords[3].toInt());
    QPoint diff = bottomright - topleft;
    QSize size(diff.x(), diff.y());
    QRect r(topleft, bottomright);
    r.setSize(size);
    return r;
}

QString DatasetObject::bbox2str(const QRect &bbox)
{
    QString bbox_str = ("%1x%2x%3x%4");
    bbox_str = bbox_str.arg(bbox.x()).arg(bbox.y())
        .arg(bbox.x()+bbox.width())
        .arg(bbox.y()+bbox.height());
    return bbox_str;
}

QImage DatasetObject::getImg() const
{
    return _img;
}

void DatasetObject::setImg(const QImage &img)
{
    _img = img;
}

QString DatasetObject::filename() const
{
    return _filename;
}

void DatasetObject::setFilename(const QString &filename)
{
    _filename = filename;
}

QString DatasetObject::backupname() const
{
    return _backupname;
}

QString DatasetObject::basename() const
{
    QFileInfo fi(_filename);
    return fi.baseName();
}

QString DatasetObject::classname() const
{
    return getInfo(class_idx);
}

void DatasetObject::setClassname(const QString &classname)
{
    setInfo(classname, class_idx);
}
