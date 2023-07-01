import serial
import logging
import sys
from time import sleep


def establish_connection(port, baud_rate):
    error_reported = False
    while True:
        try:
            ser = serial.Serial(port, baud_rate)
            try:
                ser.open()
            except IOError:
                logging.info('port is already open')
                pass
            logging.info(f'ESP32 Serial connection established on port {port} with bause_rate {baud_rate}.')
            return ser
        except serial.serialutil.SerialException:
            if not error_reported:
                logging.error('Could not connect to ESP32. Retrying...')
                error_reported = True
        sleep(1)


def read(ser):
    error_reported = False
    while True:
        try:
            return ser.readline().decode('ascii').rstrip()
        except UnicodeDecodeError:
            if not error_reported:
                logging.error('Could not interpret message. Listening again...')
                error_reported = True
    sleep(1)


def send_config(ser, config):
    ser.write(str(config).encode())  # Send the config YAML over serial
    return ser.readline().decode().strip()  # Wait for acknowledgment from ESP32 and print response
