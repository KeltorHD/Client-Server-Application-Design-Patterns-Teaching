#ifndef POPUP_H
#define POPUP_H

#include <QDialog>

namespace Ui {
class Popup;
}

class Popup : public QDialog
{
    Q_OBJECT

public:
    explicit Popup(QWidget *parent = nullptr);
    ~Popup();

    void set_title(const QString& title);
    void set_description(const QString& desc);

private:
    Ui::Popup *ui;
};

#endif // POPUP_H
