#ifndef PLUTODEVICE_H
#define PLUTODEVICE_H

#ifdef __cplusplus
   extern "C" {
#endif
#include "libpluto_structs.h"

// Init Devices
int pluto_init(char * device_uri);
int pluto_init_sensors();
int pluto_init_receiver(size_t samples_cnt);
int pluto_init_transmitter(size_t samples_cnt);
// Get TX Values
double pluto_get_tx_gain();
long long pluto_get_tx_samprate();
long long pluto_get_tx_bandwidth();
bool pluto_get_tx_fir_is_enable();
long long pluto_get_tx_freq();
bool pluto_get_tx_is_enable();
// Get RX Values
bool pluto_get_rx_is_enable();
long long pluto_get_rx_freq();
long long pluto_get_rx_samprate();
long long pluto_get_rx_bandwidth();
long long pluto_get_rx_gain();
bool pluto_get_rx_fir_is_enable();
// Get Sensors Values
double pluto_get_rf_temp();
double pluto_get_soc_temp();
double pluto_get_power();
double pluto_get_voltage();
double pluto_get_current();
// Set TX Values
int pluto_set_tx_samprate(long long tx_samprate);
int pluto_set_tx_bandwidth(long long tx_bandwidth);
int pluto_set_tx_gain(double tx_gain);
int pluto_set_tx_fir_en(bool is_enable);
int pluto_set_tx_freq(long long tx_freq);
int pluto_set_tx_en(bool is_enable);
// Set RX Values
int pluto_set_rx_en(bool is_enable);
int pluto_set_rx_fir_en(bool is_enable);
int pluto_set_rx_gain(long long rx_gain);
int pluto_set_rx_bandwidth(long long rx_bandwidth);
int pluto_set_rx_samprate(long long samprate);
int pluto_set_rx_freq(long long rx_freq);
// Other Functions
int pluto_schedule_tx_buff();
int pluto_get_rx_data();
int pluto_set_tx_data();
void pluto_destroy();
// Utilities
int pluto_util_init_all(char * device_uri, size_t rx_samples_cnt, size_t tx_samples_cnt);
double pluto_util_get_rx_power(long long rx_freq, long long rx_bandwidth, long long rx_samprate);
void pluto_util_set_tx_power(long long tx_freq, long long tx_bandwidth, long long tx_samprate, double tx_gain);


// Internal functions
int _pluto_attr_get(void *value, struct iio_device *device, char *channel, bool is_output,
                    char *attr, char *value_name, char *type_name);
int _pluto_attr_set(void *value, bool is_limited, void *value_min, void *value_max, void *value_inc,
                    struct iio_device *device, char *channel, bool is_output, char *attr,
                    char *value_name, char *type_name);
struct iio_buffer *_pluto_create_buffer(struct iio_device *device, size_t samples_cnt);
void _pluto_destroy_context();
void _pluto_destroy_buffer_rx();



// Examples
// void __pluto_receiver_example();
// void __pluto_transmitter_example();

#ifdef __cplusplus
   }
#endif

#endif
