#ifndef _LIBPLUTO_STRUCTS_H
#define _LIBPLUTO_STRUCTS_H

#include <iio.h>
#include <string.h>
#include <math.h>

bool pluto_is_right = false;

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

struct pluto_receiver{
    void *rx_ptr_start, *rx_ptr_end;
    ptrdiff_t rx_ptr_inc;
} pluto_receiver;

struct pluto_transmitter{
    void *tx_ptr_start, *tx_ptr_end;
    ptrdiff_t tx_ptr_inc;
} pluto_transmitter; 

#endif
