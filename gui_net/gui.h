#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include "configfile.h"

typedef struct
{
    QString name;
    QString type;
    void *ptr;
}gui_info;

class Gui : public QWidget
{
    Q_OBJECT
public:
    explicit Gui(ConfigFile *configure, QWidget *parent = nullptr);
    QMap<QString, QMap<int, void *>> device_map;

    QMap<int, void *> getDevice(QString str);
    gui_info *get_gui_info_byname(QString name);
    gui_info *get_gui_info_byptr(void *ptr);
signals:

public slots:

private:
    ConfigFile *configure;
};

#endif // GUI_H
