import logging

import allure
import pytest
from functional_tests.tests.test_recognition import helpers
from functional_tests.utils import constants

ATTEMPTS = 100
PERCENT = 98

'''
    Home page
'''


@allure.testcase(constants.TASK_LINK.format("VHAT-254"), "VHAT-254")
@pytest.mark.image_recognition
def test_recognition_home_icon(driver):
    if not driver.home_page.home_page_is_active():
        driver.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.HOME_PAGE_ICON, counter)
    check_recognition(counter)


'''
    Climate page
'''


@allure.testcase(constants.TASK_LINK.format("VHAT-259"), "VHAT-259")
@pytest.mark.image_recognition
def test_recognition_climate_up_arrow_temperature(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.CLIMATE_ARROW_UP, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-259"), "VHAT-259")
@pytest.mark.image_recognition
def test_recognition_climate_down_arrow_temperature(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.CLIMATE_ARROW_DOWN, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-260"), "VHAT-260")
@pytest.mark.image_recognition
def test_recognition_climate_switch_on_off_button(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.CLIMATE_ON_OFF_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-261"), "VHAT-261")
@pytest.mark.image_recognition
def test_recognition_climate_top_airflow_button(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.TOP_AIRFLOW_BUTTON, counter)
    check_recognition(counter)


'''
Audio page
'''


@allure.testcase(constants.TASK_LINK.format("VHAT-257"), "VHAT-257")
@pytest.mark.image_recognition
def test_recognition_frequency_button(driver):
    if not driver.audio_page.audio_page_is_active():
        driver.audio_page.open_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.AUDIO_FREQUENCY_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-258"), "VHAT-258")
@pytest.mark.image_recognition
def test_recognition_presets_button(driver):
    if not driver.audio_page.audio_page_is_active():
        driver.audio_page.open_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.PRESETS_BUTTON, counter)
    check_recognition(counter)


'''
Settings page
'''


@allure.testcase(constants.TASK_LINK.format("VHAT-276"), "VHAT-276")
@pytest.mark.image_recognition
def test_recognition_back_settings_button(driver):
    if not driver.settings_audio_page.setting_audio_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.BACK_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-271"), "VHAT-271")
@pytest.mark.image_recognition
def test_recognition_down_scroll_button_active(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.DOWN_SCROLL_BUTTON_ACTIVE, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-270"), "VHAT-270")
@pytest.mark.image_recognition
def test_recognition_up_scroll_button_inactive(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.UP_SCROLL_BUTTON_INACTIVE, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-267"), "VHAT-267")
@pytest.mark.image_recognition
def test_recognition_info_button(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.INFORMATION_DIALOG_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-272"), "VHAT-272")
@pytest.mark.image_recognition
def test_recognition_scroll(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.SCROLL, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-264"), "VHAT-264")
@pytest.mark.image_recognition
def test_recognition_slider(driver):
    if not driver.settings_audio_page.setting_audio_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.SLIDER, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-265"), "VHAT-265")
@pytest.mark.image_recognition
def test_recognition_balance_fade_button(driver):
    if not driver.settings_audio_page.setting_audio_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.BALANCE_FADE_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(constants.TASK_LINK.format("VHAT-274"), "VHAT-274")
@pytest.mark.image_recognition
def test_back_property_field_button(driver):
    driver.settings_audio_page.open_settings_audio_property_field_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.BACK_PROPERTY, counter)
    check_recognition(counter)


def test_recognition_active_check_button(driver):
    if not driver.settings_navigation_page.settings_navigation_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_navigation_page()
    driver.settings_navigation_page.open_route_preferences_page()
    driver.settings_navigation_page.tap_on_down_arrow()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.CHECKED_SETTINGS_NAVIGATION_CHECKBOX_BUTTON, counter)
    check_recognition(counter)


def test_recognition_inactive_check_button(driver):
    if not driver.settings_navigation_page.settings_navigation_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_navigation_page()
    driver.settings_navigation_page.open_route_preferences_page()
    driver.settings_navigation_page.tap_on_down_arrow()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, constants.UNCHECKED_SETTINGS_NAVIGATION_CHECKBOX_BUTTON, counter)
    check_recognition(counter)


def check_recognition(counter):
    logging.info("Recognized {} images".format(counter['success']))
    assert (counter['success'] / ATTEMPTS * 100) > PERCENT, "Image recognition < {}%".format(PERCENT)
