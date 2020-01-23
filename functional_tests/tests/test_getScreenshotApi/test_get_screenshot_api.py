# from vhat_client import getScreenshot as get_screenshot
import os
import shutil
import string

import pytest

file_name = 'screenshot1.png'


@pytest.fixture(scope='function')
def location():
    screenshots_folder = os.path.dirname(os.path.realpath(__file__)) + '/screenshots'
    shutil.rmtree(screenshots_folder, ignore_errors=True)
    os.mkdir(screenshots_folder)
    yield screenshots_folder
    shutil.rmtree(screenshots_folder, ignore_errors=True)


def test_default(location):
    # assert get_screenshot(file_name, screenshots_folder)
    path = location + '/' + file_name
    assert os.path.isfile(path)


def test_nested_folder(location):
    nested_folder = location + '/nested'
    os.mkdir(nested_folder)
    # assert get_screenshot(file_name, nested_folder)
    path = nested_folder + '/' + file_name
    assert os.path.isfile(path)


def test_spaces(location):
    file_name_with_spaces = 'my ' + file_name
    location_with_spaces = location + '/my screen shots'
    path = location_with_spaces + '/' + file_name_with_spaces
    os.mkdir(location_with_spaces)
    assert os.path.isdir(location_with_spaces)
    # assert get_screenshot(file_name_with_spaces, location_with_spaces)
    assert os.path.isfile(path)


def test_replaced(location):
    # assert get_screenshot(file_name, screenshots_folder)
    path = location + '/' + file_name
    modified_time_old = os.path.getmtime(path)
    print(modified_time_old)
    # assert get_screenshot(file_name, folder)
    modified_time_new = os.path.getmtime(path)
    print(modified_time_new)
    assert modified_time_new > modified_time_old


def test_folder_does_not_exist(location):
    path = location + '/' + file_name
    os.rmdir(location)
    assert not os.path.isdir(location)
    #     assert get_screenshot(file_name, screenshots_folder)
    assert os.path.isfile(path)


def test_missing_parameter():
    pass
    # assert not get_screenshot('one_parameter_only')


def test_excess_parameter():
    pass
    # assert not get_screenshot('first','2nd','3rd')


@pytest.mark.parametrize('file', ['', file_name] + list(string.punctuation))
@pytest.mark.parametrize('dir', ['', file_name] + list(string.punctuation))
def test_negative_name(location, file, dir):
    print file
    print dir
    folder = location + '/' + dir
    print folder
    #     assert not get_screenshot(file, dir)
    assert not os.path.isfile(folder + '/' + file)


@pytest.mark.parametrize('file', [1, None, True])
@pytest.mark.parametrize('dir', [None, 1, False])
def test_negative_type(location, file, dir):
    print file
    print dir
    #     assert not get_screenshot(file, dir)
