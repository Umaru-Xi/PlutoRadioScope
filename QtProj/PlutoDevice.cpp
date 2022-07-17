#include "PlutoDevice.h"

int pluto_sdr::pluto_get_tx_gain(double *tx_gain)
{
    return _pluto_attr_get(
                (void *)tx_gain,
                pluto_dev.rf_phy, (char *)"voltage0", true,
                (char *)"hardwaregain", "TX Gain",
                "double");
}

int pluto_sdr::pluto_get_tx_samprate(long long *samprate)
{
    return _pluto_attr_get(
                (void *)samprate,
                pluto_dev.rf_phy, (char *)"voltage0", true,
                (char *)"sampling_frequency", "TX Sampling Rate",
                "longlong");
}

int pluto_sdr::pluto_get_tx_bandwidth(long long *tx_bandwidth)
{
    return _pluto_attr_get(
                (void *)tx_bandwidth,
                pluto_dev.rf_phy, (char *)"voltage0", true,
                (char *)"rf_bandwidth", "TX Bnadwidth",
                "longlong");
}

int pluto_sdr::pluto_get_tx_fir_is_enable(bool *is_enable)
{
    return _pluto_attr_get(
                    (void *)is_enable,
                    pluto_dev.rf_phy, (char *)"voltage0", true,
                    (char *)"filter_fir_en", "TX FIR Filter Status",
                    "bool");
}

int pluto_sdr::pluto_get_rx_is_enable(bool *is_enable)
{
    int return_code;
    bool value = false;
    return_code = _pluto_attr_get(
                (void *)&value,
                pluto_dev.rf_phy, (char *)"altvoltage0", true,
                (char *)"powerdown", "RX Enable Status",
                "bool");
    *is_enable = !value;
    return return_code;
}

int pluto_sdr::pluto_get_tx_is_enable(bool *is_enable)
{
    int return_code;
    bool value = false;
    return_code = _pluto_attr_get(
                (void *)&value,
                pluto_dev.rf_phy, (char *)"altvoltage1", true,
                (char *)"powerdown", "TX Enable Status",
                "bool");
    *is_enable = !value;
    return return_code;
}

int pluto_sdr::pluto_get_rx_freq(long long *rx_freq)
{
    return _pluto_attr_get(
                (void *)rx_freq,
                pluto_dev.rf_phy, (char *)"altvoltage0", true,
                (char *)"frequency", "RX Frequency",
                "longlong");
}

int pluto_sdr::pluto_get_rx_samprate(long long *samprate)
{
    return _pluto_attr_get(
                (void *)samprate,
                pluto_dev.rf_phy, (char *)"voltage0", false,
                (char *)"sampling_frequency", "RX Sampling Rate",
                "longlong");
}

int pluto_sdr::pluto_get_rx_bandwidth(long long *rx_bandwidth)
{
    return _pluto_attr_get(
                (void *)rx_bandwidth,
                pluto_dev.rf_phy, (char *)"voltage0", false,
                (char *)"rf_bandwidth", "RX Bandwidth",
                "longlong");
}

int pluto_sdr::pluto_get_rx_gain(long long *rx_gain)
{
    return _pluto_attr_get(
                (void *)rx_gain,
                pluto_dev.rf_phy, (char *)"voltage0", false,
                (char *)"hardwaregain", "RX Gain",
                "longlong");
}

int pluto_sdr::pluto_get_rx_fir_is_enable(bool *is_enable)
{
    return _pluto_attr_get(
                (void *)is_enable,
                pluto_dev.rf_phy, (char *)"voltage0", false,
                (char *)"filter_fir_en", "RX FIR Filter Status",
                "bool");
}

int pluto_sdr::pluto_get_tx_freq(long long *tx_freq)
{
    return _pluto_attr_get(
                (void *)tx_freq,
                pluto_dev.rf_phy, (char *)"altvoltage1", true,
                (char *)"frequency", "TX Frequency",
                "longlong");
}

