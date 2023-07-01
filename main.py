import ESP32_interface
import audio
import virtual_keyboard
import config as con
import logging
import serial
from time import sleep


def main():

    # parameters
    config_dir = 'configurations/config0.yaml'

    # Status bits
    logged_pot_received = False

    # setup log
    logging.basicConfig(format='%(asctime)s - %(levelname)s - %(message)s', level=logging.INFO)
    file_handler = logging.FileHandler('log.txt')
    file_handler.setLevel(logging.INFO)
    file_handler.setFormatter(logging.Formatter('%(asctime)s - %(levelname)s - %(message)s'))
    logging.getLogger().addHandler(file_handler)

    # log initial available apps
    logging.info(f'initial available apps: {audio.available_apps()}')

    # load config
    config = con.load_config(config_dir)

    # values needed from config
    update_delay = config['settings']['update_delay'] / 1000  # serial update delay for pots
    baud_rate = config['settings']['baud_rate']  # baud rate
    port = config['settings']['port']  # get com port
    button_functions = [config['buttons'][button]['function'] for button in range(len(config['buttons']))]

    # initiate coms
    ser = ESP32_interface.establish_connection(port, baud_rate)

    while True:
        try:
            if ser.in_waiting > 0:
                data = ESP32_interface.read(ser)
                # Determine what to do with message

                match data:

                    # ESP32 config request
                    case 'Config request':
                        logging.info('Sent config')
                        logging.info(ESP32_interface.send_config(ser, config))

                    # ESP32 gave pot
                    case 'pot':

                        # log only once
                        if not logged_pot_received:
                            logging.info('Pot data received')
                            logged_pot_received = True

                        # read pot, split pot id and value, then convert to int
                        pot_id, pot_val = [int(x) for x in ESP32_interface.read(ser).split(',')]

                        print(pot_id, pot_val)

                        # get config apps and available apps
                        available_apps = audio.available_apps()
                        try:
                            config_apps = config['pots'][pot_id]['app']
                        except:
                            print(pot_id)
                            print(len(config['pots']))
                        # if multiple config apps
                        if isinstance(config_apps, list):
                            # scan each as set each
                            for app in config_apps:
                                if app in available_apps:
                                    print(app)
                                    audio.set_app_volume(app, pot_val / 4095)
                        else:
                            # scan each as set each
                            if config_apps in available_apps:
                                audio.set_app_volume(config_apps, pot_val / 4095)
                        # For each app in the config file, set it if it is running

                    case 'button':
                        # get button ID
                        button = int(ESP32_interface.read(ser))
                        virtual_keyboard.keypress(button_functions[button])

                    # Else
                    case _:
                        logging.info('Serial received: ' + str(data))

        # Monitor for dropped serial connection
        except serial.serialutil.SerialException:
            logging.error('Serial dropped. Reconnecting...')
            ser.close()
            ser = ESP32_interface.establish_connection(port, baud_rate)

        sleep(update_delay)

if __name__ == '__main__':
    main()
