#include <QVariant>
#include "controllistitem.h"


ControlListItem::ControlListItem(const QString &name, const QSize &size, QString img, QObject *parent) : ListItem(parent)
{
    m_name = name;
    m_size = size;
    m_img = img;
    m_command = name;
}

ControlListItem::ControlListItem(const QString &name, const QSize &size, QString img, QString command, QObject *parent) : ListItem(parent)
{
    m_name = name;
    m_size = size;
    m_img = img;
    m_command = command;
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
    case CommandRole:
        return cmd();
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
    names[CommandRole] = "command";
    return names;
}

void ControlListItem::setImage(QString img)
{
    m_img=img;
    emit dataChanged();
}
