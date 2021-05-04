#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QTcpSocket>
#include <QFileDialog>
#include <QtSql>
#include <QPixmap>
#include <QDir>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <memory>
#include <vector>
#include <fstream>

#include "tinyxml2.h"
#include "popup.h"
#include "base64.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct user_info_t
{
    QString login;
    QString password;
    QString path_to_image;
};

struct result_test_t
{
    QString name;
    QString result;
};

struct pattern_info_t
{
    QString name;
    QString description;
    QString code;
    QString path_to_image;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*открытие подробного описания паттерна*/
    void on_more_btn_clicked(const QString& name);
    /*открытие теста по паттерну*/
    void on_to_test_btn_clicked(const QString& name);

private slots:
    void on_login_2_clicked();

    void slotReadyRead   ();
    void slotError       (QAbstractSocket::SocketError);
    void slotConnected   ();

    void on_to_register_clicked();

    void on_to_login_clicked();

    void on_register_2_clicked();

    void on_reg_img_clicked();

    void on_quit_clicked();

    void on_to_result_test_clicked();

    void on_to_personal_area_clicked();

    void on_load_result_clicked();

    void on_upload_result_clicked();

    void on_to_patterns_clicked();

    void on_to_personal_area_2_clicked();

    void on_update_patterns_clicked();

    void on_to_pattern_list_clicked();

private:
    enum class screen
    {
        login = 0,
        registration = 1,
        personal_area = 2,
        patterns = 3,
        result_test = 4,
        more_pattern = 5,
        test_pattern = 6
    };
    enum class type_forward
    {
        login,
        registration,
        load_result,   /*загрузка результатов тестов с сервера*/
        upload_result, /*отгрузка данных с клиента на сервер*/
        load_patterns  /*загрузка доступных паттернов с сайта*/
    };

    Ui::MainWindow *ui;
    Popup *popup;
    QPixmap profile_pic;
    QPixmap pattern_pic;
    QSqlDatabase db;
    type_forward forward;
    QString base64_file;
    QString file_type;

    /*для результатов тестов*/
    std::vector<QLabel*> label_test;
    std::vector<QFrame*> lines_test;

    /*для списка паттернов*/
    std::vector<QLabel*> pattern_label_list;
    std::vector<QFrame*> pattern_frame_list;
    std::vector<QPushButton*> pattern_btn_more_list;
    std::vector<QPushButton*> pattern_btn_test_list;
    std::vector<QHBoxLayout*> pattern_laoyut_list;

    std::unique_ptr<QTcpSocket> socket;
    const QString host{"localhost"};
    const quint16 port{20002};

    void send(const QString& data);
    QString recv();
    void create_db();

    void send_auth(const QString& login, const QString& password);
    void send_test_result(const QString& login, const QString& password);
    void send_patterns_request();

    void fill_db_login(tinyxml2::XMLElement* body);
    void fill_db_register();
    void fill_db_result_test(tinyxml2::XMLElement* body);
    void fill_db_patterns(tinyxml2::XMLElement* body);
    void fill_personal_area();

    /*заполнение форм данными из базы*/
    void fill_result_test_form();
    void fill_patterns_list_form();

    void save_img_to_file(const QString& path, const QString& img);

    /*выйти из аккаунта*/
    void quit();

    /*выборка из базы данных*/
    user_info_t get_user_info();
    std::vector<result_test_t> get_result_test_info();
    std::vector<QString> get_patterns_name();
    pattern_info_t get_pattern_info(QString name);
};
#endif // MAINWINDOW_H
