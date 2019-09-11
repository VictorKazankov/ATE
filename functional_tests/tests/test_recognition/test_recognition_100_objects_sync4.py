import logging

import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync4 import constants

PERCENT = 98


@allure.testcase(constants.TASK_LINK.format("VHAT-342"), "VHAT-342")
@pytest.mark.text_recognition
def test_recognition_100_different_objects_1_time(driver_sync4):
    if not driver_sync4.phone_page.phone_page_is_active():
        driver_sync4.phone_page.open_phone_page()
    counter = {'success': 0, 'fail': 0}

    button_panel(counter, driver_sync4)
    phone_page(counter, driver_sync4)
    climate_page(counter, driver_sync4)
    audio_page(counter, driver_sync4)
    settings_page(counter, driver_sync4)

    check_recognition(counter)


def button_panel(counter, driver_sync4):
    if not driver_sync4.phone_page.phone_page_is_active():
        driver_sync4.phone_page.open_phone_page()
    helpers.get_result_recognition(constants.FAVORITES_PAGE_PANEL_BUTTON, counter)
    helpers.get_result_recognition(constants.APPS_BUTTON_PANEL_INACTIVE, counter)
    helpers.get_result_recognition(constants.SETTINGS_BUTTON_PANEL, counter)
    helpers.get_result_recognition(constants.FEATURES_PAGE_BUTTON, counter)


def phone_page(counter, driver_sync4):
    if not driver_sync4.phone_page.phone_page_is_active():
        driver_sync4.phone_page.open_phone_page()
    helpers.get_result_recognition(constants.PHONE_BUTTON, counter)
    helpers.get_result_recognition(constants.CONNECT_PHONE_TITLE_TEXT, counter)
    helpers.get_result_recognition(constants.PHONE_BUTTON_TEXT, counter)


def climate_page(counter, driver_sync4):
    if not driver_sync4.climate_page.climate_page_is_active():
        driver_sync4.climate_page.open_climate_page()
    helpers.get_result_recognition(constants.AUTO_BUTTON_TEXT, counter)
    helpers.get_result_recognition(constants.DUAL_BUTTON_TEXT, counter)
    helpers.get_result_recognition(constants.CLIMATE_CONTROLS_TEXT, counter)
    helpers.get_result_recognition(constants.SWITCH_ON_OFF_BUTTON, counter)
    helpers.get_result_recognition(constants.UP_ARROW_RED, counter)
    helpers.get_result_recognition(constants.DOWN_ARROW_BLUE, counter)
    helpers.get_result_recognition(constants.AIR_CONDITION_INSIDE, counter)
    helpers.get_result_recognition(constants.MENU_BUTTON, counter)
    helpers.get_result_recognition(constants.GLASS_HEATING, counter)
    helpers.get_result_recognition(constants.UP_AIRFLOW, counter)
    helpers.get_result_recognition(constants.DOWN_AIRFLOW, counter)
    helpers.get_result_recognition(constants.GLASS_HEATING_BACK, counter)
    driver_sync4.climate_page.open_climate_menu()
    helpers.get_result_recognition(constants.ADDITIONAL_TEXT, counter)
    helpers.get_result_recognition(constants.MAX_GLASS_HEATING_INACTIVE, counter)
    helpers.get_result_recognition(constants.MAX_A_C_INACTIVE, counter)
    helpers.get_result_recognition(constants.CLOSE_TEXT, counter)
    driver_sync4.climate_page.tap_on_close_button()


