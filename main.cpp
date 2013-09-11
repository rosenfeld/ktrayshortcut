#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include "mainwindow.h"

class MyApplication : public KApplication {
public:
    void setMain(MainWindow *main) {
        this->main = main;
        setTopWidget(main);
    }

private:
    bool x11EventFilter(XEvent *ev) {
        main->x11EventFilter(ev);
        return false;
    }
    MainWindow *main;
};

int main(int argc, char *argv[])
{
    KAboutData aboutData("ktrayshortcut", 0, ki18n("ktrayshortcut"), "0.1");
    KCmdLineArgs::init(argc, argv, &aboutData);
    MyApplication app;
    MainWindow main;
    main.show();
    app.setMain(&main);

    return app.exec();
}