long long pluto_sdr::pluto_kHz(double kHz)
{
    pluto_message_generate("kHz(double) to Hz(long long) convertion.", 0);
    return kHz * 1000;
}

long long pluto_sdr::pluto_MHz(double MHz)
{
    pluto_message_generate("MHz(double) to Hz(long long) convertion.", 0);
    return MHz * 1000000;
}

long long pluto_sdr::pluto_GHz(double GHz)
{
    pluto_message_generate("GHz(double) to Hz(long long) convertion.", 0);
    return GHz * 1000000000;
}

int pluto_sdr::pluto_set_tx_samprate(long long tx_samprate)
{
    long long value_min, value_max, value_inc;
    value_min = 2083334; value_max = 61439999; value_inc = 1;
    return _pluto_attr_set(
            (void *)&tx_samprate, true, (void *)&value_min, (void *)&value_max, (void *)&value_inc,
            pluto_dev.rf_phy, (char *)"voltage0", true, (char *)"sampling_frequency",
            "TX Sampling Rate", "longlong");
}

int pluto_sdr::pluto_set_tx_bandwidth(long long tx_bandwidth)
{
    long long value_min, value_max, value_inc;
    value_min = 200001; value_max = 39999999; value_inc = 1;
    return _pluto_attr_set(
            (void *)&tx_bandwidth, true, (void *)&value_min, (void *)&value_max, (void *)&value_inc,
            pluto_dev.rf_phy, (char *)"voltage0", true, (char *)"rf_bandwidth",
            "TX Bandwidth", "longlong");
}

int pluto_sdr::pluto_set_tx_gain(double tx_gain)
{
    double value_min, value_max, value_inc;
    value_min = -90.00; value_max = -0.25; value_inc = 0.25;
    return _pluto_attr_set(
            (void *)&tx_gain, true, (void *)&value_min, (void *)&value_max, (void *)&value_inc,
            pluto_dev.rf_phy, (char *)"voltage0", true, (char *)"hardwaregain",
            "TX Gain", "double");
}

int pluto_sdr::pluto_set_tx_fir_en(bool is_enable)
{
    int return_code = iio_channel_attr_write_bool(
                iio_device_find_channel(pluto_dev.rf_phy, "voltage0", true),
                "filter_fir_en", is_enable);
    if(return_code == 0) pluto_message_generate("TX FIR Filter has been set.", 0);
    else pluto_message_generate("Can not set TX FIR Filter.", 2);
    return return_code;
}

int pluto_sdr::pluto_set_tx_freq(long long tx_freq)
{
    long long value_min, value_max, value_inc;
    value_min = 46875002; value_max = 5999999999; value_inc = 1;
    return _pluto_attr_set(
            (void *)&tx_freq, true, (void *)&value_min, (void *)&value_max, (void *)&value_inc,
            pluto_dev.rf_phy, (char *)"altvoltage1", true, (char *)"frequency",
            "TX Frequency", "longlong");
}

int pluto_sdr::pluto_set_rx_en(bool is_enable)
{
    bool value;
    value = !is_enable;
    return _pluto_attr_set(
            (void *)&value, false, (void *)NULL, (void *)NULL, (void *)NULL,
            pluto_dev.rf_phy, (char *)"altvoltage0", true, (char *)"powerdown",
            "RX Enable Status", "bool");
}

int pluto_sdr::pluto_set_tx_en(bool is_enable)
{
    bool value;
    value = !is_enable;
    return _pluto_attr_set(
            (void *)&value, false, (void *)NULL, (void *)NULL, (void *)NULL,
            pluto_dev.rf_phy, (char *)"altvoltage1", true, (char *)"powerdown",
            "TX Enable Status", "bool");
}

