#ifndef QUESTION_BASE_H
#define QUESTION_BASE_H

#include <QString>
#include <QFile>
#include <QTextStream>

enum class test_type
{
    one_of_four = 1,
    some_of_four = 2,
    write_answer = 3,
    installation_of_correspondence = 4
};

class Question_base
{
public:
    Question_base() = default;
    virtual ~Question_base() = default;

    virtual void input(QTextStream& file_stream) = 0;

    /*get set*/
    const test_type& get_type(){return type;}
    void set_type(test_type type){this->type = type;}
    const QString& get_question(){return question;}

protected:
    QString question;
    test_type type;
};

#endif // QUESTION_BASE_H
