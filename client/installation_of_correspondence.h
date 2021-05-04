#ifndef INSTALLATION_OF_CORRESPONDENCE_H
#define INSTALLATION_OF_CORRESPONDENCE_H

#include "question_base.h"

class Installation_of_correspondence : public Question_base
{
public:
    Installation_of_correspondence() = default;

    void input(QTextStream& file_stream) override;

    std::array<QString, 4> get_current_answer() const {return current_answer;}
    void set_current_answer(std::array<QString, 4> i) {this->current_answer = i;}

    const std::array<QString, 4>& get_correct_answer(){return correct_answer;}
    /*answer, answer*/
    std::pair<std::array<QString, 4>, std::array<QString, 4>> get_answers();

private:
    std::array<QString, 4> answers;
    std::array<QString, 4> correct_answer;
    std::array<QString, 4> current_answer;

};

#endif // INSTALLATION_OF_CORRESPONDENCE_H
