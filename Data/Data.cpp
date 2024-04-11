#include "Data.h"


Data::Data()
{
    EdfaStatus status = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    edfaStauts = &status;
    edfaStauts->mode = ACC_MODE;
}
