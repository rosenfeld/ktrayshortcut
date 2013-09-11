#ifndef REGISTEREDAPPLICATION_H
#define REGISTEREDAPPLICATION_H

#include "mainwindow.h"
#include <QObject>
#include <KSystemTrayIcon>
#include <X11/Xlib.h>

class RegisteredApplication : public QObject
{
    Q_OBJECT
public:
    explicit RegisteredApplication(MainWindow *mainWindow);
    Window grabWindow();
    void unregister();
    KAction *action;

signals:

public slots:
    void toggle();

private:
    MainWindow *mainWindow;
    Window window;
    Display *display;
    int screen;
    bool minimized;
    KSystemTrayIcon *trayIcon;
};

#endif // REGISTEREDAPPLICATION_H
