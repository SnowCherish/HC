#ifndef REDIS_H
#define REDIS_H

#include <QObject>

class Redis : public QObject
{
    Q_OBJECT
public:
    explicit Redis(QObject *parent = 0);

signals:

public slots:
};

#endif // REDIS_H