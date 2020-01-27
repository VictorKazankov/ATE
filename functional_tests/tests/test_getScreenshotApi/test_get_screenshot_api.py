import time

import pytest
from vhat_client import attachToApplication as attach_to_application
from vhat_client import getScreenshot as get_screenshot

from functional_tests import config_reader
from functional_tests.utils.ssh_connect import start, execute_command

default_folder = config_reader.read_configuration('GET_SCREENSHOT')['location']

file_name = 'screenshot1.png'

folder_name = 'screenshots'


@pytest.fixture(scope='module', autouse=True)
def attach_to_app():
    attach_to_application('')


@pytest.fixture(scope='function')
def client():
    client = start()
    execute_command(client, 'cd {}; sudo rm -r "screenshots"'.format(default_folder),
                    True, True)
    execute_command(client, 'cd {}; sudo mkdir screenshots'.format(default_folder), True)
    yield client
    execute_command(client, 'cd {}; sudo rm -r screenshots'.format(default_folder),
                    True, True)


def check_content(client, path, name):
    content_command = 'ls "{}"'.format(path)
    content = execute_command(client, content_command).read().strip()
    assert content == name


def check_size(client, path, name):
    size_command = 'cd "{}"; stat -c %s "{}"'.format(path, name)
    size = int(execute_command(client, size_command).read().strip())
    assert size > 0


def test_default(client):
    assert get_screenshot(file_name, folder_name)
    path = "/".join([default_folder, folder_name])
    check_content(client, path, file_name)
    check_size(client, path, file_name)


def test_nested_folder(client):
    nested_folder = folder_name + '/nested'
    execute_command(client, 'cd {}; sudo mkdir {}'.format(default_folder, nested_folder), True)
    assert get_screenshot(file_name, nested_folder)
    path = "/".join([default_folder, nested_folder])
    check_content(client, path, file_name)
    check_size(client, path, file_name)


def test_spaces(client):
    file_name_with_spaces = 'my ' + file_name
    folder_with_spaces = folder_name + '/my screen shots'
    execute_command(client, 'cd {}; sudo mkdir "{}"'.format(default_folder, folder_with_spaces), True)
    assert get_screenshot(file_name_with_spaces, folder_with_spaces)
    path = "/".join([default_folder, folder_with_spaces])
    check_content(client, path, file_name_with_spaces)
    check_size(client, path, file_name_with_spaces)


def test_folder_does_not_exist(client):
    absent_folder = folder_name + '/absent'
    assert get_screenshot(file_name, absent_folder)
    path = "/".join([default_folder, absent_folder])
    check_content(client, path, file_name)


def test_replaced(client):
    assert get_screenshot(file_name, folder_name)
    path = "/".join([default_folder, folder_name, file_name])
    modified_time_command = 'date -r {} +%s'.format(path)
    modified_time_old = int(execute_command(client, modified_time_command).read().strip())
    # sleep for time changed event
    time.sleep(1)
    assert get_screenshot(file_name, folder_name)
    modified_time_new = int(execute_command(client, modified_time_command).read().strip())
    assert modified_time_new > modified_time_old


@pytest.mark.parametrize('test_data', [('abc', RuntimeError),
                                       ('', RuntimeError),
                                       (1, TypeError),
                                       (None, TypeError),
                                       (True, TypeError)])
def test_unexpected_file_name(client, test_data):
    name_negative, error = test_data
    with pytest.raises(error):
        get_screenshot(name_negative, folder_name)


@pytest.mark.parametrize('test_data', [(None, TypeError),
                                       (True, TypeError)])
def test_unexpected_location(client, test_data):
    folder_negative, error = test_data
    with pytest.raises(error):
        get_screenshot(file_name, folder_negative)

# TODO: Need to clarify expected behavior for special symbols filename/folder
#
# @pytest.mark.parametrize(list(string.punctuation))
# @pytest.mark.parametrize(list(string.punctuation))
# def test_negative_name(client, file, dir):
#     print file
#     print dir
