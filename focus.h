#ifndef FOCUS_H
#define FOCUS_H

#include <QWidget>

namespace Ui {
class Focus;
}

class Focus : public QWidget
{
    Q_OBJECT

public:
    explicit Focus(QWidget *parent = nullptr);
    ~Focus();

private:
    Ui::Focus *ui;
};

#endif // FOCUS_H
