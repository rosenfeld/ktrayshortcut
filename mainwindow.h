#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <KShortcutsEditor>
#include <KAction>
#include <KActionCollection>

class RegisteredApplication; // forward declaration

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    KAction* addCustomAction(const QString &name);
    void x11EventFilter(XEvent *event);
    void removeApplication(RegisteredApplication *app);

public slots:
    void toggle();
    void findApplicationClick();

private:
    Ui::MainWindow *ui;
    KShortcutsEditor *shortcutsEditor;
    KActionCollection *actionCollection;
    KAction *action;
};

#endif // MAINWINDOW_H
