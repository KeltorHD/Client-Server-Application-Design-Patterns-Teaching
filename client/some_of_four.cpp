#include "some_of_four.h"

void Some_of_four::input(QTextStream& file_stream)
{
    this->question = file_stream.readLine();
    for (size_t i = 0; i < 4; i++)
    {
        this->answers[i] = file_stream.readLine();
    }
    size_t count{file_stream.readLine().toULongLong()};
    for(size_t i = 0; i < count; i++)
    {
        this->correct_answer.push_back(file_stream.readLine().toULongLong());
    }
}
