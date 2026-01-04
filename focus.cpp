#include "focus.h"
#include "ui_focus.h"

Focus::Focus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Focus)
{
    ui->setupUi(this);
}

Focus::~Focus()
{
    delete ui;
}