int pluto_sdr::pluto_init_transmitter(size_t samples_cnt)
{
    pluto_dev.tx_dev = iio_context_find_device(pluto_dev.pluto_ctx, "cf-ad9361-dds-core-lpc");
        if(pluto_dev.tx_dev == NULL)
        {
            pluto_message_generate("Can not find TX Core(cf-ad9361-dds-core-lpc).", 2);
            return 1;
        }
    pluto_dev.tx_i = iio_device_find_channel(pluto_dev.tx_dev, "voltage0", true);
        if(pluto_dev.tx_i == NULL)
        {
            pluto_message_generate("Can not find TX Imaginary Channel.", 2);
            return 2;
        }
    pluto_dev.tx_q = iio_device_find_channel(pluto_dev.tx_dev, "voltage1", true);
        if(pluto_dev.tx_q == NULL)
        {
            pluto_message_generate("Can not find TX Real Channel.", 2);
            return 2;
        }
    iio_channel_enable(pluto_dev.tx_i);
    iio_channel_enable(pluto_dev.tx_q);
    pluto_dev.tx_buf = _pluto_create_buffer(pluto_dev.tx_dev, samples_cnt);
        if(pluto_dev.tx_buf == NULL)
        {
            pluto_message_generate("Create TX Buffer failed.", 2);
            return 3;
        }
    pluto_message_generate("Transmitter has been initialized.", 0);
    return 0;
}

int pluto_sdr::pluto_set_rx_fir_en(bool is_enable)
{
    return _pluto_attr_set(
            (void *)&is_enable, false, (void *)NULL, (void *)NULL, (void *)NULL,
            pluto_dev.rf_phy, (char *)"voltage0", false, (char *)"filter_fir_en",
            "RX FIR Filter", "bool");
}

int pluto_sdr::pluto_set_rx_gain(long long rx_gain)
{
    long long value_min, value_max, value_inc;
    value_min =0; value_max = 72; value_inc = 1;
    return _pluto_attr_set(
            (void *)&rx_gain, true, (void *)&value_min, (void *)&value_max, (void *)&value_inc,
            pluto_dev.rf_phy, (char *)"voltage0", false, (char *)"hardwaregain",
            "RX Gain", "longlong");
}

int pluto_sdr::pluto_set_rx_bandwidth(long long rx_bandwidth)
{
    long long value_min, value_max, value_inc;
    value_min = 200001; value_max = 55999999; value_inc = 1;
    return _pluto_attr_set(
            (void *)&rx_bandwidth, true, (void *)&value_min, (void *)&value_max, (void *)&value_inc,
            pluto_dev.rf_phy, (char *)"voltage0", false, (char *)"rf_bandwidth",
            "RX Bandwidth", "longlong");
}

void pluto_sdr::pluto_message_generate(std::string msg, int code)
{
    if(code < 0 || code > 2)
    {
        pluto_message = "#ERRO: Pluto message generation failed, unknown message code.";
    }
    else
    {
        switch(code)
        {
        case 0:
            pluto_message = "#INFO: ";
            pluto_message.append(msg);
            break;
        case 1:
            pluto_message = "#WARN: ";
            pluto_message.append(msg);
            break;
        case 2:
            pluto_message = "#ERRO: ";
            pluto_message.append(msg);
            break;
        }
    }
    return;
}

int pluto_sdr::pluto_set_rx_samprate(long long samprate)
{
    long long value_min, value_max, value_inc;
    value_min = 2083334; value_max = 61439999; value_inc = 1;
    return _pluto_attr_set(
            (void *)&samprate, true, (void *)&value_min, (void *)&value_max, (void *)&value_inc,
            pluto_dev.rf_phy, (char *)"voltage0", false, (char *)"sampling_frequency",
            "RX Sampling Rate", "longlong");
}

int pluto_sdr::pluto_set_rx_freq(long long rx_freq)
{
    long long value_min, value_max, value_inc;
    value_min = 70000001; value_max = 5999999999; value_inc = 1;
    return _pluto_attr_set(
            (void *)&rx_freq, true, (void *)&value_min, (void *)&value_max, (void *)&value_inc,
            pluto_dev.rf_phy, (char *)"altvoltage0", true, (char *)"frequency",
            "RX Frequency", "longlong");
}

