#include "mainwindow.h"
#include "ui_mainwindow.h"

bool fileExists(QString path)
{
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isFile();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->popup = new Popup(this);

    if (!fileExists(QDir::currentPath() + "/main.db"))
    {
        this->ui->screen_stacked->setCurrentIndex((int)screen::login);
        this->forward = type_forward::login;

        this->socket.reset(new QTcpSocket());
        socket->connectToHost(this->host, this->port);
        connect(socket.get(), SIGNAL(connected()), SLOT(slotConnected()));
        connect(socket.get(), SIGNAL(readyRead()), SLOT(slotReadyRead()));
        connect(socket.get(), SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this,  SLOT(slotError(QAbstractSocket::SocketError)));
    }
    else
    {
        this->db = QSqlDatabase::addDatabase("QSQLITE");


        this->db.setDatabaseName(QDir::currentPath() + "/main.db");

        if (!this->db.open())
        {
               qDebug() << this->db.lastError().text();
        }
        this->ui->screen_stacked->setCurrentIndex((int)screen::personal_area);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_login_2_clicked()
{
    if (!this->ui->login_lineEdit->text().size() || !this->ui->password_lineEdit->text().size())
        return;

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.1\" encoding=\"UTF-8\"");
    doc.InsertEndChild(decl);

    tinyxml2::XMLElement* type = doc.NewElement("type");
    type->SetText("authorization");
    doc.InsertEndChild(type);

    tinyxml2::XMLElement* body = doc.NewElement("body");
    doc.InsertEndChild(body);

    tinyxml2::XMLElement* login = doc.NewElement("login");
    login->SetText(this->ui->login_lineEdit->text().toLocal8Bit().data());
    body->InsertEndChild(login);

    tinyxml2::XMLElement* password = doc.NewElement("password");
    password->SetText(this->ui->password_lineEdit->text().toLocal8Bit().data());
    body->InsertEndChild(password);

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);

    qDebug() << printer.CStr();

    this->send(printer.CStr());
}

void MainWindow::slotReadyRead()
{
    qDebug() << "Ready to read!";
    try
    {
        tinyxml2::XMLDocument doc;
        QString data = this->recv();
        doc.Parse(data.toUtf8());
        qDebug() << data;

        switch (this->forward)
        {
        case (type_forward::login):
        {
            auto type = doc.FirstChildElement("body")->FirstChildElement("answer");
            if (type->GetText() == QString("correct"))
            {
                this->ui->screen_stacked->setCurrentIndex((int)screen::personal_area);
                this->create_db();
                this->fill_db_login(doc.FirstChildElement("body"));
            }
            else
            {
                throw "Неверный логин или пароль!";
            }
            break;
        }
        case (type_forward::registration):
        {
            auto type = doc.FirstChildElement("body")->FirstChildElement("answer");
            if (type->GetText() == QString("correct"))
            {
                this->ui->screen_stacked->setCurrentIndex((int)screen::personal_area);
                this->create_db();
            }
            else
            {
                throw "Пользователь с таким логином уже существует!";
            }
            break;
        }
        }

        this->socket->close();
    }
    catch (const char* e)
    {
        this->ui->login_2->setEnabled(false);
        this->ui->register_2->setEnabled(false);
        this->popup->set_title("Произошла ошибка");
        this->popup->set_description(e);
        this->popup->exec();
        this->socket->close();
        socket->connectToHost(this->host, this->port);
    }
    catch (...)
    {
        this->ui->login_2->setEnabled(false);
        this->ui->register_2->setEnabled(false);
        this->popup->set_title("Произошла ошибка");
        this->popup->set_description("Неопознанная ошибка");
        this->popup->exec();
        this->socket->close();
        socket->connectToHost(this->host, this->port);
    }
}

void MainWindow::slotError(QAbstractSocket::SocketError)
{
    qDebug() << "socket error!";
}

void MainWindow::slotConnected()
{
    qDebug() << "host connected";
    switch (this->forward)
    {
    case (type_forward::login):
    case (type_forward::registration):
        this->ui->login_2->setEnabled(true);
        this->ui->register_2->setEnabled(true);
        break;
    }
}

void MainWindow::send(const QString &data)
{
    int32_t length = int32_t(data.toUtf8().length());
    length++;
    char buf[4];
    std::memcpy(buf, &length, 4);
    this->socket->write(buf, 4);

    this->socket->write(data.toUtf8());
}

QString MainWindow::recv()
{
    char buf[sizeof(int32_t)]{0};
    this->socket->read(buf, sizeof(int32_t));

    int32_t length;
    std::memcpy(&length, buf, sizeof(int32_t));

    return this->socket->readAll();
}

