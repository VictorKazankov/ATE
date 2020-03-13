from importlib import import_module

from functional_tests.config_reader import read_configuration


def get_env():
    try:
        return str(read_configuration('ENVIRONMENT')['target']).lower()
    except KeyError:
        raise RuntimeError('target is not specified in ENVIRONMENT section of test_config.ini')


env = get_env()

TEST_DATA_PATH_RAW = 'functional_tests.{}.test_data.{}.test_data'


def get_test_data_path(path):
    return TEST_DATA_PATH_RAW.format(env, path)


def do_import(path, param):
    return getattr(import_module(get_test_data_path(path)), param)


def get_test_data(path, name):
    return do_import(path, name)


def data(path, name='test_data'):
    return get_test_data(path, name)
