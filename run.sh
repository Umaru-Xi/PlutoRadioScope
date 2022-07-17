#! /bin/bash

# ADALM-Pluto Config
default_pluto_freq="2801.798673"  # in MHz
pluto_uri="ip:192.168.2.1"
pluto_samprate="2.40" # in MHz
pluto_bandwidth="0.26"    #in MHz
pluto_buffersize="4096"
# Android Sensor Server Config
sensor_port="5000"
# Data Output Config
output_file="PlutoPWR_Output.txt"
# Octave Server Config
octave_server_port="5001"
octave_img_size="16"

echo " "
echo "The radio power scope based on ADALM-Pluto and Android sensors. (Umaru Aya)"
echo "This scripts only for configuring PlutoPWR frequency options interactively, others are editable with text editor."
echo "### Remember! You can only save the image of scope in octave plot window! ###"
echo " "
echo "Q: What's the frequency in MHz do you want? [default: $default_pluto_freq]"
read pluto_freq
pluto_freq="${pluto_freq:=$default_pluto_freq}"

echo "Generating octave server scripts."
echo "octave_server_port = ${octave_server_port};" > ./OctaveScript/OctaveServer.m
echo "img_size = ${octave_img_size};" >> ./OctaveScript/OctaveServer.m
cat ./OctaveScript/meta_OctaveServer.m >> ./OctaveScript/OctaveServer.m

echo "Launching scope..."
killall PlutoPWR
killall octave
echo "====== Information ======"
echo "Data Info"
echo "  Output:                 $output_file"
echo "Pluto Info"
echo "  URI:                    $pluto_uri"
echo "  Frequency:              $pluto_freq MHz"
echo "  Samprate:               $pluto_samprate MHz"
echo "  Bandwidth:              $pluto_bandwidth MHz"
echo "  Buffer Size:            $pluto_buffersize"
echo "Sensor Info"
echo "  Android Server Port:    $sensor_port"
echo "Octave Info"
echo "  Client Port:            $octave_server_port"
echo "  Image Size:             $octave_img_size x $octave_img_size"
echo "==== ==== ==== ==== ==== "

./Bin/PlutoPWR -u $pluto_uri -o $output_file -s $sensor_port -O $octave_server_port -S $pluto_samprate -f $pluto_freq -b $pluto_bandwidth -B $pluto_buffersize &
sleep 2
octave ./OctaveScript/OctaveServer.m

killall PlutoPWR
killall octave

echo "Exit scope."

exit 0
