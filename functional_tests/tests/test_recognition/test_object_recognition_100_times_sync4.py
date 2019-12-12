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


@allure.testcase(TASK_LINK.format("VHAT-274"), "VHAT-274")
@pytest.mark.image_recognition
def test_recognition_phone_button_panel(settings_sync4):
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.MAIN_PHONE_BUTTON_INACTIVE, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-273"), "VHAT-273")
@pytest.mark.image_recognition
def test_recognition_12_hour_mode_button(clock_settings_sync4):
    if not clock_settings_sync4.clock_12h_button_is_active():
        clock_settings_sync4.tap_on_24h_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.MODE_BUTTON_INACTIVITY, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-272"), "VHAT-272")
@pytest.mark.image_recognition
def test_recognition_24_hour_mode_button(clock_settings_sync4):
    if not clock_settings_sync4.clock_24h_button_is_active():
        clock_settings_sync4.tap_on_12h_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.MODE_BUTTON_ACTIVITY, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-271"), "VHAT-271")
@pytest.mark.image_recognition
def test_recognition_pm_button_clock(clock_settings_sync4):
    if not clock_settings_sync4.clock_12h_button_is_active():
        clock_settings_sync4.tap_on_24h_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.SETTINGS_CLOCK_PM_BUTTON_ACTIVE, counter)
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
