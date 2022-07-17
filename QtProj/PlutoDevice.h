#ifndef PLUTODEVICE_H
#define PLUTODEVICE_H

#include <iio.h>
#include <string>
#include <math.h>

class pluto_sdr{
private:
    struct pluto_dev{
        struct iio_context *pluto_ctx;  // Pluto context;
        struct iio_device *rf_phy;
        struct iio_device *soc_phy;
        struct iio_device *pwr_phy;
        struct iio_device *rx_dev;
        struct iio_device *tx_dev;  // Devices;
        struct iio_channel *pwr_current;
        struct iio_channel *pwr_voltage;
        struct iio_channel *rf_temperature;
        struct iio_channel *soc_temperature;   // Sensors;
        struct iio_buffer *rx_buf;
        struct iio_channel *rx_i, *rx_q;    // Receiver;
        struct iio_buffer *tx_buf;
        struct iio_channel *tx_i, *tx_q;    // Receiver;
    } pluto_dev;

    int _pluto_attr_get(void *value, struct iio_device *device, char *channel, bool is_output,
                        char *attr, std::string value_name, std::string type_name);
    int _pluto_attr_set(void *value, bool is_limited, void *value_min, void *value_max, void *value_inc,
                        struct iio_device *device, char *channel, bool is_output, char *attr,
                        std::string value_name, std::string type_name);
    struct iio_buffer *_pluto_create_buffer(iio_device *device, size_t samples_cnt);
    void _pluto_destroy_context();
    void _pluto_destroy_buffer_rx();

    // Examples
    void __pluto_receiver_example();
    void __pluto_transmitter_example();

public:
    std::string pluto_message;
    struct pluto_receiver{
        void *rx_ptr_start, *rx_ptr_end;
        ptrdiff_t rx_ptr_inc;
    } pluto_receiver;
    struct pluto_transmitter{
        void *tx_ptr_start, *tx_ptr_end;
        ptrdiff_t tx_ptr_inc;
    } pluto_transmitter;

    int pluto_get_tx_gain(double *tx_gain);
    int pluto_get_tx_samprate(long long *samprate);
    int pluto_get_tx_bandwidth(long long *tx_bandwidth);
    int pluto_get_tx_fir_is_enable(bool *is_enable);
    int pluto_get_rx_is_enable(bool *is_enable);
    int pluto_get_rx_freq(long long *rx_freq);
    int pluto_get_rx_samprate(long long *samprate);
    int pluto_get_rx_bandwidth(long long *rx_bandwidth);
    int pluto_get_rx_gain(long long *rx_gain);
    int pluto_get_rx_fir_is_enable(bool *is_enable);
    int pluto_get_tx_freq(long long *tx_freq);
    int pluto_get_tx_is_enable(bool *is_enable);
    long long pluto_MHz(double MHz);
    long long pluto_GHz(double GHz);
    long long pluto_kHz(double kHz);
    int pluto_set_tx_samprate(long long tx_samprate);
    int pluto_set_tx_bandwidth(long long tx_bandwidth);
    int pluto_set_tx_gain(double tx_gain);
    int pluto_set_tx_fir_en(bool is_enable);
    int pluto_set_tx_freq(long long tx_freq);
    int pluto_set_tx_en(bool is_enable);
    int pluto_set_rx_en(bool is_enable);
    int pluto_schedule_tx_buff();
    int pluto_init_transmitter(size_t samples_cnt);
    int pluto_set_rx_fir_en(bool is_enable);
    int pluto_set_rx_gain(long long rx_gain);
    int pluto_set_rx_bandwidth(long long rx_bandwidth);
    void pluto_message_generate(std::string msg, int code);
    int pluto_get_rf_temp(double *rf_temp);
    int pluto_get_soc_temp(double *soc_temp);
    int pluto_get_power(double *sys_power);
    int pluto_get_voltage(double *sys_voltage);
    int pluto_get_current(double *sys_current);
    int pluto_get_rx_data();
    int pluto_set_rx_samprate(long long samprate);
    int pluto_set_rx_freq(long long rx_freq);
    int pluto_init_sensors();
    int pluto_init_receiver(size_t samples_cnt);
    int pluto_init(char * device_uri);
    void pluto_destroy();
};

#endif
