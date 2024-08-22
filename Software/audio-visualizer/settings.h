#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMutex>
#include <QObject>

/**
 * The Settings class
 * @brief Singleton
 * @author Marijan Kovač
 */
class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

signals:
};

#endif // SETTINGS_H
