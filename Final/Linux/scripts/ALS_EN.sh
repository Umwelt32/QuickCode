#!/bin/sh

#This script might help to enable ALS_Sensor / Works on Asus Nx500Jk

sudo pacman -S acpi_call-dkms dkms linux-cachyos-headers
sudo modprobe acpi_call
ls -l /proc/acpi/call
echo '\_SB.ATKD.ALSC 1' | sudo tee /proc/acpi/call
# Enable ASUS ALS sensor
if [ -w /proc/acpi/call ]; then
echo '\_SB.ATKD.ALSC 1' > /proc/acpi/call
fi

cat /sys/bus/iio/devices/iio:device0/in_illuminance_raw
echo ' ' >> ~/.bashrc
echo 'if [ -w /proc/acpi/call ]; then' >> ~/.bashrc
echo "echo '\_SB.ATKD.ALSC 1' > /proc/acpi/call" >> ~/.bashrc
echo 'fi' >> ~/.bashrc
echo ' ' >> ~/.bashrc

kwrite ~/.bashrc

