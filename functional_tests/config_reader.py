import os

import configparser


def read_configuration(name):
    old = os.getcwd()
    configuration = configparser.ConfigParser()
    os.chdir(os.path.dirname(__file__))
    configuration.read('test_config.ini')
    os.chdir(old)
    return configuration[name]