int pluto_sdr::pluto_init(char * device_uri)
{
    pluto_dev.pluto_ctx = iio_create_context_from_uri(device_uri);
        if(pluto_dev.pluto_ctx == NULL)
        {
            pluto_message_generate("Can not connect ADALM-Pluto by URI.", 2);
            return 1;
        }
    pluto_dev.rf_phy = iio_context_find_device(pluto_dev.pluto_ctx, "ad9361-phy");
        if(pluto_dev.rf_phy == NULL)
        {
            pluto_message_generate("Can not find RF Chipset(AD936X).", 2);
            return 2;
        }
    pluto_dev.soc_phy = iio_context_find_device(pluto_dev.pluto_ctx, "xadc");
        if(pluto_dev.soc_phy == NULL)
        {
            pluto_message_generate("Can not find Xilinx APSoC(Zynq-70X0).", 2);
            return 2;
        }
    pluto_dev.pwr_phy = iio_context_find_device(pluto_dev.pluto_ctx, "adm1177-iio");
        if(pluto_dev.pwr_phy == NULL)
        {
            pluto_message_generate("Can not find Power Manager Chipset(ADM1177).", 2);
            return 2;
        }
    pluto_message_generate("ADALM-Pluto has been initialized.", 0);
    return 0;
}

int pluto_sdr::pluto_init_receiver(size_t samples_cnt)
{
    pluto_dev.rx_dev = iio_context_find_device(pluto_dev.pluto_ctx, "cf-ad9361-lpc");
        if(pluto_dev.rx_dev == NULL)
        {
            pluto_message_generate("Can not find RX Core(cf-ad936x-lpc).", 2);
            return 1;
        }
    pluto_dev.rx_i = iio_device_find_channel(pluto_dev.rx_dev, "voltage0", false);
        if(pluto_dev.rx_i == NULL)
        {
            pluto_message_generate("Can not find RX Imaginary Channel.", 2);
            return 2;
        }
    pluto_dev.rx_q = iio_device_find_channel(pluto_dev.rx_dev, "voltage1", false);
        if(pluto_dev.rx_q == NULL)
        {
            pluto_message_generate("Can not find RX Real Channel.", 2);
            return 2;
        }
    iio_channel_enable(pluto_dev.rx_i);
    iio_channel_enable(pluto_dev.rx_q);
    pluto_dev.rx_buf = _pluto_create_buffer(pluto_dev.rx_dev, samples_cnt);
        if(pluto_dev.rx_buf == NULL)
        {
            pluto_message_generate("Create RX Buffer failed.", 2);
            return 3;
        }
    pluto_message_generate("Receiver has been initialized.", 0);
    return 0;
}

int pluto_sdr::pluto_schedule_tx_buff()
{
    if(iio_buffer_push(pluto_dev.tx_buf) < 0)
    {
        pluto_message_generate("Schedule TX Buffer failed.", 2);
        return 1;
    }
    pluto_transmitter.tx_ptr_inc = iio_buffer_step(pluto_dev.tx_buf);
    pluto_transmitter.tx_ptr_end = iio_buffer_end(pluto_dev.tx_buf);
    pluto_transmitter.tx_ptr_start = iio_buffer_first(pluto_dev.tx_buf, pluto_dev.tx_i);
    pluto_message_generate("TX Buffer has been scheduled.", 0);
    return 0;
}

int pluto_sdr::pluto_get_rx_data()
{
    if(iio_buffer_refill(pluto_dev.rx_buf) < 0)
    {
        pluto_message_generate("Refill RX Buffer failed.", 2);
        return 1;
    }
    pluto_receiver.rx_ptr_inc = iio_buffer_step(pluto_dev.rx_buf);
    pluto_receiver.rx_ptr_end = iio_buffer_end(pluto_dev.rx_buf);
    pluto_receiver.rx_ptr_start = iio_buffer_first(pluto_dev.rx_buf, pluto_dev.rx_i);
    pluto_message_generate("RX Data has been gotten.", 0);
    return 0;
}

