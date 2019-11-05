import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import (
    TASK_LINK, Text, a_c_controls_page_text, audio_direct_tune_text,
    audio_page_text, audio_sirius_page_text, audio_sources_page_text,
    automatic_system_updates_settings_page_text, bluetooth_settings_page_text,
    climate_page_text, clock_settings_page_text, defrost_controls_page_text,
    display_settings_page_text, general_settings_page_text, home_page_text,
    mobile_apps_page_text, mobile_apps_settings_page_text,
    navigation_map_settings_page_text, navigation_route_settings_page_text,
    navigation_settings_page_text, pair_bluetooth_device_page,
    radio_settings_page_text, settings_first_screen_page_text,
    settings_second_screen_page_text, siriusxm_travel_link_text,
    sound_settings_page_text, subscription_info_text,
    sync_connect_settings_page_text, vehicle_settings_page_text,
    voice_control_settings_page_text)

# coordinates for drag down
touch_drag_coordinates_down = ['page', 400, 300, 0, -200]

pytestmark = pytest.mark.regression_sync3


# Home page
@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.parametrize('testdata', home_page_text)
@pytest.mark.smoke_text_recognition
def test_home_page_text(driver_sync3, testdata):
    assert helpers.get_exist_result(testdata)


# Climate page
@allure.testcase(TASK_LINK.format("VHAT-585"), "VHAT-585")
@pytest.mark.parametrize('testdata', climate_page_text)
@pytest.mark.smoke_text_recognition
def test_climate_page_text(climate_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-585"), "VHAT-585")
@pytest.mark.parametrize('testdata', defrost_controls_page_text)
@pytest.mark.smoke_text_recognition
def test_defrost_controls_page_text(climate_defrost_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-585"), "VHAT-585")
@pytest.mark.parametrize('testdata', a_c_controls_page_text)
@pytest.mark.smoke_text_recognition
def test_a_c_controls_page_text(climate_a_c_sync3, testdata):
    assert helpers.get_exist_result(testdata)


# Navigation page
@allure.testcase(TASK_LINK.format("VHAT-587"), "VHAT-587")
@pytest.mark.parametrize('testdata', [Text.NAVIGATION_FAULT_TEXT, Text.NAVIGATION_FAULT_DESCRIPTION_TEXT])
@pytest.mark.smoke_text_recognition
def test_navigation_dialog_text(driver_sync3, testdata):
    driver_sync3.navigation_page.open_navigation_page()
    result = helpers.get_exist_result(testdata)
    driver_sync3.navigation_page.close_information_dialog()
    assert result


# Phone page
@allure.testcase(TASK_LINK.format("VHAT-586"), "VHAT-586")
@pytest.mark.smoke_text_recognition
def test_phone_page_text(phone_sync3):
    assert helpers.get_exist_result(Text.PAIR_PHONE_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-586"), "VHAT-586")
@pytest.mark.parametrize('testdata', pair_bluetooth_device_page)
@pytest.mark.smoke_text_recognition
def test_phone_pair_page_text(phone_pair_sync3, testdata):
    assert helpers.get_exist_result(testdata)


# Mobile Apps
@allure.testcase(TASK_LINK.format("VHAT-588"), "VHAT-588")
@pytest.mark.parametrize('testdata', mobile_apps_page_text)
@pytest.mark.smoke_text_recognition
def test_text_on_mobile_apps_page(mobile_apps_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-588"), "VHAT-588")
@pytest.mark.parametrize('testdata', siriusxm_travel_link_text)
@pytest.mark.smoke_text_recognition
def test_text_siriusxm_travel_link_page(mobile_apps_siriusxm_travel_link_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-588"), "VHAT-588")
@pytest.mark.parametrize('testdata', subscription_info_text)
@pytest.mark.smoke_text_recognition
def test_text_subscription_info_page(mobile_apps_subscription_info_sync3, testdata):
    assert helpers.get_exist_result(testdata)


# Settings page
@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', settings_first_screen_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_page_text_first_screen(settings_first_screen_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', sound_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_sound_page_text(settings_sound_sync3, testdata):
    if testdata == Text.SETTINGS_SOUND_ADAPTIVE_VOLUME_TEXT:
        helpers.touch_and_drag_page(*touch_drag_coordinates_down)
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', clock_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_clock_page_text(settings_clock_sync3, testdata):
    if not settings_clock_sync3.clock_12h_button_is_active():
        settings_clock_sync3.tap_on_24h_button()
    if testdata == Text.SETTINGS_CLOCK_24H_TEXT:
        settings_clock_sync3.tap_on_12h_button()
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', bluetooth_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_bluetooth_page_text(settings_bluetooth_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', radio_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_radio_page_text(settings_radio_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', navigation_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_navigation_page_text(settings_navigation_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', navigation_map_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_navigation_map_preferences_page_text(settings_navigation_map_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', navigation_route_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_navigation_route_preferences_page_text(settings_navigation_route_sync3, testdata):
    if testdata == Text.DYNAMIC_ROUTE_GUIDANCE_TEXT:
        helpers.touch_and_drag_page(*touch_drag_coordinates_down)
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', mobile_apps_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_mobile_apps_page_text(settings_mobile_apps_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', sync_connect_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_sync_connect_page_text(settings_sync_connect_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', settings_second_screen_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_page_text_second_screen(settings_second_screen_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', general_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_general_page_text(settings_general_sync3, testdata):
    if testdata == Text.SETTINGS_GENERAL_ABOUT_SYNC_TEXT:
        helpers.touch_and_drag_page(*touch_drag_coordinates_down)
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', automatic_system_updates_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_automatic_system_updates_page_text(settings_automatic_updates_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', vehicle_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_vehicle_page_text(settings_vehicle_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', display_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_display_page_text(settings_display_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-589"), "VHAT-589")
@pytest.mark.parametrize('testdata', voice_control_settings_page_text)
@pytest.mark.smoke_text_recognition
def test_settings_voice_control_page_text(settings_voice_control_sync3, testdata):
    assert helpers.get_exist_result(testdata)


# Audio page
@allure.testcase(TASK_LINK.format("VHAT-576"), "VHAT-576")
@pytest.mark.parametrize('testdata', audio_page_text)
@pytest.mark.smoke_text_recognition
def test_audio_page_text(audio_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-576"), "VHAT-576")
@pytest.mark.parametrize('testdata', audio_direct_tune_text)
@pytest.mark.smoke_text_recognition
def test_audio_direct_tune_text(audio_direct_tune_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-576"), "VHAT-576")
@pytest.mark.parametrize('testdata', audio_sources_page_text)
@pytest.mark.smoke_text_recognition
def test_audio_sources_page_text(audio_sources_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-576"), "VHAT-576")
@pytest.mark.parametrize('testdata', audio_sirius_page_text)
@pytest.mark.smoke_text_recognition
def test_audio_sirius_page_text(audio_sirius_sync3, testdata):
    assert helpers.get_exist_result(testdata)
