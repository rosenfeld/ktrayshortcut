#include "registeredapplication.h"
#include <QPixmap>
#include <X11/cursorfont.h>
#include <QX11Info>
#include <X11/Xmu/WinUtil.h>
#include <X11/xpm.h>

RegisteredApplication::RegisteredApplication(MainWindow *mainWindow)
{
    minimized = false;
    this->mainWindow = mainWindow;
    display = QX11Info::display();
    screen = DefaultScreen(display);
}

Window RegisteredApplication::grabWindow()
{
    Window root = RootWindow(display, screen);
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
    trayIcon = new KSystemTrayIcon(QIcon(appIcon));
    trayIcon->actionCollection()->clear();
    // TODO: add an "undock" context menu entry
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(toggle()));

    XClassHint ch;
    XGetClassHint(display, window, &ch);
    action = mainWindow->addCustomAction(ch.res_name);
    connect(action, SIGNAL(triggered()), this, SLOT(toggle()));

    return window;
}

void RegisteredApplication::toggle()
{
    if (minimized) {
        XMapRaised(display, window);
        XSetInputFocus(display, window, RevertToParent, CurrentTime);
    } else {
        XIconifyWindow(display, window, screen);
        XWithdrawWindow(display, window, screen);
    }
    minimized = !minimized;
}

void RegisteredApplication::unregister()
{
    if (minimized) toggle();
    action->forgetGlobalShortcut();
    trayIcon->hide();
    delete trayIcon;
}
