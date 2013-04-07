#ifndef CONTROLLISTITEM_H
#define CONTROLLISTITEM_H
#include "listmodel.h"
#include <QSize>
#include <QString>

//class Object : public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(QString theChange READ getTheChange NOTIFY changeOfStatus)

//public:
//    Object()
//    {
//        changeMe = false;
//        myTimer = new QTimer(this);
//        myTimer->start(5000);
//        connect(myTimer, SIGNAL(timeout()), this, SLOT(testSlot()));
//    }

//    QString getTheChange()
//    {
//        if (theValue == 0)
//        {
//            return "The text changed";
//        }
//        if (theValue == 1)
//        {
//            return "New text change";
//        }
//        return "nothing has happened yet";

//    }

//    Q_INVOKABLE void someFunction(int i)
//    {
//        if ( i == 0) {
//            theValue = 0;
//        }
//        if (i == 1) {
//            theValue = 1;
//        }

//        emit changeOfStatus(i);
//    }

//signals:
//    void changeOfStatus(int i) ;
//public slots:
//    void testSlot()
//    {
//        if (changeMe)
//        {
//            someFunction(0);
//        } else {
//            someFunction(1);
//        }
//        changeMe = !changeMe;
//    }

//private:
//    bool changeMe;
//    int theValue;
//    QTimer *myTimer;

//};


class StatusObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY changeOfStatus)

public:
    StatusObject(QObject *parent=0) : QObject(parent) { state=false; }
    bool isActive(void) { return state; }
    Q_INVOKABLE invokeActive(bool newstate) { state=newstate; }

signals:
    void changeOfStatus(bool b);

public slots:
    void ActiveSlot(newstate) { state=newstate; }
private:
    bool state;
};

class ControlListItem : public ListItem
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole+1,
        SizeRole,
        ImageRole,
        CommandRole,
        SubmenuRole
    };

public:
    explicit ControlListItem(const QString &name, const QSize &size, QString img, QObject *parent = 0);
    explicit ControlListItem(const QString &name, const QSize &size, QString img, QString command, QObject *parent = 0);
    ControlListItem(QObject *parent = 0): ListItem(parent) {}
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    void setImage(QString img);
    inline QString id() const { return m_name; }
    inline QString name() const { return m_name; }
    inline QSize size() const { return m_size; }
    inline QString image() const { return m_img; }
    inline QString cmd() const { return m_command; }

private:
    QString m_name;
    QSize m_size;
    QString m_img;
    QString m_command;

signals:
    
public slots:
    
};

#endif // CONTROLLISTITEM_H
