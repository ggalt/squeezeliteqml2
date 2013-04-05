#ifndef CONTROLLISTITEM_H
#define CONTROLLISTITEM_H
#include "listmodel.h"
#include <QSize>
#include <QString>

class ControlListItem : public ListItem
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole+1,
        SizeRole,
        ImageRole,
        SubmenuRole
    };

public:
    explicit ControlListItem(const QString &name, const QSize &size, QString img, QObject *parent = 0);
    ControlListItem(QObject *parent = 0): ListItem(parent) {}
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    void setImage(QString img);
    inline QString id() const { return m_name; }
    inline QString name() const { return m_name; }
    inline QSize size() const { return m_size; }
    inline QString image() const { return m_img; }

private:
    QString m_name;
    QSize m_size;
    QString m_img;

signals:
    
public slots:
    
};

#endif // CONTROLLISTITEM_H
