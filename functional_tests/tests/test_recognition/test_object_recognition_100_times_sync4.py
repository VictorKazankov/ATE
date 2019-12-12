import logging

import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync4.constants import TASK_LINK, Icons, Text

ATTEMPTS = 100
PERCENT = 98

pytestmark = pytest.mark.regression_sync4


@allure.testcase(TASK_LINK.format("VHAT-276"), "VHAT-276")
@pytest.mark.image_recognition
def test_recognition_add_phone_settings_button(settings_sync4):
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.SETTINGS_ADD_PHONE_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-273"), "VHAT-273")
@pytest.mark.image_recognition
def test_recognition_radio_button(settings_sync4):
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.SETTINGS_RADIO_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-272"), "VHAT-272")
@pytest.mark.image_recognition
def test_recognition_general_settings_icon(settings_sync4):
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.SETTINGS_GENERAL_SETTINGS_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-274"), "VHAT-274")
@pytest.mark.image_recognition
def test_recognition_phone_button_panel(driver_sync4):
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.MAIN_PHONE_BUTTON_INACTIVE, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-271"), "VHAT-271")
@pytest.mark.image_recognition
def test_recognition_main_panel_apps_button_clock(driver_sync4):
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.MAIN_APPS_BUTTON_INACTIVE, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-363"), "VHAT-363")
@pytest.mark.text_recognition
def test_recognition_text_on_button_in_phone_page(driver_sync4):
    driver_sync4.phone_page.open_phone_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Text.ADD_PHONE_TEXT, counter)
    check_recognition(counter)


def check_recognition(counter):
    logging.info("Recognized {} images".format(counter['success']))
    assert (counter['success'] / ATTEMPTS * 100) > PERCENT, "Image recognition < {}%".format(PERCENT)