int pluto_sdr::pluto_get_rf_temp(double *rf_temp)
{
    long long rf_temp_raw;
    if(iio_channel_attr_read_longlong(pluto_dev.rf_temperature, "input", &rf_temp_raw) < 0)
    {
        pluto_message_generate("Can not get RF Chipset's Temperature.", 2);
        return 1;
    }
    else
    {
        *rf_temp =rf_temp_raw / 1000.0;
        pluto_message_generate("RF Chipset's Temperature has been gotten.", 0);
    }
    return 0;
}

int pluto_sdr::pluto_get_soc_temp(double *soc_temp)
{
    double soc_temp_scale;
    long long soc_temp_raw, soc_temp_offset;
    if(iio_channel_attr_read_double(pluto_dev.soc_temperature, "scale", &soc_temp_scale) < 0)
    {
        pluto_message_generate("Can not get SOC Temperature Scale value.", 2);
        return 1;
    }
    if(iio_channel_attr_read_longlong(pluto_dev.soc_temperature, "raw", &soc_temp_raw) < 0)
    {
        pluto_message_generate("Can not get SOC Temperature RAW value.", 2);
        return 1;
    }
    if(iio_channel_attr_read_longlong(pluto_dev.soc_temperature, "offset", &soc_temp_offset) < 0)
    {
        pluto_message_generate("Can not get SOC Temperature Offset value.", 2);
        return 1;
    }
    pluto_message_generate("APSoC's Temperature has been gotten.", 0);
    *soc_temp = (soc_temp_raw + soc_temp_offset) * soc_temp_scale / 1000.0;
    return 0;
}

int pluto_sdr::pluto_get_power(double *sys_power)
{
    double current, voltage;
    if(pluto_get_current(&current) != 0)
    {
        pluto_message_generate("Can not get System Current.", 0);
        return 1;
    }
    if(pluto_get_voltage(&voltage) != 0)
    {
        pluto_message_generate("Can not get System Voltage.", 0);
        return 1;
    }
    pluto_message_generate("System Power has been calculated.", 0);
    *sys_power = current * voltage;
    return 0;
}

int pluto_sdr::pluto_get_voltage(double *sys_voltage)
{
    double voltage_scale;
    long long voltage_raw;
    if(iio_channel_attr_read_double(pluto_dev.pwr_voltage, "scale", &voltage_scale) < 0)
    {
        pluto_message_generate("Can not get System Voltage Scale value.", 2);
        return 1;
    }
    if(iio_channel_attr_read_longlong(pluto_dev.pwr_voltage, "raw", &voltage_raw) < 0)
    {
        pluto_message_generate("Can not get System Voltage RAW value.", 2);
        return 1;
    }
    pluto_message_generate("System Voltage has been gotten.", 0);
    *sys_voltage = voltage_raw * voltage_scale / 1000.0;
    return 0;
}

int pluto_sdr::pluto_get_current(double *sys_current)
{
    double current_scale;
    long long current_raw;
    if(iio_channel_attr_read_double(pluto_dev.pwr_current, "scale", &current_scale) <0)
    {
        pluto_message_generate("Can not get System Current Scale value.", 2);
        return 1;
    }
    if(iio_channel_attr_read_longlong(pluto_dev.pwr_current, "raw", &current_raw) < 0)
    {
        pluto_message_generate("Can not get System Current RAW value.", 2);
        return 1;
    }
    pluto_message_generate("System Current has been gotten.", 0);
    *sys_current = current_raw * current_scale / 1000.0;
    return 0;
}

