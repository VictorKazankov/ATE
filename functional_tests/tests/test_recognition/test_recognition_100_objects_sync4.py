import logging

import allure
from functional_tests.tests import helpers
from functional_tests.utils.sync4.constants import TASK_LINK, Icons, Text

PERCENT = 98


@allure.testcase(TASK_LINK.format("VHAT-342"), "VHAT-342")
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
    driver_sync4.phone_page.open_phone_page()
    helpers.get_result_recognition(Icons.MAIN_FAVORITES_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Icons.MAIN_APPS_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Icons.MAIN_PANEL_SETTINGS_BUTTON_ACTIVE, counter)
    helpers.get_result_recognition(Icons.MAIN_FEATURES_BUTTON_INACTIVE, counter)


def phone_page(counter, driver_sync4):
    driver_sync4.phone_page.open_phone_page()
    helpers.get_result_recognition(Icons.MAIN_PHONE_BUTTON_ACTIVE, counter)
    helpers.get_result_recognition(Text.PHONE_CONNECT_PHONE_TITLE_TEXT, counter)
    helpers.get_result_recognition(Text.PHONE_BUTTON_TEXT, counter)


def climate_page(counter, driver_sync4):
    if not driver_sync4.climate_page.climate_page_is_active():
        driver_sync4.climate_page.open_climate_page()
    helpers.get_result_recognition(Text.CLIMATE_PAGE_AUTO_BUTTON_TEXT, counter)
    helpers.get_result_recognition(Text.CLIMATE_PAGE_DUAL_BUTTON_TEXT, counter)
    helpers.get_result_recognition(Text.CLIMATE_PAGE_CONTROLS_TITLE_TEXT, counter)
    helpers.get_result_recognition(Icons.CLIMATE_SWITCH_ON_OFF_BUTTON_ACTIVE, counter)
    helpers.get_result_recognition(Icons.ARROW_UP_THIN_BUTTON, counter)
    helpers.get_result_recognition(Icons.ARROW_DOWN_THIN_BUTTON, counter)
    helpers.get_result_recognition(Icons.CLIMATE_AIR_CONDITION_INSIDE, counter)
    helpers.get_result_recognition(Icons.CLIMATE_PAGE_MENU_BUTTON, counter)
    helpers.get_result_recognition(Icons.CLIMATE_FRONT_HEATING_BUTTON, counter)
    helpers.get_result_recognition(Icons.CLIMATE_UP_AIRFLOW_BUTTON, counter)
    helpers.get_result_recognition(Icons.CLIMATE_DOWN_AIRFLOW_BUTTON, counter)
    helpers.get_result_recognition(Icons.CLIMATE_REAR_HEATING_BUTTON, counter)
    driver_sync4.climate_page.open_climate_menu()
    helpers.get_result_recognition(Text.CLIMATE_MENU_PAGE_POPUP_TITLE_TEXT, counter)
    helpers.get_result_recognition(Icons.CLIMATE_MENU_MAX_FRONT_HEATINGBUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Icons.CLIMATE_MENU_MAX_A_C_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Text.CLOSE_TEXT, counter)
    driver_sync4.climate_page.tap_on_close_button()


