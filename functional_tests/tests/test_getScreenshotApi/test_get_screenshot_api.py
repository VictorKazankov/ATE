# from vhat_client import getScreenshot as get_screenshot
# from vhat_client import ExtensionError, EmptyFileName

import pytest

from functional_tests import config_reader
from functional_tests.utils.ssh_connect import start, execute_command

default_folder = config_reader.read_configuration('GET_SCREENSHOT')['location']

file_name = 'screenshot1.png'

folder_name = 'screenshots'


@pytest.fixture(scope='function')
def client():
    client = start()
    execute_command(client, 'cd {}; sudo rmdir screenshots'.format(default_folder),
                    True, True)
    execute_command(client, 'cd {}; sudo mkdir screenshots'.format(default_folder), True)
    yield client
    execute_command(client, 'cd {}; sudo rmdir screenshots'.format(default_folder),
                    True, True)


def test_default(client):
    # assert get_screenshot(file_name, screenshots_folder)
    path = "/".join([default_folder, folder_name])
    command = 'ls {}'.format(path)
    res = execute_command(client, command).read().strip()
    assert res == file_name


def test_nested_folder(client):
    nested_folder = folder_name + '/nested'
    # execute_command(client, 'cd {}; sudo mkdir {}'.format(default_folder, nested_folder), True)
    # assert get_screenshot(file_name, nested_folder)
    path = "/".join([default_folder, nested_folder])
    command = 'ls {}'.format(path)
    res = execute_command(client, command).read().strip()
    assert res == file_name


def test_spaces(client):
    file_name_with_spaces = 'my ' + file_name
    folder_with_spaces = folder_name + '/my screen shots'
    # execute_command(client, 'cd {}; sudo mkdir "{}"'.format(default_folder, folder_with_spaces), True)
    path = "/".join([default_folder, folder_with_spaces])
    command = 'ls "{}"'.format(path)
    res = execute_command(client, command).read().strip()
    assert res == file_name


def test_replaced(client):
    # assert get_screenshot(file_name, screenshots_folder)
    path = "/".join([default_folder, folder_name, file_name])
    command = 'date -r {} +%s'.format(path)
    modified_time_old = int(execute_command(client, command).read().strip())
    # assert get_screenshot(file_name, screenshots_folder)
    modified_time_new = int(execute_command(client, command).read().strip())
    assert modified_time_new > modified_time_old


def test_folder_does_not_exist(client):
    ansent_folder = folder_name + '/absent'
    # assert get_screenshot(file_name, ansent_folder)
    path = "/".join([default_folder, ansent_folder])
    command = 'ls {}'.format(path)
    res = execute_command(client, command).read().strip()
    assert res == file_name

# @pytest.mark.parametrize('test_data', [('abc', ExtensionError),
#                                        ('', EmptyFileName),
#                                        (1, TypeError),
#                                        (None, TypeError),
#                                        (True, TypeError)])
# def test_unexpected_file_name(client, test_data):
#     name_negative, error = test_data
#     with pytest.raises(error):
#         get_screenshot(name_negative, folder_name)

# @pytest.mark.parametrize('test_data', [(None, TypeError),
#                                        (True, TypeError)])
# def test_unexpected_location(client, test_data):
#     folder_negative, error = test_data
#     with pytest.raises(error):
#         get_screenshot(file_name, folder_negative)

#
# @pytest.mark.xfail(raises=TypeError)
# def test_missing_parameter():
#     get_screenshot(file_name)

# @pytest.mark.xfail(raises=TypeError)
# def test_excess_parameter():
#     get_screenshot('first.png','2nd','3rd')
#
# TODO: Need to clarify expected behavior for special symbols filename/folder
#
# @pytest.mark.parametrize(list(string.punctuation))
# @pytest.mark.parametrize(list(string.punctuation))
# def test_negative_name(client, file, dir):
#     print file
#     print dir
