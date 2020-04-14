import time

import allure
import pytest
from vhat_client import WrongScreenshotExtension, EmptyScreenshotFileName, PermissionDenied
from vhat_client import getScreenshot as get_screenshot

from functional_tests import config_reader
from functional_tests.utils.ssh_connect import start, execute_command
from functional_tests.utils.sync4.constants import TASK_LINK

pytestmark = [pytest.mark.get_screenshot_api, pytest.mark.regression_sync3, pytest.mark.regression_sync4]

default_folder = config_reader.read_configuration('GET_SCREENSHOT')['location']

file_name = 'screenshot1.png'

folder_name = 'screenshots'


@pytest.fixture(scope='function')
def client(attach_to_app):
    client = start()
    execute_command(client, 'cd {}; sudo rm -r "{}"'.format(default_folder, folder_name),
                    True, True)
    execute_command(client, 'cd {}; mkdir {}'.format(default_folder, folder_name), True)
    yield client
    execute_command(client, 'cd {}; sudo rm -r {}'.format(default_folder, folder_name),
                    True, True)


def check_content(client, path, name):
    content_command = 'ls "{}"'.format(path)
    content = execute_command(client, content_command).read().strip()
    assert content == name


def check_size(client, path, name):
    size_command = 'cd "{}"; stat -c %s "{}"'.format(path, name)
    size = int(execute_command(client, size_command).read().strip())
    assert size > 0


@allure.testcase(TASK_LINK.format("VHAT-1371"), "VHAT-1371")
def test_default(client):
    assert get_screenshot(file_name, folder_name)
    path = "/".join([default_folder, folder_name])
    check_content(client, path, file_name)
    check_size(client, path, file_name)


@allure.testcase(TASK_LINK.format("VHAT-1372"), "VHAT-1372")
def test_nested_folder(client):
    nested_folder = folder_name + '/nested'
    execute_command(client, 'cd {}; mkdir {}'.format(default_folder, nested_folder), True)
    assert get_screenshot(file_name, nested_folder)
    path = "/".join([default_folder, nested_folder])
    check_content(client, path, file_name)
    check_size(client, path, file_name)


@allure.testcase(TASK_LINK.format("VHAT-1373"), "VHAT-1373")
def test_spaces(client):
    file_name_with_spaces = 'my ' + file_name
    folder_with_spaces = folder_name + '/my screen shots'
    execute_command(client, 'cd {}; mkdir "{}"'.format(default_folder, folder_with_spaces), True)
    assert get_screenshot(file_name_with_spaces, folder_with_spaces)
    path = "/".join([default_folder, folder_with_spaces])
    check_content(client, path, file_name_with_spaces)
    check_size(client, path, file_name_with_spaces)


@allure.testcase(TASK_LINK.format("VHAT-1374"), "VHAT-1374")
def test_folder_does_not_exist(client):
    absent_folder = folder_name + '/absent'
    assert get_screenshot(file_name, absent_folder)
    path = "/".join([default_folder, absent_folder])
    check_content(client, path, file_name)


@allure.testcase(TASK_LINK.format("VHAT-1375"), "VHAT-1375")
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


@allure.testcase(TASK_LINK.format("VHAT-1440"), "VHAT-1440")
def test_permission_denied(client):
    execute_command(client, 'sudo chown root {}'.format('/'.join([default_folder, folder_name])),
                    True)
    with pytest.raises(PermissionDenied):
        get_screenshot(file_name, folder_name)


@allure.testcase(TASK_LINK.format("VHAT-1376"), "VHAT-1376")
@pytest.mark.parametrize('test_data', [('abc', WrongScreenshotExtension),
                                       ('', EmptyScreenshotFileName),
                                       (1, TypeError),
                                       (None, TypeError),
                                       (True, TypeError)])
def test_unexpected_file_name(client, test_data):
    name_negative, error = test_data
    with pytest.raises(error):
        get_screenshot(name_negative, folder_name)


@allure.testcase(TASK_LINK.format("VHAT-1377"), "VHAT-1377")
@pytest.mark.parametrize('test_data', [(None, TypeError),
                                       (True, TypeError)])
def test_unexpected_location(client, test_data):
    folder_negative, error = test_data
    with pytest.raises(error):
        get_screenshot(file_name, folder_negative)
