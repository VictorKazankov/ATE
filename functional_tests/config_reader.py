import configparser
import os


def read_configuration(name):
    old = os.getcwd()
    configuration = configparser.ConfigParser()
    os.chdir(os.path.dirname(__file__))
    configuration.read('test_config.ini')
    os.chdir(old)
    return configuration[name]


def get_absolute_path():
    return os.path.dirname(__file__)


def read_configuration_from_file(file_name, section=None, option=None, absolute_path=None):
    configuration = configparser.ConfigParser()

    if absolute_path:
        path = os.path.join(absolute_path, file_name)
    else:
        path = os.path.join(get_absolute_path(), 'config', file_name)

    if os.path.isfile(path):
        configuration.read(path)
    else:
        raise IOError('No such config file {}'.format(path))

    if section is None and option is None:
        return configuration
    elif option is None:
        return configuration[section]
    elif section and option:
        value = configuration.get(section, option).strip()
        if (value[0] == "[") and (value[-1] == "]"):
            return eval(value)
        elif (value[0] == "{") and (value[-1] == "}"):
            return eval(value)
        else:
            return value
