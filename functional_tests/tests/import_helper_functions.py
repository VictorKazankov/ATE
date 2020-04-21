from importlib import import_module

from functional_tests.config_reader import read_configuration

_TEST_DATA_PATH_RAW = 'functional_tests.{}.test_data.{}.test_data'


# TODO:: replace usage of environment configuration during VHAT-1845 implementation
def _get_env():
    """
    to get current environment
    :return: value mentioned in tests config ini file
    """
    try:
        return str(read_configuration('ENVIRONMENT')['target']).lower()
    except KeyError:
        raise RuntimeError('target is not specified in ENVIRONMENT section of test_config.ini')


def _get_path_complete(path):
    """
    to achieve module for import path complete using environment
    :param path: name of the package with test data
    :return: module path string formatted by environment and package name
    """
    return _TEST_DATA_PATH_RAW.format(_get_env(), path)


def data(path, name='test_data'):
    """
    to get environment specific test data
    Example: data('test_wait_for_object', name='icon')
    :param path: name of the package to look test_data module in
    :param name: name of the attribute of the module (default = 'test_data')
    :return: value of the attribute of the module
    """
    try:
        return getattr(import_module(_get_path_complete(path)), name)
    except AttributeError:
        raise AttributeError('Unable to find {} test data attribute'
                             ' in test_data module of {}.test_data.{} package'.format(name, _get_env(), path))


class Data(object):
    """
    Duplicates data function above with attributes usage
    Example: Data("text_exist").icon returns icon attribute of test_data module
    placed in test_exists package
    """

    def __init__(self, path):
        """
        :param path: package name to find test data
        """
        self._path = path

    def __getattr__(self, name):
        """
        :param name: name of the attribute of the test data module
        :return: value of the attribute
        """
        return data(self._path, name)
