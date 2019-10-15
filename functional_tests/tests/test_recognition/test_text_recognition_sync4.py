import allure
import pytest
from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.sync4.constants import TASK_LINK, Text


# Main panel
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.MAIN_PANEL_PHONE_TEXT, Text.MAIN_PANEL_FAVORITES_TEXT,
                                  Text.MAIN_PANEL_APPS_TEXT,
                                  Text.MAIN_PANEL_SETTINGS_TEXT,
                                  Text.MAIN_PANEL_FEATURES_TEXT,
                                  Text.MAIN_PANEL_AUDIO_TEXT,
                                  Text.MAIN_PANEL_CLIMATE_TEXT, ])
def test_main_panel_text(driver_sync4, name):
    assert get_exist_result(name)


# Phone page
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.CONNECT_PHONE_TITLE_TEXT, Text.ADD_PHONE_TEXT])
def test_phone_page_text(driver_sync4, name):
    assert get_exist_result(name)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.PAIR_BLUETOOTH_DEVICE_TITLE_TEXT, Text.ADD_PHONE_PAGE_DISCOVER_OTHER_BUTTON_TEXT,
                                  Text.HELP_TEXT])
def test_pair_bluetooth_device_text(driver_sync4, name):
    driver_sync4.phone_page.open_add_phone_page()
    assert get_exist_result(name)


# Settings page
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.SOUND_SETTINGS_BUTTON_TEXT, Text.CLOCK_SETTINGS_TEXT,
                                  Text.CONNECTION_SETTINGS_BUTTON_TEXT, Text.ADD_PHONE_TEXT,
                                  Text.MOBILE_APPS_SETTINGS_BUTTON_TEXT, Text.GENERAL_SETTINGS_BUTTON_TEXT,
                                  Text.ASSIST_911_SETTINGS_BUTTON_TEXT])
def test_settings_page_buttons_text(settings_sync4, name):
    assert get_exist_result(name)


# Sound settings
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.TONE_SETTINGS_TEXT, Text.SOUND_SETTINGS_BALANCE_FADE_TEXT,
                                  Text.SOUND_SETTINGS_SPEED_COMPENSATED_TEXT, Text.SOUND_SETTINGS_OCCUPANCY_MODE_TEXT])
def test_sound_settings_page_text(sound_settings_sync4, name):
    assert get_exist_result(Text.TONE_SETTINGS_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_tone_settings_page_treble_text(sound_settings_sync4):
    sound_settings_sync4.open_tone_settings_page()
    assert get_exist_result(Text.SOUND_SETTINGS_TREBLE_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_tone_settings_page_midrange_text(sound_settings_sync4):
    sound_settings_sync4.open_tone_settings_page()
    assert get_exist_result(Text.SOUND_SETTINGS_MIDRANGE_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_tone_settings_page_bass_text(sound_settings_sync4):
    sound_settings_sync4.open_tone_settings_page()
    assert get_exist_result(Text.SOUND_SETTINGS_BASS_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_balance_fade_settings_page_reset_text(sound_settings_sync4):
    sound_settings_sync4.open_balance_fade_page()
    assert get_exist_result(Text.RESET_BALANCE_FADE_TEXT)
    sound_settings_sync4.tap_on_back_button()


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_speed_comp_volume_settings_page_off_text(sound_settings_sync4):
    sound_settings_sync4.open_speed_compensated_volume()
    assert get_exist_result(Text.SPEED_COMPENSATED_VOLUME_PAGE_OFF_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_speed_comp_volume_settings_page_low_text(sound_settings_sync4):
    sound_settings_sync4.open_speed_compensated_volume()
    assert get_exist_result(Text.SPEED_COMPENSATED_VOLUME_PAGE_LOW_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_speed_comp_volume_settings_page_medium_text(sound_settings_sync4):
    sound_settings_sync4.open_speed_compensated_volume()
    assert get_exist_result(Text.SPEED_COMPENSATED_VOLUME_PAGE_MEDIUM_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_speed_comp_volume_settings_page_high_text(sound_settings_sync4):
    sound_settings_sync4.open_speed_compensated_volume()
    assert get_exist_result(Text.SPEED_COMPENSATED_VOLUME_PAGE_HIGH_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.SOUND_SETTINGS_DRIVER_TEXT, Text.SOUND_SETTINGS_ALL_SEATS_TEXT,
                                  Text.SOUND_SETTINGS_FRONT_TEXT, Text.SOUND_SETTINGS_REAR_TEXT])
def test_settings_occupancy_mode_page_text(occupancy_mode_sync4, name):
    assert get_exist_result(name)


# Clock settings
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_clock_settings_page_24h_text(clock_settings_sync4):
    if not clock_settings_sync4.clock_24h_button_is_active():
        clock_settings_sync4.tap_on_12h_button()
    assert get_exist_result(Text.HOUR_MODE_24_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.AM_TEXT, Text.PM_TEXT,
                                  Text.HOUR_MODE_12_TEXT])
def test_clock_settings_page_12h_text(clock_settings_sync4, name):
    if not clock_settings_sync4.clock_12h_button_is_active():
        clock_settings_sync4.tap_on_24h_button()
    assert get_exist_result(name)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
def test_clock_settings_page_reset_clock_text(clock_settings_sync4):
    assert get_exist_result(Text.RESET_CLOCK_GPS_TEXT)


# Connection settings
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.SYSTEM_WIFI_TEXT,
                                  Text.BLUETOOTH_TEXT])
def test_connection_settings_page_text(connection_settings_sync4, name):
    assert get_exist_result(name)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.BLUETOOTH_SETTINGS_CHANGE_VEHICLE_NAME_TEXT,
                                  Text.BLUETOOTH_SETTINGS_ADD_BLUETOOTH_DEVICE_TEXT,
                                  Text.BLUETOOTH_TEXT])
def test_bluetooth_settings_page_text(bluetooth_settings_sync4, name):
    assert get_exist_result(name)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.skip('main panel is not visible, looking for workaround')
@pytest.mark.parametrize("n",
                         ['space', '?123', '.', '1', '2', '3', '4', '5', '6', '7', '8',
                          '9', '0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
                          'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C',
                          'V', 'B', 'N', 'M'])
def test_change_vehicle_name_page_keyboard_keys(change_vehicle_name_page_sync4, n):
    assert get_exist_result(n)
    change_vehicle_name_page_sync4.tap_on_back_button()


# General settings
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("n", [Text.LANGUAGE_TEXT, Text.GENERAL_SETTINGS_DISTANCE_TEXT,
                               Text.GENERAL_SETTINGS_TEMPERATURE_UNITS_TEXT, Text.TOUCH_PANEL_BEEP_TEXT,
                               Text.TOUCH_SCREEN_BEEP_TEXT])
def test_general_settings_page_text(general_settings_sync4, n):
    assert get_exist_result(n)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("n", [Text.LANGUAGE_PAGE_ENGLISH_US_TEXT])
def test_language_page_text(general_settings_sync4, n):
    general_settings_sync4.open_language_page()
    assert get_exist_result('{}'.format(n))


# Mobile apps
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.MOBILE_APPS_SETTINGS_ENABLE_MOBILE_APPS_TEXT,
                                  Text.MOBILE_APPS_SETTINGS_ENABLE_MOBILE_APPS_VIA_USB_TEXT])
