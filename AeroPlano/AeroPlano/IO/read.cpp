#include "read.h"

Read::Read()
{
}

Mat Read::readImage(string path)
{
    return imread(path);
}