void MainWindow::create_db()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(QDir::currentPath() + "/main.db");

    if (!this->db.open())
    {
           qDebug() << this->db.lastError().text();
    }
    QSqlQuery query;
    query.exec("CREATE TABLE Pattern (                      "
                    "id INTEGER NOT NULL UNIQUE PRIMARY KEY,"
                    "name VARCHAR(45) NOT NULL UNIQUE,      "
                    "description TEXT NOT NULL,             "
                    "code TEXT NOT NULL,                    "
                    "path_to_image VARCHAR(45) NOT NULL);   ");
    query.exec("CREATE TABLE Pattern_test (           "
                    "id_pattern INTEGER NOT NULL,           "
                    "question TEXT NOT NULL,                "
                    "id_type INTEGER NOT NULL,              "
                    "a1 VARCHAR(45) NOT NULL DEFAULT 'NULL',"
                    "a2 VARCHAR(45) NOT NULL DEFAULT 'NULL',"
                    "a3 VARCHAR(45) NOT NULL DEFAULT 'NULL',"
                    "a4 VARCHAR(45) NOT NULL DEFAULT 'NULL',"
                    "correct_answer VARCHAR(45) NOT NULL);  " );
    query.exec("CREATE TABLE User (                              "
                    "login VARCHAR(45) NOT NULL UNIQUE,          "
                    "password VARCHAR(45) NOT NULL UNIQUE,       "
                    "path_to_image VARCHAR(45) NOT NULL UNIQUE); " );
    query.exec("CREATE TABLE User_test (               "
                    "pattern VARCHAR(45) NOT NULL UNIQUE,      "
                    "count_corrent INTEGER NOT NULL);           " );
}

void MainWindow::fill_db_login(tinyxml2::XMLElement *body)
{
    QString login {this->ui->login_lineEdit->text()};
    QString password {this->ui->password_lineEdit->text()};
    QString type_image {body->FirstChildElement("img_type")->GetText()};
    QString path_to_image {QDir::currentPath() + "/images/user." + type_image};
    this->base64_file = body->FirstChildElement("img")->GetText();
    this->save_img_to_file(path_to_image, this->base64_file);

    QSqlQuery insert;
    insert.exec("INSERT INTO User (login, password, path_to_image) VALUES ('" + login + "', '" + password + "', '" + path_to_image + "')");
    int length {body->FirstChildElement("count_test")->IntText()};
    if (length > 0)
    {
        tinyxml2::XMLElement * tests = body->FirstChildElement("tests");
        tinyxml2::XMLElement * test = tests->FirstChildElement("test");
        while (test)
        {
            QString name {test->FirstChildElement("name")->GetText()};
            QString result {test->FirstChildElement("result")->GetText()};

            insert.exec("INSERT INTO User_test (pattern, count_corrent) VALUES ('" + name + "', " + result + ")");

            test = test->NextSiblingElement("test");
        }
    }
}

void MainWindow::save_img_to_file(const QString &path, const QString &img)
{
    std::ofstream tmp_file(path.toUtf8());
    tmp_file.close();
    qDebug() << path;

    std::ofstream file(path.toUtf8(), std::ios::out | std::ios::binary);
    auto data{ base64_decode(img.toStdString()) };
    for (size_t i = 0; i < data.size(); i++)
    {
        file << data[i];
    }
    file.close();
}

void MainWindow::on_to_register_clicked()
{
    this->ui->screen_stacked->setCurrentIndex((int)screen::registration);
    this->forward = type_forward::registration;
}

void MainWindow::on_to_login_clicked()
{
    this->ui->screen_stacked->setCurrentIndex((int)screen::login);
    this->forward = type_forward::login;
}

void MainWindow::on_register_2_clicked()
{
    if (!this->ui->reg_login->text().size() || !this->ui->reg_pas->text().size() || !this->base64_file.size())
        return;

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.1\" encoding=\"UTF-8\"");
    doc.InsertEndChild(decl);

    tinyxml2::XMLElement* type = doc.NewElement("type");
    type->SetText("registration");
    doc.InsertEndChild(type);

    tinyxml2::XMLElement* body = doc.NewElement("body");
    doc.InsertEndChild(body);

    tinyxml2::XMLElement* login = doc.NewElement("login");
    login->SetText(this->ui->reg_login->text().toLocal8Bit().data());
    body->InsertEndChild(login);

    tinyxml2::XMLElement* password = doc.NewElement("password");
    password->SetText(this->ui->reg_pas->text().toLocal8Bit().data());
    body->InsertEndChild(password);

    tinyxml2::XMLElement* img = doc.NewElement("img");
    img->SetText(this->base64_file.toLocal8Bit().data());
    body->InsertEndChild(img);

    tinyxml2::XMLElement* img_type = doc.NewElement("img_type");
    img_type->SetText(this->file_type.toLocal8Bit().data());
    body->InsertEndChild(img_type);

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);

    //qDebug() << printer.CStr();

    this->send(printer.CStr());

    this->base64_file = "";
    this->file_type = "";
}

void MainWindow::on_reg_img_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    std::ifstream file(fileName.toUtf8(), std::ios::in | std::ios::binary);
    if (!file.is_open())
        return;

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    this->base64_file = base64_encode(data).c_str();
    this->file_type = fileName.split(".").back();
}