def test_mobile_apps_page_text(settings_sync4, name):
    settings_sync4.open_mobile_apps_settings()
    assert get_exist_result(name)


# Radio settings
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.RADIO_SETTINGS_RADIO_TEXT,
                                  Text.RADIO_SETTINGS_REFRESH_TEXT, Text.RADIO_SETTINGS_AUTOSET_TEXT])
def test_radio_settings_page_text(settings_sync4, name):
    settings_sync4.open_radio_settings()
    assert get_exist_result(name)


# Climate page
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.CLIMATE_PAGE_CONTROLS_TITLE_TEXT,
                                  Text.CLIMATE_PAGE_AUTO_BUTTON_TEXT, Text.CLIMATE_PAGE_DUAL_BUTTON_TEXT,
                                  Text.CLIMATE_PAGE_MENU_BUTTON_TEXT, Text.CLIMATE_PAGE_AC_BUTTON_TEXT])
def test_climate_page_text(driver_sync4, name):
    driver_sync4.climate_page.open_climate_page()
    assert get_exist_result(name)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.CLIMATE_PAGE_MAX_AC_TEXT,
                                  Text.CLIMATE_PAGE_MAX_GLASS_HEATING_TEXT, Text.CLOSE_TEXT])
def test_climate_menu_page_text(driver_sync4, name):
    driver_sync4.climate_page.open_climate_menu()
    assert get_exist_result(name)
    driver_sync4.climate_page.tap_on_close_button()


# Audio page
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.AUDIO_PAGE_SOURCES_TEXT,
                                  Text.AUDIO_PAGE_DIRECT_TUNE_TEXT, Text.AUDIO_PAGE_TITLE_TEXT])
def test_audio_page_text(audio_sync4, name):
    assert get_exist_result(name)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.AM_TEXT,
                                  Text.FM_TEXT, Text.BLUETOOTH_STEREO_TEXT, Text.ENTERTAINMENT_SOURCES_TITLE_TEXT])
def test_audio_sources_page_text(audio_sync4, name):
    audio_sync4.tap_on_sources_page()
    assert get_exist_result(name)


@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.skip('Text cancel is not recognized')
@pytest.mark.parametrize("name", ['1', '2', '3', '4', '5', '6', '7', '8', '9', '0', Text.CANCEL_TEXT, Text.ENTER_TEXT])
def test_direct_tune_page_text(direct_tune_sync4, name):
    assert get_exist_result(name)
    direct_tune_sync4.tap_on_cancel_button()


# Apps page
@allure.testcase(TASK_LINK.format("VHAT-545"), "VHAT-545")
@pytest.mark.text_recognition
@pytest.mark.parametrize("name", [Text.APPS_PAGE_ADD_DEVICE_TEXT, Text.APPS_PAGE_FIND_APPS_TEXT,
                                  Text.APPS_PAGE_MOBILE_APPS_HELP_TEXT])
def test_apps_page_text(apps_sync4, name):
    assert get_exist_result(name)
