#ifndef REGISTEREDAPPLICATION_H
#define REGISTEREDAPPLICATION_H

#include "mainwindow.h"
#include <QObject>
#include <X11/Xlib.h>

class RegisteredApplication : public QObject
{
    Q_OBJECT
public:
    explicit RegisteredApplication(MainWindow *mainWindow);
    void grabWindow();

signals:

public slots:
    void toggle();

private:
    MainWindow *mainWindow;
    Window window;
    Display *display;
    int screen;
    bool minimized;
};

#endif // REGISTEREDAPPLICATION_H
