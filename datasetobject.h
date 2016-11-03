#ifndef DATASETOBJECT_H
#define DATASETOBJECT_H

#include <QString>
#include <QStringList>
#include <QImage>
#include <QRect>
#include <QDir>

class DatasetObject
{
public:
    DatasetObject(const QString &filename);

    QString filename() const;
    void setFilename(const QString &filename);

    QString basename() const;

    QString pagename() const;
    void setPagename(const QString &pagename);

    QString classname() const;
    void setClassname(const QString &classname);

    QString thumb_bbox_str() const;
    void setThumb_bbox_str(const QString &thumb_bbox_str);

    QString symbol_bbox_str() const;
    void setSymbol_bbox_str(const QString &symbol_bbox_str);

    QRect thumb_bbox() const;
    void setThumb_bbox(const QRect &thumb_bbox);

    QRect symbol_bbox() const;
    void setSymbol_bbox(const QRect &symbol_bbox);

    QImage getImg() const;
    void setImg(const QImage &img);

    void loadImg();
    bool saveImg();

    QString getInfo(int index) const;
    void setInfo(const QString &info, int index);

    QRect rel_bbox() const;
    void setRel_bbox(const QRect &bbox);

    bool isModified() const;

    static QRect str2bbox(const QString &bbox_str);
    static QString bbox2str(const QRect &bbox);

    static QStringList classnames;
private:

    QString _filename;
    QString _backupname;
    QImage _img;

    static QString _sep_info;
    static QString _sep_bbox;
    static int page_idx;
    static int class_idx;
    static int thumb_bbox_idx;
    static int symbol_bbox_idx;
};

#endif // DATASETOBJECT_H
