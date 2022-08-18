#ifndef FLAGS_H
#define FLAGS_H

#include <QString>

QString iio_uri_set;
unsigned int freq_unit_flag = 0;    // unit = pow(1000, freq_unit_flag);
unsigned int freq_unit_set = 0;
double freq_set;
double samprate_set;
double bandwidth_set;
size_t buffersize_set;
bool agc_set = true;

QString ip_get = "127.0.0.1";
bool start_status = false;
long long freq_get = 0;
long long samprate_get = 0;
long long bandwidth_get = 0;
bool agc_get = true;

QString status_message = "Uninitialized.";

bool device_busy = true;
#endif // FLAGS_H
