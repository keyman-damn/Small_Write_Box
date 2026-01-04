#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 添加更新游戏按钮的方法
    void updateGameButtons(const QStringList& gameNames);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;


private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
