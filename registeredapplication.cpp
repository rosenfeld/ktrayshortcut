#include "registeredapplication.h"
#include <QPixmap>
#include <X11/cursorfont.h>
#include <QX11Info>
#include <X11/Xmu/WinUtil.h>
#include <X11/xpm.h>
#include <QMenu>

RegisteredApplication::RegisteredApplication(MainWindow *mainWindow)
{
    minimized = false;
    this->mainWindow = mainWindow;
    display = QX11Info::display();
    screen = DefaultScreen(display);
    root = RootWindow(display, screen);
    createContextMenu();
}

RegisteredApplication::~RegisteredApplication()
{
    delete contextMenu;
}

void RegisteredApplication::createContextMenu()
{
    contextMenu = new QMenu();
    contextMenu->addAction("Undock", this, SLOT(remove()));
}

void RegisteredApplication::grabWindow()
{
    Cursor cursor = XCreateFontCursor(display, XC_draped_box);

    XGrabPointer(display, root, false,
                 ButtonPressMask | ButtonReleaseMask, GrabModeSync,
                 GrabModeAsync, None, cursor, CurrentTime);
    XAllowEvents(display, SyncPointer, CurrentTime);
    XEvent event;
    XWindowEvent(display, root, ButtonPressMask, &event);
    window = event.xbutton.subwindow ? event.xbutton.subwindow : root;
    XUngrabPointer(display, CurrentTime);
    XFreeCursor(display, cursor);
    window = XmuClientWindow(display, window);

    XWindowAttributes attr;
    XGetWindowAttributes(display, window, &attr);
    XSelectInput(display, window, attr.your_event_mask | StructureNotifyMask);

    char **window_icon = 0;
    XWMHints *wm_hints = XGetWMHints(display, window);
    XpmCreateDataFromPixmap(display, &window_icon, wm_hints->icon_pixmap,
                            wm_hints->icon_mask, 0);
    QPixmap appIcon = QPixmap(const_cast<const char **> (window_icon));
    XpmFree(window_icon);
    trayIcon = new QSystemTrayIcon(QIcon(appIcon));
    trayIcon->setContextMenu(contextMenu);
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onTrayClick(QSystemTrayIcon::ActivationReason)));

    XClassHint ch;
    XGetClassHint(display, window, &ch);
    action = mainWindow->addCustomAction(ch.res_name);
    connect(action, SIGNAL(triggered()), this, SLOT(toggle()));
}


void RegisteredApplication::onTrayClick(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) toggle();
}

void RegisteredApplication::toggle()
{
    if (minimized) {
        XMapRaised(display, window);
        sendActiveWindowEvent();
        XSetInputFocus(display, window, RevertToParent, CurrentTime);
    } else {
        XIconifyWindow(display, window, screen);
        XWithdrawWindow(display, window, screen);
    }
    minimized = !minimized;
}

void RegisteredApplication::sendActiveWindowEvent() {
    XEvent e;
    memset(&e, 0, sizeof(XEvent));
    e.xclient.window = window;
    e.xclient.type = ClientMessage;
    e.xclient.message_type = XInternAtom(display, "_NET_ACTIVE_WINDOW", true);
    e.xclient.format = 32;
    long data[2] = {2, CurrentTime};
    memcpy((void *) &e.xclient.data, (void *) data, sizeof(data));
    XSendEvent(display, root, false,
               SubstructureNotifyMask | SubstructureRedirectMask, &e);
    XSync(display, false);
}

void RegisteredApplication::unregister()
{
    // unregister X11 close event for this window
    XWindowAttributes attr;
    XGetWindowAttributes(display, window, &attr);
    XSelectInput(display, window, attr.your_event_mask & ~StructureNotifyMask);

    if (minimized) toggle();
    action->forgetGlobalShortcut();
    trayIcon->hide();
    delete trayIcon;
}

void RegisteredApplication::remove()
{
    mainWindow->removeApplication(this);
    deleteLater();
}
