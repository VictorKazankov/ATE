import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import TASK_LINK, Text


@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.smoke_text_recognition
def test_connect_phone_text(driver_sync3):
    assert helpers.get_exist_result(Text.CONNECT_A_PHONE_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.smoke_text_recognition
def test_navigation_fault_text(driver_sync3):
    assert helpers.get_exist_result(Text.NAVIGATION_FAULT_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.smoke_text_recognition
def test_audio_button_panel_text(driver_sync3):
    assert helpers.get_exist_result(Text.AUDIO_BUTTON_PANEL_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.smoke_text_recognition
def test_climate_button_panel_text(driver_sync3):
    assert helpers.get_exist_result(Text.CLIMATE_BUTTON_PANEL_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.smoke_text_recognition
def test_phone_button_panel_text(driver_sync3):
    assert helpers.get_exist_result(Text.PHONE_BUTTON_PANEL_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.smoke_text_recognition
def test_navigation_button_panel_text(driver_sync3):
    assert helpers.get_exist_result(Text.NAVIGATION_BUTTON_PANEL_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.smoke_text_recognition
def test_mobile_apps_button_panel_text(driver_sync3):
    assert helpers.get_exist_result(Text.MOBILE_APPS_BUTTON_PANEL_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.smoke_text_recognition
def test_settings_button_panel_text(driver_sync3):
    assert helpers.get_exist_result(Text.SETTINGS_BUTTON_PANEL_TEXT)
