import os

from functional_tests.config_reader import get_absolute_path

RAISE_TEXT = 'Provided {} [{}] does not exist'


def path_joiner(*paths):
    """assamptions:
    getting path using location of config_reader.py file
    """
    path = os.path.join(get_absolute_path(), *paths)
    return path


def path_joiner_dir(*paths):
    """example:
    path = path_joiner_dir('tests', 'test_drag')"""
    path = path_joiner(*paths)
    if not os.path.isdir(path):
        raise ValueError(RAISE_TEXT.format('dir', path))
    return path


def path_joiner_file(*paths):
    """example:
        path = path_joiner_file('tests', 'test_drag', 'test_drag_api_sync3.py')"""
    path = path_joiner(*paths)
    if not os.path.isfile(path):
        raise ValueError(RAISE_TEXT.format('file', path))
    return path
