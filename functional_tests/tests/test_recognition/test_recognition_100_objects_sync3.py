import logging

import allure
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import TASK_LINK, Icons, Text

PERCENT = 98


@allure.testcase(TASK_LINK.format("VHAT-284"), "VHAT-284")
def test_recognition_100_different_objects_1_time(driver_sync3):
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
    helpers.get_result_recognition(Text.NAVIGATION_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(Text.MAP_PREFERENCES_TEXT, counter)
    helpers.get_result_recognition(Text.ROUTE_PREFERENCES_TEXT, counter)
    helpers.get_result_recognition(Text.NAVIGATION_PREFERENCES_TEXT, counter)
    driver.settings_navigation_page.open_map_preferences_page()
    helpers.get_result_recognition(Text.CITY_MODEL_TEXT, counter)
    helpers.get_result_recognition(Text.BREADCRUMBS, counter)
    driver.settings_radio_page.tap_on_back_button()
    driver.settings_navigation_page.open_route_preferences_page()
    helpers.get_result_recognition(Text.ROUTE_PREFERENCES_TEXT, counter)
    helpers.get_result_recognition(Text.PREFERRED_ROUTE_TEXT, counter)
    helpers.get_result_recognition(Text.ALWAYS_USE_FASTEST_ROUTE_TEXT, counter)
    helpers.get_result_recognition(Text.FIND_PARKING_AUTOMATICALLY_TEXT, counter)
    helpers.get_result_recognition(Text.ECO_TIME_PENALTY_TEXT, counter)
    driver.settings_navigation_page.tap_on_down_arrow()
    helpers.get_result_recognition(Text.DYNAMIC_ROUTE_GUIDANCE_TEXT, counter)
    helpers.get_result_recognition(Icons.UNCHECKED_SETTINGS_NAVIGATION_CHECKBOX_BUTTON, counter)
    helpers.get_result_recognition(Icons.CHECKED_SETTINGS_NAVIGATION_CHECKBOX_BUTTON, counter)
    helpers.get_result_recognition(Text.AVOID_MOTORWAYS_TEXT, counter)
    helpers.get_result_recognition(Text.AVOID_TOLL_ROADS_TEXT, counter)
    helpers.get_result_recognition(Text.AVOID_FERRIES_CAR_TRAINS_TEXT, counter)
    driver.settings_radio_page.tap_on_back_button()
    driver.settings_navigation_page.open_navigation_preferences_page()
    helpers.get_result_recognition(Text.NAVIGATION_PREFERENCES_TEXT, counter)
    helpers.get_result_recognition(Text.GUIDANCE_PROMPTS_TEXT, counter)
    driver.settings_navigation_page.open_guidance_prompts_page()
    helpers.get_result_recognition(Text.VOICE_CHIMES_TEXT, counter)
    helpers.get_result_recognition(Text.VOICE_ONLY_TEXT, counter)
    helpers.get_result_recognition(Text.CHIMES_ONLY_TEXT, counter)
    helpers.get_result_recognition(Icons.VOICE_ICON_ACTIVE, counter)
    helpers.get_result_recognition(Icons.VOICE_ICON, counter)
    helpers.get_result_recognition(Icons.PLUS_ICON, counter)
    helpers.get_result_recognition(Icons.BACK_PROPERTY, counter)
    driver.settings_navigation_page.tap_on_back_property_button_on_guidance_prompts_page()
    driver.settings_navigation_page.tap_on_back_button()


def radio_settings_page(counter, driver):
    # Go to Radio settings page apps page
    driver.settings_page.open_settings_radio_page()
    helpers.get_result_recognition(Text.RADIO_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(Text.RADIO_TEXT, counter)
    helpers.get_result_recognition(Text.RADIO_AUTOSTORE_PRESETS_TEXT, counter)
    helpers.get_result_recognition(Text.RADIO_REFRESH_TEXT, counter)
    driver.settings_radio_page.tap_on_back_button()


def bluetooth_settings_page(counter, driver):
    # Go to Bluetooth settings page apps page
    driver.settings_page.open_settings_bluetooth_page()
    helpers.get_result_recognition(Text.BLUETOOTH_SETTINGS_TEXT, counter)
    helpers.get_result_recognition(Icons.BLUETOOTH_SWITCH_ON_BUTTON, counter)
    helpers.get_result_recognition(Text.ADD_BLUETOOTH_DEVICE_TEXT, counter)
    helpers.get_result_recognition(Text.BLUETOOTH_VIEW_DEVICES_TEXT, counter)
    driver.settings_bluetooth_page.tap_on_back_button()


def clock_settings_page(counter, driver):
    # Go to Clock Settings page apps page
    driver.settings_page.open_setting_clock_page()
    helpers.get_result_recognition(Text.CLOCK_SETTINGS_TITLE_TEXT, counter)
    if not driver.settings_clock_page.clock_12h_button_is_active():
        driver.settings_clock_page.tap_on_24h_button()
    helpers.get_result_recognition(Icons.CLOCK_SETTINGS_12H, counter)
    helpers.get_result_recognition(Text.CLOCK_PM_PERIOD_TEXT, counter)
    helpers.get_result_recognition(Icons.CLOCK_SWITCH_OFF, counter)
    helpers.get_result_recognition(Text.CLOCK_FORMAT_TEXT, counter)
    helpers.get_result_recognition(Text.CLOCK_AUTO_TIME_ZONE_TEXT, counter)
    helpers.get_result_recognition(Text.CLOCK_RESET_GPS_TEXT, counter)
    driver.settings_clock_page.tap_on_back_button()


def sound_settings_page(counter, driver):
    # Go to Sound Settings page apps page
    driver.settings_page.open_setting_audio_page()
    helpers.get_result_recognition(Text.SOUND_SETTINGS_TITLE_TEXT, counter)
    helpers.get_result_recognition(Icons.SOUND_RESET_ALL_BUTTON, counter)
    helpers.get_result_recognition(Text.SOUND_TREBLE_TEXT, counter)
    helpers.get_result_recognition(Icons.BACK_BUTTON, counter)
    helpers.get_result_recognition(Icons.SLIDER, counter)
    helpers.get_result_recognition(Icons.UP_SCROLL_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Icons.DOWN_SCROLL_BUTTON_ACTIVE, counter)
    helpers.get_result_recognition(Icons.BALANCE_FADE_BUTTON, counter)
    driver.settings_audio_page.tap_on_down_scroll_button()
    helpers.get_result_recognition(Icons.UP_SCROLL_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Icons.DOWN_SCROLL_BUTTON_INACTIVE, counter)
    helpers.get_result_recognition(Icons.INFORMATION_DIALOG_BUTTON, counter)
    helpers.get_result_recognition(Icons.ADAPTIVE_VOLUME_MEDIUM_OPTION, counter)
    helpers.get_result_recognition(Text.ADAPTIVE_VOLUME_TEXT, counter)
    helpers.get_result_recognition(Text.DSP_OCCUPANCY_TEXT, counter)
    helpers.get_result_recognition(Text.ALL_SEATS_TEXT, counter)
    driver.settings_audio_page.tap_on_back_button()


def main_settings_page(counter, driver):
    # Go to Main Settings page apps page
    driver.settings_page.open_settings_page()
    helpers.get_result_recognition(Icons.SETTINGS_SOUND_BUTTON, counter)
    helpers.get_result_recognition(Icons.SETTINGS_CLOCK_BUTTON, counter)
    helpers.get_result_recognition(Icons.MOBILE_BLUETOOTH_BUTTON, counter)
    helpers.get_result_recognition(Icons.SETTINGS_RADIO_BUTTON, counter)
    helpers.get_result_recognition(Icons.SETTINGS_NAVIGATION_BUTTON, counter)
    helpers.get_result_recognition(Icons.MOBILE_APPS_BUTTON, counter)
    helpers.get_result_recognition(Icons.SYNC_CONNECT_BUTTON, counter)
    helpers.get_result_recognition(Icons.ACTIVE_SETTINGS_PAGE_BUTTON, counter)


def mobile_apps_page(counter, driver):
    # Go to Mobile apps page
    driver.mobile_apps_page.open_mobile_apps_page()
    helpers.get_result_recognition(Icons.MOBILE_BLUETOOTH_BUTTON, counter)
    helpers.get_result_recognition(Icons.FIND_MOBILE_APPS_BUTTON, counter)
    helpers.get_result_recognition(Icons.SIRIUSXM_BUTTON, counter)
    helpers.get_result_recognition(Icons.ACTIVE_MOBILE_APPS_PAGE_BUTTON, counter)
    helpers.get_result_recognition(Text.CONNECT_A_DEVICE_TEXT, counter)
    helpers.get_result_recognition(Text.FIND_MOBILE_APPS_TEXT, counter)
    helpers.get_result_recognition(Text.SIRIUS_XM_TRAVEL_LINK_TEXT, counter)


def phone_page(counter, driver):
    # Go to Phone page
    driver.phone_page.open_phone_page()
    helpers.get_result_recognition(Text.PAIR_PHONE_TEXT, counter)
    helpers.get_result_recognition(Icons.PHONE_CONNECT_TO, counter)
    helpers.get_result_recognition(Icons.ACTIVE_PHONE_PAGE_BUTTON, counter)


def climate_page(counter, driver):
    # Go to Climate page
    driver.climate_page.open_climate_page()
    helpers.get_result_recognition(Text.AUTO_TEXT, counter)
    helpers.get_result_recognition(Icons.CLIMATE_ON_OFF_BUTTON, counter)
    helpers.get_result_recognition(Text.DUAL_TEXT, counter)
    helpers.get_result_recognition(Icons.CLIMATE_ARROW_UP, counter)
    helpers.get_result_recognition(Icons.CLIMATE_ARROW_DOWN, counter)
    helpers.get_result_recognition(Icons.DEFROST, counter)
    helpers.get_result_recognition(Icons.TOP_AIRFLOW_ACTIVE_BUTTON, counter)
    helpers.get_result_recognition(Icons.DOWN_AIRFLOW_ACTIVE_BUTTON, counter)
    helpers.get_result_recognition(Icons.A_C_ACTIVE, counter)
    # Go to Defrost controls panel
    driver.climate_page.tap_on_climate_defrost_button()
    helpers.get_result_recognition(Text.DEFROST_CONTROLS_TEXT, counter)
    helpers.get_result_recognition(Icons.MAX_FRONT_HEATING_INACTIVE, counter)
    helpers.get_result_recognition(Icons.FRONT_HEATING, counter)
    helpers.get_result_recognition(Icons.REAR_HEATING, counter)
    driver.climate_page.close_information_dialog()
    helpers.get_result_recognition(Icons.ACTIVE_CLIMATE_PAGE_ICON, counter)


def audio_page(counter, driver):
    # Go to Audio page
    driver.audio_page.open_audio_page()
    helpers.get_result_recognition(Text.SOURCES_TEXT, counter)
    helpers.get_result_recognition(Icons.AUDIO_FREQUENCY_BUTTON, counter)
    helpers.get_result_recognition(Icons.PRESETS_BUTTON, counter)
    helpers.get_result_recognition(Icons.ACTIVE_AUDIO_BUTTON, counter)


def home_page(counter):
    helpers.get_result_recognition(Icons.HOME_PAGE_ICON, counter)
    helpers.get_result_recognition(Icons.INACTIVE_AUDIO_PAGE_BUTTON, counter)
    helpers.get_result_recognition(Icons.INACTIVE_CLIMATE_PAGE_ICON, counter)
    helpers.get_result_recognition(Icons.INACTIVE_PHONE_PAGE_BUTTON, counter)
    helpers.get_result_recognition(Icons.INACTIVE_NAVIGATION_PAGE_BUTTON, counter)
    helpers.get_result_recognition(Icons.INACTIVE_MOBILE_APPS_PAGE_BUTTON, counter)
    helpers.get_result_recognition(Icons.INACTIVE_SETTINGS_PAGE_BUTTON, counter)
    helpers.get_result_recognition(Icons.EXCLAMATION_ICON, counter)
    helpers.get_result_recognition(Text.NAVIGATION_FAULT_TEXT, counter)


def check_recognition(counter):
    logging.info("Recognized {} images".format(counter['success']))
    assert (counter['success']) > PERCENT, "Image recognition < {}%".format(PERCENT)
