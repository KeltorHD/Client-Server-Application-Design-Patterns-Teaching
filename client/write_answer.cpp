#include "write_answer.h"

void Write_answer::input(QTextStream& file_stream)
{
    this->question = file_stream.readLine();
    this->correct_answer = file_stream.readLine();
}