int pluto_sdr::pluto_init_sensors()
{
    pluto_dev.pwr_current =iio_device_find_channel(pluto_dev.pwr_phy, "current0", false);
        if(pluto_dev.pwr_current == NULL)
        {
            pluto_message_generate("Can not find System Current Channel.", 2);
            return 1;
        }
    pluto_dev.pwr_voltage =iio_device_find_channel(pluto_dev.pwr_phy, "voltage0", false);
        if(pluto_dev.pwr_voltage == NULL)
        {
            pluto_message_generate("Can not find System Voltage Channel.", 2);
            return 1;
        }
    pluto_dev.rf_temperature = iio_device_find_channel(pluto_dev.rf_phy, "temp0", false);
        if(pluto_dev.rf_temperature == NULL)
        {
            pluto_message_generate("Can not find RF Chipset's Temperature Channel.", 2);
            return 1;
        }
    pluto_dev.soc_temperature = iio_device_find_channel(pluto_dev.soc_phy, "temp0", false);
        if(pluto_dev.soc_temperature == NULL)
        {
            pluto_message_generate("Can not find APSoC's Temperature Channel.", 2);
            return 1;
        }
    iio_channel_enable(pluto_dev.pwr_current);
    iio_channel_enable(pluto_dev.pwr_voltage);
    iio_channel_enable(pluto_dev.rf_temperature);
    iio_channel_enable(pluto_dev.soc_temperature);
    pluto_message_generate("Sensors has been initialized.", 0);
    return 0;
}

struct iio_buffer *pluto_sdr::_pluto_create_buffer(iio_device *device, size_t samples_cnt)
{
    struct iio_buffer *buf;
    buf = iio_device_create_buffer(device, samples_cnt, false);
    return buf;
}

void pluto_sdr::_pluto_destroy_context()
{
    iio_context_destroy(pluto_dev.pluto_ctx);
    return;
}

void pluto_sdr::_pluto_destroy_buffer_rx()
{
    iio_buffer_destroy(pluto_dev.rx_buf);
    return;
}

void pluto_sdr::pluto_destroy()
{
    _pluto_destroy_buffer_rx();
    _pluto_destroy_context();
    pluto_message_generate("ADALM-Pluto can be disconnected now.", 0);
    return;
}

int pluto_sdr::_pluto_attr_set(
        void *value, bool is_limited, void *value_min, void *value_max, void *value_inc,
        struct iio_device *device, char *channel, bool is_output, char *attr,
        std::string value_name, std::string type_name)
{
    int type_code, return_code;
    bool out_of_limit;
    if(type_name == "bool")
    {
        type_code = 0;
        out_of_limit = false;
    }
    else if(type_name == "long long" || type_name == "longlong")
    {
        type_code = 1;
        out_of_limit = (is_limited && (*(long long *)value < *(long long *)value_min
                        || *(long long *)value > *(long long *)value_max
                        || *(long long *)value % *(long long *)value_inc != 0
                ))? true : false;
    }
    else if(type_name == "double")
    {
        type_code = 2;
        out_of_limit = (is_limited && (*(double *)value < *(double *)value_min
                        || *(double *)value > *(double *)value_max
                        || *(double *)value - (floor(*(double *)value / *(double *)value_inc) * (*(double *)value_inc)) != 0.00
                ))? true : false;
    }
    else
    {
        type_code = 3;
        out_of_limit = false;
    }
    if(out_of_limit)
    {
        pluto_message_generate(value_name, 2);
        pluto_message.append(" must in range [");
        if(type_code == 1) pluto_message.append(std::to_string(*(long long *)value_min));
        else pluto_message.append(std::to_string(*(double *)value_min));
        pluto_message.append(" : ");
        if(type_code == 1) pluto_message.append(std::to_string(*(long long *)value_inc));
        else pluto_message.append(std::to_string(*(double *)value_inc));
        pluto_message.append(" : ");
        if(type_code == 1) pluto_message.append(std::to_string(*(long long *)value_max));
        else pluto_message.append(std::to_string(*(double *)value_max));
        pluto_message.append("].");
        return 1;
    }
    switch(type_code)
    {
        case 0:
        {
            return_code = iio_channel_attr_write_bool(
                            iio_device_find_channel(device, channel, is_output),
                            attr, *(bool *)value); break;
        }
        case 1:
        {
            return_code = iio_channel_attr_write_longlong(
                            iio_device_find_channel(device, channel, is_output),
                            attr, *(long long *)value); break;
        }
        case 2:
        {
            return_code = iio_channel_attr_write_double(
                            iio_device_find_channel(device, channel, is_output),
                            attr, *(double *)value); break;
        }
        default:
        {
            return_code = 1;
        }
    }
    switch(return_code)
    {
        case 0:
        {
            pluto_message_generate(value_name, 0);
            pluto_message.append(" has been set.");
            break;
        }
        case 1:
        {
            pluto_message_generate("Input type of internal function _pluto_attr_set error.", 2);
            break;
        }
        default:
        {
            pluto_message_generate("Can not set ", 2);
            pluto_message.append(value_name);
            pluto_message.append(".");
            break;
        }
    }
    return return_code;
}

