#include "functions.h"

QString functions_freq_long_to_str(long long num)
{
    return (num > 1000000000? QString::number((long long)(num / 1000000000)).append(".").append(QString::number((long long)(num % 1000000000))).append(" GHz"):
           (num > 1000000? QString::number((long long)(num / 1000000)).append(".").append(QString::number((long long)(num % 1000000))).append(" MHz") :
           (num > 1000? QString::number((long long)(num / 1000)).append(".").append(QString::number((long long)(num % 1000))).append(" kHz") :
                        QString::number((long long)(num)).append(" Hz"))));
}

double functions_freq_long_to_double(long long num)
{
    return (num > 1000000000? num / 1000000000.0 :
           (num > 1000000? num / 1000000.0 :
           (num > 1000? num / 1000.0 : (double)num)));
}

QString functions_freq_long_to_unit(long long num)
{
    return (num > 1000000000? QString(" GHz"):
           (num > 1000000? QString(" MHz") :
           (num > 1000? QString(" kHz") : QString(" Hz"))));
}
