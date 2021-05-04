#ifndef WRITE_ANSWER_H
#define WRITE_ANSWER_H

#include "question_base.h"

class Write_answer : public Question_base
{
public:
    Write_answer() = default;

    void input(QTextStream& file_stream) override;

    QString get_current_answer() const {return current_answer;}
    void set_current_answer(QString i) {this->current_answer = i;}

    const QString& get_correct_answer(){return correct_answer;}

private:
    QString correct_answer;
    QString current_answer;
};

#endif // WRITE_ANSWER_H