int pluto_sdr::_pluto_attr_get(void *value, struct iio_device *device, char *channel, bool is_output, char *attr, std::string value_name, std::string type_name)
{
    int return_code;
    if(type_name == "bool")
        return_code = iio_channel_attr_read_bool(
                        iio_device_find_channel(device, channel, is_output),
                        attr, (bool *)value);
    else if(type_name == "long long" || type_name == "longlong")
        return_code = iio_channel_attr_read_longlong(
                        iio_device_find_channel(device, channel, is_output),
                        attr, (long long *)value);
    else if(type_name == "double")
        return_code = iio_channel_attr_read_double(
                        iio_device_find_channel(device, channel, is_output),
                        attr, (double *)value);
    else return_code = 1;
    switch(return_code)
    {
        case 0:
        {
            pluto_message_generate(value_name, 0);
            pluto_message.append(" has been gotten.");
            break;
        }
        case 1:
        {
            pluto_message_generate("Input type of internal function _pluto_attr_get error.", 2);
            break;
        }
        default:
        {
            pluto_message_generate("Can not get ", 2);
            pluto_message.append(value_name);
            pluto_message.append(".");
            break;
        }
    }
    return return_code;
}

void pluto_sdr::__pluto_receiver_example()
{
    void *point;
    char *uri = (char *)"ip:192.168.2.1"; // "ip:xxx.xxx.xxx.xxx" or "usb:XX.XX.X";
    long long samprate = 2000000;
    long long rx_freq = 24000000;
    size_t samp_cnt = 4096;

    pluto_init(uri);
    pluto_set_rx_samprate(samprate);
    pluto_set_rx_freq(rx_freq);
    pluto_init_receiver(samp_cnt);

    while (true) {
        pluto_get_rx_data();
        for (point = pluto_receiver.rx_ptr_start; point < pluto_receiver.rx_ptr_end; point = (void *)((ptrdiff_t)point + pluto_receiver.rx_ptr_inc))
        {
            //const int16_t i = ((int16_t*)point)[0]; // Real (I)
            //const int16_t q = ((int16_t*)point)[1]; // Imag (Q)
            /* Process here */
        }
    }
    pluto_destroy();
}

void pluto_sdr::__pluto_transmitter_example()
{
    void *point;
    char *uri = (char *)"ip:192.168.2.1"; // "ip:xxx.xxx.xxx.xxx" or "usb:XX.XX.X";
    long long samprate = 2000000;
    long long tx_freq = 24000000;
    size_t samp_cnt = 4096;

    pluto_init(uri);
    pluto_set_tx_samprate(samprate);
    pluto_set_tx_freq(tx_freq);
    pluto_init_transmitter(samp_cnt);
    pluto_set_tx_gain(-1.00);
    pluto_set_tx_en(true);

    while (true) {
        pluto_schedule_tx_buff();
        for (point = pluto_transmitter.tx_ptr_start; point < pluto_transmitter.tx_ptr_end; point = (void *)((ptrdiff_t)point + pluto_transmitter.tx_ptr_inc))
        {
            //((int16_t*)point)[0] = i << 4; // Real (I)
            //((int16_t*)point)[1] = q << 4; // Imag (Q)
        }
    }
    // Others Code
    pluto_set_tx_en(false);
    pluto_destroy();
}

