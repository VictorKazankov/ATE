import logging

import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import TASK_LINK, Icons, Text

ATTEMPTS = 100
PERCENT = 98

'''
    Home page
'''


@allure.testcase(TASK_LINK.format("VHAT-254"), "VHAT-254")
@pytest.mark.image_recognition
def test_recognition_home_icon(driver_sync3):
    if not driver_sync3.home_page.home_page_is_active():
        driver_sync3.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.HOME_PAGE_ICON, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-255"), "VHAT-255")
@pytest.mark.text_recognition
def test_recognition_navigation_text(driver_sync3):
    if not driver_sync3.home_page.home_page_is_active():
        driver_sync3.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Text.NAVIGATION_FAULT_TEXT, counter)
    check_recognition(counter)


'''
    Climate page
'''


@allure.testcase(TASK_LINK.format("VHAT-259"), "VHAT-259")
@pytest.mark.image_recognition
def test_recognition_climate_up_arrow_temperature(driver_sync3):
    if not driver_sync3.climat_page.climate_page_is_active():
        driver_sync3.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.CLIMATE_ARROW_UP, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-259"), "VHAT-259")
@pytest.mark.image_recognition
def test_recognition_climate_down_arrow_temperature(driver_sync3):
    if not driver_sync3.climat_page.climate_page_is_active():
        driver_sync3.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.CLIMATE_ARROW_DOWN, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-260"), "VHAT-260")
@pytest.mark.image_recognition
def test_recognition_climate_switch_on_off_button(driver_sync3):
    if not driver_sync3.climat_page.climate_page_is_active():
        driver_sync3.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.CLIMATE_ON_OFF_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-261"), "VHAT-261")
@pytest.mark.image_recognition
def test_recognition_climate_top_airflow_button(driver_sync3):
    if not driver_sync3.climat_page.climate_page_is_active():
        driver_sync3.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.TOP_AIRFLOW_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-269"), "VHAT-269")
@pytest.mark.text_recognition
def test_recognition_auto_text_on_climate_page(driver_sync3):
    if not driver_sync3.climat_page.climate_page_is_active():
        driver_sync3.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Text.AUTO_TEXT, counter)
    check_recognition(counter)


'''
Audio page
'''


@allure.testcase(TASK_LINK.format("VHAT-257"), "VHAT-257")
@pytest.mark.image_recognition
def test_recognition_frequency_button(driver_sync3):
    if not driver_sync3.audio_page.audio_page_is_active():
        driver_sync3.audio_page.open_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.AUDIO_FREQUENCY_BUTTON, counter)
    check_recognition(counter)


@allure.testcase(TASK_LINK.format("VHAT-258"), "VHAT-258")
@pytest.mark.image_recognition
def test_recognition_presets_button(driver_sync3):
    if not driver_sync3.audio_page.audio_page_is_active():
        driver_sync3.audio_page.open_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition_in_loop(ATTEMPTS, Icons.PRESETS_BUTTON, counter)
    check_recognition(counter)


def check_recognition(counter):
    logging.info("Recognized {} images".format(counter['success']))
    assert (counter['success'] / ATTEMPTS * 100) > PERCENT, "Image recognition < {}%".format(PERCENT)
