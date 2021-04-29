#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QTcpSocket>
#include <QFileDialog>
#include <QtSql>

#include <memory>
#include <vector>
#include <fstream>

#include "tinyxml2.h"
#include "popup.h"
#include "base64.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_login_2_clicked();

    void slotReadyRead   ();
    void slotError       (QAbstractSocket::SocketError);
    void slotConnected   ();

    void on_to_register_clicked();

    void on_to_login_clicked();

    void on_register_2_clicked();

    void on_reg_img_clicked();

private:
    enum class screen
    {
        login = 0,
        registration = 1,
        personal_area = 2,
        patterns = 3
    };
    enum class type_forward
    {
        login,
        registration
    };

    Ui::MainWindow *ui;
    Popup *popup;
    QSqlDatabase db;
    type_forward forward;
    QString base64_file;
    QString file_type;

    std::unique_ptr<QTcpSocket> socket;
    const QString host{"localhost"};
    const quint16 port{20002};

    void send(const QString& data);
    QString recv();
    void create_db();

    void fill_db_login(tinyxml2::XMLElement* body);
    void save_img_to_file(const QString& path, const QString& img);
};
#endif // MAINWINDOW_H
