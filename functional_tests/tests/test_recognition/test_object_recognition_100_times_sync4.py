import logging

import allure
import pytest
from functional_tests.tests.test_recognition import helpers
from functional_tests.utils.sync4 import constants

ATTEMPTS = 100
PERCENT = 98


# image on button

@allure.testcase(constants.TASK_LINK.format("VHAT-363"), "VHAT-363")
@pytest.mark.image_recognition
def test_recognition_add_phone_settings_button(driver_sync4):
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.ADD_PHONE_SETTINGS_BUTTON, counter)
    check_recognition(counter)


# image on panel

@allure.testcase(constants.TASK_LINK.format("VHAT-363"), "VHAT-363")
@pytest.mark.image_recognition
def test_recognition_phone_button_panel(driver_sync4):
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.PHONE_PAGE_PANEL_BUTTON, counter)
    check_recognition(counter)


# inactivity mode object image

@allure.testcase(constants.TASK_LINK.format("VHAT-363"), "VHAT-363")
@pytest.mark.image_recognition
def test_recognition_12_hour_mode_button(driver_sync4):
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    driver_sync4.settings_page.open_setting_clock_page()
    if not driver_sync4.settings_clock_page.clock_12h_button_is_active():
        driver_sync4.settings_clock_page.tap_on_24h_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.MODE_BUTTON_INACTIVITY, counter)
    check_recognition(counter)


# activity mode object image

@allure.testcase(constants.TASK_LINK.format("VHAT-363"), "VHAT-363")
@pytest.mark.image_recognition
def test_recognition_24_hour_mode_button(driver_sync4):
    driver_sync4.phone_page.open_phone_page()
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    driver_sync4.settings_page.open_setting_clock_page()
    if not driver_sync4.settings_clock_page.clock_24h_button_is_active():
        driver_sync4.settings_clock_page.tap_on_12h_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.MODE_BUTTON_ACTIVITY, counter)
    check_recognition(counter)


# text on button

@allure.testcase(constants.TASK_LINK.format("VHAT-363"), "VHAT-363")
@pytest.mark.image_recognition
def test_recognition_pm_button_clock(driver_sync4):
    driver_sync4.phone_page.open_phone_page()
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    driver_sync4.settings_page.open_setting_clock_page()
    if not driver_sync4.settings_clock_page.clock_12h_button_is_active():
        driver_sync4.settings_clock_page.tap_on_24h_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.PM_BUTTON_ACTIVE, counter)
    check_recognition(counter)


# text on button

@allure.testcase(constants.TASK_LINK.format("VHAT-363"), "VHAT-363")
@pytest.mark.text_recognition
def test_recognition_text_on_button_in_phone_page(driver_sync4):
    if not driver_sync4.phone_page.phone_page_is_active():
        driver_sync4.phone_page.open_phone_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.ADD_PHONE_TEXT, counter)
    check_recognition(counter)


def check_recognition(counter):
    logging.info("Recognized {} images".format(counter['success']))
    assert (counter['success'] / ATTEMPTS * 100) > PERCENT, "Image recognition < {}%".format(PERCENT)
