#include <QVariant>
#include "controllistitem.h"


ControlListItem::ControlListItem(const QString &name, const QSize &size, QString img, QObject *parent) : ListItem(parent)
{
    m_name = name;
    m_size = size;
    m_img = img;
}

QVariant ControlListItem::data(int role) const
{
    switch(role){
    case NameRole:
        return name();
    case SizeRole:
        return size();
    case ImageRole:
        return image();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ControlListItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[SizeRole] = "size";
    names[ImageRole] = "image";
    return names;
}

void ControlListItem::setImage(QString img)
{
    m_img=img;
    emit dataChanged();
}