def audio_page(counter, driver_sync4):
    if not driver_sync4.audio_page.audio_page_is_active():
        driver_sync4.audio_page.open_audio_page()
    helpers.get_result_recognition(constants.HD_TEXT, counter)
    helpers.get_result_recognition(constants.SOURCES_TEXT, counter)
    helpers.get_result_recognition(constants.DIRECT_TEXT, counter)
    helpers.get_result_recognition(constants.RADIO_SETTINGS_ICON, counter)

    # audio page - sources page
    driver_sync4.audio_page.tap_on_sources_page()
    helpers.get_result_recognition(constants.ENTERTAINMENT_SOURCES_TEXT, counter)
    helpers.get_result_recognition(constants.AM_TEXT, counter)
    helpers.get_result_recognition(constants.FM_TEXT, counter)
    helpers.get_result_recognition(constants.BLUETOOTH_STEREO_TEXT, counter)
    helpers.get_result_recognition(constants.RADIO_AM_ICON, counter)
    helpers.get_result_recognition(constants.RADIO_FM_ICON, counter)
    helpers.get_result_recognition(constants.BLUETOOTH_ICON, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # audio - direct tune page
    driver_sync4.audio_page.tap_on_direct_tune_button()
    helpers.get_result_recognition(constants.TEXT_BUTTON_1, counter)
    helpers.get_result_recognition(constants.TEXT_BUTTON_2, counter)
    helpers.get_result_recognition(constants.TEXT_BUTTON_3, counter)
    helpers.get_result_recognition(constants.TEXT_BUTTON_4, counter)
    helpers.get_result_recognition(constants.TEXT_BUTTON_5, counter)
    helpers.get_result_recognition(constants.TEXT_BUTTON_6, counter)
    helpers.get_result_recognition(constants.TEXT_BUTTON_7, counter)
    helpers.get_result_recognition(constants.TEXT_BUTTON_8, counter)
    helpers.get_result_recognition(constants.TEXT_BUTTON_9, counter)
    helpers.get_result_recognition(constants.TEXT_BUTTON_0, counter)
    helpers.get_result_recognition(constants.DELETE_BUTTON, counter)
    helpers.get_result_recognition(constants.ENTER, counter)
    helpers.get_result_recognition(constants.CANCEL_TEXT, counter)
    driver_sync4.audio_page.tap_on_cancel_button()


def settings_page(counter, driver_sync4):
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    helpers.get_result_recognition(constants.SOUND_SETTINGS_BUTTON_AND_TITLE_TEXT, counter)
    helpers.get_result_recognition(constants.CLOCK_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(constants.CONNECTION_SETTINGS_BUTTON_TEXT, counter)
    helpers.get_result_recognition(constants.PHONE_BUTTON_TEXT, counter)
    helpers.get_result_recognition(constants.GENERAL_SETTINGS_BUTTON_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_BUTTON, counter)
    helpers.get_result_recognition(constants.CLOCK_SETTINGS_BUTTON, counter)
    helpers.get_result_recognition(constants.ADD_PHONE_SETTINGS_BUTTON, counter)
    helpers.get_result_recognition(constants.RADIO_SETTINGS_ICON, counter)
    helpers.get_result_recognition(constants.MOBILE_APPS_SETTINGS_BUTTON, counter)
    helpers.get_result_recognition(constants.GENERAL_SETTINGS_BUTTON, counter)

    # setting audio page
    driver_sync4.settings_page.open_setting_audio_page()
    helpers.get_result_recognition(constants.SOUND_SETTINGS_BUTTON_AND_TITLE_TEXT, counter)
    helpers.get_result_recognition(constants.TONE_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_BALANCE_FADE_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_SPEED_COMPENSATED_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_OCCUPANCY_MODE_TEXT, counter)
    helpers.get_result_recognition(constants.BACK_BUTTON, counter)

    # setting audio page - tone settings page
    driver_sync4.settings_audio_page.open_tone_settings_page()
    helpers.get_result_recognition(constants.TONE_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_TREBLE_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_MIDRANGE_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_BASS_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_RESET_ALL_TEXT, counter)
    helpers.get_result_recognition(constants.LEFT_ARROW_SLIDER, counter)
    helpers.get_result_recognition(constants.RIGHT_ARROW_SLIDER, counter)
    helpers.get_result_recognition(constants.SLIDER, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # setting audio page - balance/fade settings page
    driver_sync4.settings_audio_page.open_balance_fade_page()
    helpers.get_result_recognition(constants.FADE_TEXT, counter)
    helpers.get_result_recognition(constants.BALANCE_TEXT, counter)
    helpers.get_result_recognition(constants.RESET_BALANCE_FADE_TEXT, counter)
    helpers.get_result_recognition(constants.BALANCE_FADE_UP_ARROW, counter)
    helpers.get_result_recognition(constants.BALANCE_FADE_DOWN_ARROW, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # setting audio page - occupancy mode page
    driver_sync4.settings_audio_page.open_occupancy_mode_page()
    helpers.get_result_recognition(constants.SOUND_SETTINGS_OCCUPANCY_MODE_TITLE_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_RADIO_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_RADIO_BUTTON_ACTIVE, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_OCCUPANCY_MODE_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_DRIVER_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_ALL_SEATS_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_FRONT_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_SETTINGS_REAR_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()
    driver_sync4.settings_page.tap_on_back_button()

    # setting clock page
    driver_sync4.settings_page.open_setting_clock_page()
    helpers.get_result_recognition(constants.AM_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(constants.PM_BUTTON_ACTIVE, counter)
    helpers.get_result_recognition(constants.MODE_BUTTON_INACTIVITY, counter)
    helpers.get_result_recognition(constants.INFO_ICON, counter)
    helpers.get_result_recognition(constants.HOUR_MODE_12_TEXT, counter)
    helpers.get_result_recognition(constants.RESET_CLOCK_GPS_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # settings connection
    driver_sync4.settings_page.open_connection_settings()
    helpers.get_result_recognition(constants.WIFI, counter)
    helpers.get_result_recognition(constants.SYSTEM_WIFI_TEXT, counter)
    helpers.get_result_recognition(constants.BLUETOOTH_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # settings radio
    driver_sync4.settings_page.open_radio_settings()
    helpers.get_result_recognition(constants.RADIO_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(constants.AUTOSET_TEXT, counter)
    helpers.get_result_recognition(constants.REFRESH_BUTTON_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # settings general
    driver_sync4.settings_page.open_general_settings()
    helpers.get_result_recognition(constants.TEMPERATURE_UNITS_TEXT, counter)
    helpers.get_result_recognition(constants.TEMPERATURE_FAHRENHEIT_TEXT, counter)
    helpers.get_result_recognition(constants.LANGUAGE_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()


def check_recognition(counter):
    logging.info("Recognized {} objects".format(counter['success']))
    assert (counter['success']) > PERCENT, "Objects recognition < {}%".format(PERCENT)
