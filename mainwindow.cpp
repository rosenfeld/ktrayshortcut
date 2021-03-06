#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registeredapplication.h"

#include <KSystemTrayIcon>


std::map<Window, RegisteredApplication*> appByWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    shortcutsEditor = new KShortcutsEditor(this, KShortcutsEditor::GlobalAction);

    actionCollection = new KActionCollection(this);
    action = addCustomAction("main settings window");
    connect(action, SIGNAL(triggered()), SLOT(toggle()));

    ui->gridLayout->addWidget(shortcutsEditor);

    (new KSystemTrayIcon("configure", this))->show();

    connect(ui->findApplicationButton, SIGNAL(clicked()), this, SLOT(findApplicationClick()));
}

MainWindow::~MainWindow()
{
    std::map<Window, RegisteredApplication*>::iterator iter;
    shortcutsEditor->save();
    for (iter = appByWindow.begin(); iter != appByWindow.end(); ++iter)
        iter->second->unregister();
    delete shortcutsEditor;
    delete actionCollection;
    delete ui;
}

KAction* MainWindow::addCustomAction(const QString &name) {
    shortcutsEditor->save();
    shortcutsEditor->clearCollections();
    KAction* action = actionCollection->addAction(name);
    action->setText(name);
    action->setGlobalShortcut(KShortcut(),
                              KAction::ActiveShortcut | KAction::DefaultShortcut,
                              KAction::NoAutoloading);
    shortcutsEditor->addCollection(actionCollection);
    return action;
}

void MainWindow::toggle() {
    if (isHidden()) show();
    else hide();
}

void MainWindow::findApplicationClick() {
    RegisteredApplication *app = new RegisteredApplication(this);
    app->grabWindow();
    appByWindow[app->window] = app;
}

void MainWindow::x11EventFilter(XEvent *event)
{
    if (event->type != DestroyNotify) return;
    Window window = event->xclient.window;
    if (!appByWindow.count(window)) return;
    RegisteredApplication *app = appByWindow[window];
    removeApplication(app);
    delete app;
}

void MainWindow::removeApplication(RegisteredApplication *app)
{
    shortcutsEditor->save();
    app->unregister();
    shortcutsEditor->clearCollections();
    actionCollection->removeAction(app->action);
    appByWindow.erase(app->window);

    shortcutsEditor->addCollection(actionCollection);
}
