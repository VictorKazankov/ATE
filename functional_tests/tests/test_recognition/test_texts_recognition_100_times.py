import logging

import allure
import pytest
from functional_tests.tests.test_recognition import helpers
from functional_tests.utils import constants

ATTEMPTS = 100
PERCENT = 98


@allure.testcase(constants.TASK_LINK.format("VHAT-255"), "VHAT-255")
@pytest.mark.text_recognition
def test_recognition_navigation_text(driver):
    if not driver.home_page.home_page_is_active():
        driver.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.NAVIGATION_FAULT_TEXT, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-256"), "VHAT-256")
@pytest.mark.text_recognition
def test_recognition_connecting_text(driver):
    if not driver.home_page.home_page_is_active():
        driver.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.CONNECTING_PHONE_TEXT, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-263"), "VHAT-263")
@pytest.mark.text_recognition
def test_recognition_pair_phone_text(driver):
    if not driver.phone_page.phone_page_is_active():
        driver.phone_page.open_phone_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.PAIR_PHONE_TEXT, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-266"), "VHAT-266")
@pytest.mark.text_recognition
def test_recognition_adaptive_volume_text(driver):
    if not driver.settings_audio_page.setting_audio_page_is_active:
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.ADAPTIVE_VOLUME_TEXT, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-268"), "VHAT-268")
@pytest.mark.text_recognition
def test_recognition_text_on_auto_time_zone_update_dialog(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
        driver.settings_clock_page.close_information_dialog()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.AUTO_TIME_ZONE_TEXT, counter)
    driver.settings_clock_page.close_information_dialog()
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-269"), "VHAT-269")
@pytest.mark.text_recognition
def test_recognition_ac_control_text_for_active_button(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
        if not driver.climat_page.ac_control_button_is_active():
            driver.climat_page.tap_on_climate_auto_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.AC_CONTROL_TEXT, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-273"), "VHAT-273")
@pytest.mark.text_recognition
def test_recognition_12h_text_on_clock_format_button(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
        if not driver.settings_clock_page.clock_12h_button_is_active():
            driver.settings_clock_page.tap_on_24h_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.CLOCK_FORMAT_12H_TEXT, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-277"), "VHAT-277")
@pytest.mark.text_recognition
def test_recognition_105_50_frequency_text(driver):
    if not driver.audio_page.audio_page_is_active():
        driver.audio_page.open_audio_page()
    if not driver.audio_page.verify_frequency_value(constants.FREQUENCY_105_50):
        driver.audio_page.set_frequency(constants.FREQUENCY_105_50)
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.FREQUENCY_105_50, counter)
    check_recognition(counter)


def check_recognition(counter):
    logging.info("Recognized {} texts".format(counter['success']))
    assert (counter['success'] / ATTEMPTS * 100) > PERCENT, "Text recognition < {}%".format(PERCENT)
