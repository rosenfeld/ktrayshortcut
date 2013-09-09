#include "registeredapplication.h"
#include <KSystemTrayIcon>
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

void RegisteredApplication::grabWindow()
{
    Window root = RootWindow(display, screen);
    Cursor cursor = XCreateFontCursor(display, XC_draped_box);

    XGrabPointer(display, root, false,
                 ButtonPressMask | ButtonReleaseMask, GrabModeSync,
                 GrabModeAsync, None, cursor, CurrentTime);
    XAllowEvents(display, SyncPointer, CurrentTime);
    XEvent *event = new XEvent();
    XWindowEvent(display, root, ButtonPressMask, event);
    window = event->xbutton.subwindow ? event->xbutton.subwindow : root;
    XUngrabPointer(display, CurrentTime);
    XFreeCursor(display, cursor);
    window = XmuClientWindow(display, window);

    char **window_icon = 0;
    XWMHints *wm_hints = XGetWMHints(display, window);
    XpmCreateDataFromPixmap(display, &window_icon, wm_hints->icon_pixmap,
                            wm_hints->icon_mask, 0);
    QPixmap appIcon = QPixmap(const_cast<const char **> (window_icon));
    XpmFree(window_icon);
    KSystemTrayIcon *trayIcon = new KSystemTrayIcon(QIcon(appIcon));
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(toggle()));

    XClassHint ch;
    XGetClassHint(display, window, &ch);
    KAction *a = mainWindow->addCustomAction(ch.res_name);
    connect(a, SIGNAL(triggered()), this, SLOT(toggle()));
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
