import logging

import allure
import pytest
from functional_tests.tests.test_recognition import helpers
from functional_tests.utils.sync3 import constants

PERCENT = 98


@allure.testcase(constants.TASK_LINK.format("VHAT-284"), "VHAT-284")
@pytest.mark.text_recognition
def test_recognition_100_different_objects_1_time(driver_sync3):
    if not driver_sync3.home_page.home_page_is_active():
        driver_sync3.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}

    home_page(counter)
    audio_page(counter, driver_sync3)
    climate_page(counter, driver_sync3)
    phone_page(counter, driver_sync3)
    mobile_apps_page(counter, driver_sync3)
    main_settings_page(counter, driver_sync3)
    sound_settings_page(counter, driver_sync3)
    clock_settings_page(counter, driver_sync3)
    bluetooth_settings_page(counter, driver_sync3)
    radio_settings_page(counter, driver_sync3)
    navigation_settings_page(counter, driver_sync3)

    check_recognition(counter)


def navigation_settings_page(counter, driver):
    driver.settings_page.open_setting_navigation_page()
    helpers.get_result_recognition(constants.NAVIGATION_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(constants.MAP_PREFERENCES_TEXT, counter)
    helpers.get_result_recognition(constants.ROUTE_PREFERENCES_TEXT, counter)
    helpers.get_result_recognition(constants.NAVIGATION_PREFERENCES_TEXT, counter)
    driver.settings_navigation_page.open_map_preferences_page()
    helpers.get_result_recognition(constants.CITY_MODEL, counter)
    helpers.get_result_recognition(constants.BREADCRUMBS, counter)
    driver.settings_radio_page.tap_on_back_button()
    driver.settings_navigation_page.open_route_preferences_page()
    helpers.get_result_recognition(constants.ROUTE_PREFERENCES_TEXT, counter)
    helpers.get_result_recognition(constants.PREFERRED_ROUTE_TEXT, counter)
    helpers.get_result_recognition(constants.ALWAYS_USE_FASTEST_ROUTE_TEXT, counter)
    helpers.get_result_recognition(constants.FIND_PARKING_AUTOMATICALLY_TEXT, counter)
    helpers.get_result_recognition(constants.ECO_TIME_PENALTY_TEXT, counter)
    driver.settings_navigation_page.tap_on_down_arrow()
    helpers.get_result_recognition(constants.DYNAMIC_ROUTE_GUIDANCE_TEXT, counter)
    helpers.get_result_recognition(constants.UNCHECKED_SETTINGS_NAVIGATION_CHECKBOX_BUTTON, counter)
    helpers.get_result_recognition(constants.CHECKED_SETTINGS_NAVIGATION_CHECKBOX_BUTTON, counter)
    helpers.get_result_recognition(constants.AVOID_MOTORWAYS_TEXT, counter)
    helpers.get_result_recognition(constants.AVOID_TOLL_ROADS_TEXT, counter)
    helpers.get_result_recognition(constants.AVOID_FERRIES_CAR_TRAINS_TEXT, counter)
    driver.settings_radio_page.tap_on_back_button()
    driver.settings_navigation_page.open_navigation_preferences_page()
    helpers.get_result_recognition(constants.NAVIGATION_PREFERENCES_TEXT, counter)
    helpers.get_result_recognition(constants.GUIDANCE_PROMPTS_TEXT, counter)
    driver.settings_navigation_page.open_guidance_prompts_page()
    helpers.get_result_recognition(constants.VOICE_CHIMES_TEXT, counter)
    helpers.get_result_recognition(constants.VOICE_ONLY_TEXT, counter)
    helpers.get_result_recognition(constants.CHIMES_ONLY_TEXT, counter)
    helpers.get_result_recognition(constants.VOICE_ICON_ACTIVE, counter)
    helpers.get_result_recognition(constants.VOICE_ICON, counter)
    helpers.get_result_recognition(constants.PLUS_ICON, counter)
    helpers.get_result_recognition(constants.BACK_PROPERTY, counter)
    driver.settings_navigation_page.tap_on_back_property_button_on_guidance_prompts_page()
    driver.settings_navigation_page.tap_on_back_button()


def radio_settings_page(counter, driver):
    # Go to Radio settings page apps page
    driver.settings_page.open_settings_radio_page()
    helpers.get_result_recognition(constants.RADIO_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(constants.RADIO_TEXT, counter)
    helpers.get_result_recognition(constants.RADIO_AUTOSTORE_PRESETS_TEXT, counter)
    helpers.get_result_recognition(constants.RADIO_REFRESH_TEXT, counter)
    driver.settings_radio_page.tap_on_back_button()


def bluetooth_settings_page(counter, driver):
    # Go to Bluetooth settings page apps page
    driver.settings_page.open_settings_bluetooth_page()
    helpers.get_result_recognition(constants.BLUETOOTH_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(constants.BLUETOOTH_SWITCH_ON_BUTTON, counter)
    helpers.get_result_recognition(constants.ADD_BLUETOOTH_DEVICE_TEXT, counter)
    helpers.get_result_recognition(constants.BLUETOOTH_VIEW_DEVICES_TEXT, counter)
    driver.settings_bluetooth_page.tap_on_back_button()


def clock_settings_page(counter, driver):
    # Go to Clock Settings page apps page
    driver.settings_page.open_setting_clock_page()
    helpers.get_result_recognition(constants.CLOCK_SETTINGS_TITLE_TEXT, counter)
    if not driver.settings_clock_page.clock_12h_button_is_active():
        driver.settings_clock_page.tap_on_24h_button()
    helpers.get_result_recognition(constants.CLOCK_SETTINGS_12H, counter)
    helpers.get_result_recognition(constants.CLOCK_PM_PERIOD_TEXT, counter)
    helpers.get_result_recognition(constants.CLOCK_SWITCH_OFF, counter)
    helpers.get_result_recognition(constants.CLOCK_FORMAT_TEXT, counter)
    helpers.get_result_recognition(constants.CLOCK_AUTO_TIME_ZONE_TEXT, counter)
    helpers.get_result_recognition(constants.CLOCK_RESET_GPS_TEXT, counter)
    driver.settings_clock_page.tap_on_back_button()


def sound_settings_page(counter, driver):
    # Go to Sound Settings page apps page
    driver.settings_page.open_setting_audio_page()
    helpers.get_result_recognition(constants.SOUND_SETTINGS_TITLE_TEXT, counter)
    helpers.get_result_recognition(constants.SOUND_RESET_ALL_BUTTON, counter)
    helpers.get_result_recognition(constants.SOUND_TREBLE_TEXT, counter)
    helpers.get_result_recognition(constants.BACK_BUTTON, counter)
    helpers.get_result_recognition(constants.SLIDER, counter)
    helpers.get_result_recognition(constants.UP_SCROLL_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(constants.DOWN_SCROLL_BUTTON_ACTIVE, counter)
    helpers.get_result_recognition(constants.BALANCE_FADE_BUTTON, counter)
    driver.settings_audio_page.tap_on_down_scroll_button()
    helpers.get_result_recognition(constants.UP_SCROLL_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(constants.DOWN_SCROLL_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(constants.INFORMATION_DIALOG_BUTTON, counter)
    helpers.get_result_recognition(constants.ADAPTIVE_VOLUME_MEDIUM_OPTION, counter)
    helpers.get_result_recognition(constants.ADAPTIVE_VOLUME_TEXT, counter)
    helpers.get_result_recognition(constants.DSP_OCCUPANCY_TEXT, counter)
    helpers.get_result_recognition(constants.ALL_SEATS_TEXT, counter)
    driver.settings_audio_page.tap_on_back_button()


def main_settings_page(counter, driver):
    # Go to Main Settings page apps page
    driver.settings_page.open_settings_page()
    helpers.get_result_recognition(constants.SETTINGS_SOUND_BUTTON, counter)
    helpers.get_result_recognition(constants.SETTINGS_CLOCK_BUTTON, counter)
    helpers.get_result_recognition(constants.MOBILE_BLUETOOTH_BUTTON, counter)
    helpers.get_result_recognition(constants.SETTINGS_RADIO_BUTTON, counter)
    helpers.get_result_recognition(constants.SETTINGS_NAVIGATION_BUTTON, counter)
    helpers.get_result_recognition(constants.MOBILE_APPS_BUTTON, counter)
    helpers.get_result_recognition(constants.SYNC_CONNECT_BUTTON, counter)
    helpers.get_result_recognition(constants.ACTIVE_SETTINGS_PAGE_BUTTON, counter)


def mobile_apps_page(counter, driver):
    # Go to Mobile apps page
    driver.mobile_apps_page.open_mobile_apps_page()
    helpers.get_result_recognition(constants.MOBILE_BLUETOOTH_BUTTON, counter)
    helpers.get_result_recognition(constants.FIND_MOBILE_APPS_BUTTON, counter)
    helpers.get_result_recognition(constants.SIRIUSXM_BUTTON, counter)
    helpers.get_result_recognition(constants.ACTIVE_MOBILE_APPS_PAGE_BUTTON, counter)
    helpers.get_result_recognition(constants.CONNECT_A_DEVICE_TEXT, counter)
    helpers.get_result_recognition(constants.FIND_MOBILE_APPS_TEXT, counter)
    helpers.get_result_recognition(constants.SIRIUS_XM_TRAVEL_LINK_TEXT, counter)


def phone_page(counter, driver):
    # Go to Phone page
    driver.phone_page.open_phone_page()
    helpers.get_result_recognition(constants.PAIR_PHONE_TEXT, counter)
    helpers.get_result_recognition(constants.PHONE_CONNECT_TO, counter)
    helpers.get_result_recognition(constants.ACTIVE_PHONE_PAGE_BUTTON, counter)


def climate_page(counter, driver):
    # Go to Climate page
    driver.climat_page.open_climate_page()
    helpers.get_result_recognition(constants.AUTO_TEXT, counter)
    helpers.get_result_recognition(constants.CLIMATE_ON_OFF_BUTTON, counter)
    helpers.get_result_recognition(constants.DUAL_TEXT, counter)
    helpers.get_result_recognition(constants.CLIMATE_ARROW_UP, counter)
    helpers.get_result_recognition(constants.CLIMATE_ARROW_DOWN, counter)
    helpers.get_result_recognition(constants.DEFROST, counter)
    helpers.get_result_recognition(constants.TOP_AIRFLOW_ACTIVE_BUTTON, counter)
    helpers.get_result_recognition(constants.DOWN_AIRFLOW_ACTIVE_BUTTON, counter)
    helpers.get_result_recognition(constants.A_C_ACTIVE, counter)
    # Go to Defrost controls panel
    driver.climat_page.tap_on_climate_defrost_button()
    helpers.get_result_recognition(constants.DEFROST_CONTROLS_TEXT, counter)
    helpers.get_result_recognition(constants.MAX_FRONT_HEATING_INACTIVE, counter)
    helpers.get_result_recognition(constants.FRONT_HEATING, counter)
    helpers.get_result_recognition(constants.REAR_HEATING, counter)
    driver.climat_page.close_information_dialog()
    helpers.get_result_recognition(constants.ACTIVE_CLIMATE_PAGE_ICON, counter)


def audio_page(counter, driver):
    # Go to Audio page
    driver.audio_page.open_audio_page()
    helpers.get_result_recognition(constants.SOURCES_TEXT, counter)
    helpers.get_result_recognition(constants.AUDIO_FREQUENCY_BUTTON, counter)
    helpers.get_result_recognition(constants.PRESETS_BUTTON, counter)
    helpers.get_result_recognition(constants.ACTIVE_AUDIO_BUTTON, counter)


def home_page(counter):
    helpers.get_result_recognition(constants.HOME_PAGE_ICON, counter)
    helpers.get_result_recognition(constants.INACTIVE_AUDIO_PAGE_BUTTON, counter)
    helpers.get_result_recognition(constants.INACTIVE_CLIMATE_PAGE_ICON, counter)
    helpers.get_result_recognition(constants.INACTIVE_PHONE_PAGE_BUTTON, counter)
    helpers.get_result_recognition(constants.INACTIVE_NAVIGATION_PAGE_BUTTON, counter)
    helpers.get_result_recognition(constants.INACTIVE_MOBILE_APPS_PAGE_BUTTON, counter)
    helpers.get_result_recognition(constants.INACTIVE_SETTINGS_PAGE_BUTTON, counter)
    helpers.get_result_recognition(constants.EXCLAMATION_ICON, counter)
    helpers.get_result_recognition(constants.NAVIGATION_FAULT_TEXT, counter)


def check_recognition(counter):
    logging.info("Recognized {} images".format(counter['success']))
    assert (counter['success']) > PERCENT, "Image recognition < {}%".format(PERCENT)
