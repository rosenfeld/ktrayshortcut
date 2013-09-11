#ifndef REGISTEREDAPPLICATION_H
#define REGISTEREDAPPLICATION_H

#include "mainwindow.h"
#include <QObject>
#include <QSystemTrayIcon>
#include <X11/Xlib.h>

class RegisteredApplication : public QObject
{
    Q_OBJECT
public:
    explicit RegisteredApplication(MainWindow *mainWindow);
    virtual ~RegisteredApplication();
    void grabWindow();
    void unregister();
    KAction *action;
    Window window;

signals:

public slots:
    void onTrayClick(QSystemTrayIcon::ActivationReason reason);
    void toggle();
    void remove();

private:
    MainWindow *mainWindow;
    Display *display;
    int screen;
    bool minimized;
    QSystemTrayIcon *trayIcon;
    QMenu *contextMenu;
    void createContextMenu();
};

#endif // REGISTEREDAPPLICATION_H