def audio_page(counter, driver_sync4):
    if not driver_sync4.audio_page.audio_page_is_active():
        driver_sync4.audio_page.open_audio_page()
    helpers.get_result_recognition(Text.HD_TEXT, counter)
    helpers.get_result_recognition(Text.AUDIO_PAGE_SOURCES_TEXT, counter)
    helpers.get_result_recognition(Text.AUDIO_PAGE_DIRECT_TUNE_TEXT, counter)
    helpers.get_result_recognition(Icons.SETTINGS_RADIO_BUTTON, counter)

    # audio page - sources page
    driver_sync4.audio_page.tap_on_sources_page()
    helpers.get_result_recognition(Text.ENTERTAINMENT_SOURCES_TITLE_TEXT, counter)
    helpers.get_result_recognition(Text.AM_TEXT, counter)
    helpers.get_result_recognition(Text.FM_TEXT, counter)
    helpers.get_result_recognition(Text.BLUETOOTH_STEREO_TEXT, counter)
    helpers.get_result_recognition(Icons.AUDIO_AM_ICON, counter)
    helpers.get_result_recognition(Icons.AUDIO_FM_ICON, counter)
    helpers.get_result_recognition(Icons.AUDIO_SOURCES_BLUETOOTH_BUTTON, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # audio - direct tune page
    driver_sync4.audio_page.tap_on_direct_tune_button()
    helpers.get_result_recognition(Text.TEXT_BUTTON_1, counter)
    helpers.get_result_recognition(Text.TEXT_BUTTON_2, counter)
    helpers.get_result_recognition(Text.TEXT_BUTTON_3, counter)
    helpers.get_result_recognition(Text.TEXT_BUTTON_4, counter)
    helpers.get_result_recognition(Text.TEXT_BUTTON_5, counter)
    helpers.get_result_recognition(Text.TEXT_BUTTON_6, counter)
    helpers.get_result_recognition(Text.TEXT_BUTTON_7, counter)
    helpers.get_result_recognition(Text.TEXT_BUTTON_8, counter)
    helpers.get_result_recognition(Text.TEXT_BUTTON_9, counter)
    helpers.get_result_recognition(Text.TEXT_BUTTON_0, counter)
    helpers.get_result_recognition(Icons.AUDIO_DELETE_ICON, counter)
    helpers.get_result_recognition(Icons.AUDIO_DIRECT_ENTER_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Text.CANCEL_TEXT, counter)
    driver_sync4.audio_page.tap_on_cancel_button()


def settings_page(counter, driver_sync4):
    driver_sync4.settings_page.open_settings_page()
    helpers.get_result_recognition(Text.SOUND_SETTINGS_BUTTON_TEXT, counter)
    helpers.get_result_recognition(Text.CLOCK_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(Text.CONNECTION_SETTINGS_BUTTON_TEXT, counter)
    helpers.get_result_recognition(Text.PHONE_BUTTON_TEXT, counter)
    helpers.get_result_recognition(Text.GENERAL_SETTINGS_BUTTON_TEXT, counter)
    helpers.get_result_recognition(Icons.SOUND_SETTINGS_BUTTON, counter)
    helpers.get_result_recognition(Icons.SETTINGS_CLOCK_BUTTON, counter)
    helpers.get_result_recognition(Icons.SETTINGS_ADD_PHONE_BUTTON, counter)
    helpers.get_result_recognition(Icons.SETTINGS_RADIO_BUTTON, counter)
    helpers.get_result_recognition(Icons.SETTINGS_MOBILE_APPS_BUTTON, counter)
    helpers.get_result_recognition(Icons.GENERAL_SETTINGS_BUTTON, counter)

    # setting audio page
    driver_sync4.settings_page.open_sound_settings_page()
    helpers.get_result_recognition(Text.SOUND_SETTINGS_BUTTON_TEXT, counter)
    helpers.get_result_recognition(Text.TONE_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_BALANCE_FADE_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_SPEED_COMPENSATED_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_OCCUPANCY_MODE_TEXT, counter)
    helpers.get_result_recognition(Icons.BACK_BUTTON, counter)

    # setting audio page - tone settings page
    driver_sync4.settings_audio_page.open_tone_settings_page()
    helpers.get_result_recognition(Text.TONE_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_TREBLE_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_MIDRANGE_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_BASS_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_RESET_ALL_TEXT, counter)
    helpers.get_result_recognition(Icons.LEFT_ARROW_BUTTON, counter)
    helpers.get_result_recognition(Icons.RIGHT_ARROW_SLIDER, counter)
    helpers.get_result_recognition(Icons.SLIDER, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # setting audio page - balance/fade settings page
    driver_sync4.settings_audio_page.open_balance_fade_page()
    helpers.get_result_recognition(Text.FADE_TEXT, counter)
    helpers.get_result_recognition(Text.BALANCE_TEXT, counter)
    helpers.get_result_recognition(Text.RESET_BALANCE_FADE_TEXT, counter)
    helpers.get_result_recognition(Icons.ARROW_DOWN_BUTTON, counter)
    helpers.get_result_recognition(Icons.ARROW_UP_BUTTON, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # setting audio page - occupancy mode page
    driver_sync4.settings_audio_page.open_occupancy_mode_page()
    helpers.get_result_recognition(Text.SOUND_SETTINGS_OCCUPANCY_MODE_TITLE_TEXT, counter)
    helpers.get_result_recognition(Icons.SOUND_SETTINGS_RADIO_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Icons.SOUND_SETTINGS_RADIO_BUTTON_ACTIVE, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_OCCUPANCY_MODE_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_DRIVER_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_ALL_SEATS_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_FRONT_TEXT, counter)
    helpers.get_result_recognition(Text.SOUND_SETTINGS_REAR_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()
    driver_sync4.settings_page.tap_on_back_button()

    # setting clock page
    driver_sync4.settings_page.open_setting_clock_page()
    helpers.get_result_recognition(Icons.SETTINGS_CLOCK_AM_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Icons.SETTINGS_CLOCK_PM_BUTTON_ACTIVE, counter)
    helpers.get_result_recognition(Icons.MODE_BUTTON_INACTIVITY, counter)
    helpers.get_result_recognition(Icons.INFO_ICON, counter)
    helpers.get_result_recognition(Text.HOUR_MODE_12_TEXT, counter)
    helpers.get_result_recognition(Text.RESET_CLOCK_GPS_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # settings connection
    driver_sync4.settings_page.open_connection_settings()
    helpers.get_result_recognition(Icons.WIFI, counter)
    helpers.get_result_recognition(Text.CONNECTION_SETTINGS_MANAGE_WIFI, counter)
    helpers.get_result_recognition(Text.BLUETOOTH_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # settings radio
    driver_sync4.settings_page.open_radio_settings()
    helpers.get_result_recognition(Text.RADIO_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(Text.AUTOSET_TEXT, counter)
    helpers.get_result_recognition(Text.REFRESH_BUTTON_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()

    # settings general
    driver_sync4.settings_page.open_general_settings()
    helpers.get_result_recognition(Text.GENERAL_SETTINGS_TEMPERATURE_UNITS_TEXT, counter)
    helpers.get_result_recognition(Text.TEMPERATURE_FAHRENHEIT_TEXT, counter)
    helpers.get_result_recognition(Text.LANGUAGE_TEXT, counter)
    driver_sync4.settings_page.tap_on_back_button()


def check_recognition(counter):
    logging.info("Recognized {} objects".format(counter['success']))
    assert (counter['success']) > PERCENT, "Objects recognition < {}%".format(PERCENT)
