import pytest

from functional_tests.pages.menlo.constants import Text, Icons
from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.report import jira_test, jira_issue

pytestmark = [pytest.mark.recognition_menlo,
              pytest.mark.regression_menlo]


class TestSettingsFirstPart(object):
    @jira_test("VHAT-2068")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.SETTINGS_SOUND_BUTTON, Icons.SETTINGS_CLOCK_BUTTON,
        Icons.SETTINGS_PHONE_LIST_BUTTON, Icons.SETTINGS_CONNECTIVITY_BUTTON,
        Icons.SETTINGS_RADIO_BUTTON, Icons.SETTINGS_VEHICLE_BUTTON,
        Icons.GENERAL_DRAWER_HANDLE
    ])
    def test_settings_first_part_images(self, settings_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2069")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_TAB_TITLE_TEXT, Text.SETTINGS_SOUND_BUTTON_TEXT,
        Text.SETTINGS_CLOCK_BUTTON_TEXT, Text.SETTINGS_PHONE_LIST_BUTTON_TEXT,
        Text.SETTINGS_CONNECTIVITY_BUTTON_TEXT, Text.SETTINGS_RADIO_BUTTON_TEXT,
        Text.SETTINGS_VEHICLE_BUTTON_TEXT
    ])
    def test_settings_first_part_text(self, settings_menlo, text):
        assert get_exist_result(text)


class TestSettingsSound(object):
    @jira_test("VHAT-2070")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_INFO
    ])
    def test_settings_sound_images(self, settings_sound_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2071")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_SOUND_TITLE_TEXT, Text.SETTINGS_SOUND_TONE_TEXT,
        Text.SETTINGS_SOUND_BALANCE_FADE_TEXT, Text.SETTINGS_SOUND_DSP_OCCUPANCY_TEXT

    ])
    def test_settings_sound_text(self, settings_sound_menlo, text):
        assert get_exist_result(text)


class TestSettingsClock(object):
    @jira_issue("VHAT-2012", test_data=[Icons.GENERAL_ARROW_DOWN_DISABLED])
    @jira_test("VHAT-2072")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_UP, Icons.GENERAL_ARROW_DOWN_DISABLED,
        Icons.GENERAL_INFO, Icons.GENERAL_TOGLE_INACTIVE,
        Icons.SETTINGS_CLOCK_AM_BUTTON_ACTIVE, Icons.SETTINGS_CLOCK_PM_BUTTON_INACTIVE
    ])
    def test_settings_clock_images(self, settings_clock_menlo, icon):
        assert get_exist_result(icon)

    @jira_issue("VHAT-2012", test_data=[Text.SETTINGS_CLOCK_TIME_ZONE_TEXT])
    @jira_test("VHAT-2073")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_CLOCK_TITLE_TEXT,
        Text.SETTINGS_CLOCK_MODE_TEXT,
        Text.SETTINGS_CLOCK_TIME_ZONE_TEXT

    ])
    def test_settings_clock_text(self, settings_clock_menlo, text):
        assert get_exist_result(text)


class TestSettingsPhoneList(object):
    @jira_test("VHAT-2074")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.SETTINGS_PHONE_LIST_ADD_PHONE_BUTTON,
        Icons.GENERAL_INFO
    ])
    def test_settings_phone_list_images(self, settings_phone_list_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2075")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_PHONE_LIST_TITLE_TEXT,
        Text.SETTINGS_PHONE_LIST_ADD_PHONE_TEXT
    ])
    def test_settings_phone_list_text(self, settings_phone_list_menlo, text):
        assert get_exist_result(text)


class TestSettingsConnectivity(object):
    @jira_test("VHAT-2076")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_INFO
    ])
    def test_settings_connectivity_images(self, settings_connectivity_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2077")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_CONNECTIVITY_TITLE_TEXT, Text.SETTINGS_CONNECTIVITY_FEATURES_TEXT,
        Text.SETTINGS_CONNECTIVITY_BLUETOOTH_TEXT, Text.SETTINGS_CONNECTIVITY_PROJECTION_TEXT,
        Text.SETTINGS_CONNECTIVITY_NETWORKS_TEXT
    ])
    def test_settings_connectivity_text(self, settings_connectivity_menlo, text):
        assert get_exist_result(text)


class TestSettingsRadio(object):
    @jira_test("VHAT-2159")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_TOGLE_INACTIVE,
        Icons.GENERAL_INFO
    ])
    def test_settings_radio_images(self, settings_radio_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2160")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_RADIO_TITLE_TEXT, Text.SETTINGS_RADIO_PRESET_ROWS_TEXT,
        Text.SETTINGS_RADIO_AM_HD_RADIO_TEXT
    ])
    def test_settings_radio_text(self, settings_radio_menlo, text):
        assert get_exist_result(text)


class TestSettingsDriverAssistance(object):
    @jira_test("VHAT-2161")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_TOGLE_INACTIVE,
        Icons.GENERAL_TOGLE_ACTIVE,
        Icons.GENERAL_INFO
    ])
    def test_settings_driver_assistance_images(self, settings_driver_assistance_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2162")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_DRIVER_ASSISTANCE_TITLE_TEXT, Text.SETTINGS_DRIVER_ASSISTANCE_CRUISE_CONTROL_TEXT,
        Text.SETTINGS_DRIVER_ASSISTANCE_SPEED_LIMIT_ASSIST_TEXT,
        Text.SETTINGS_DRIVER_ASSISTANCE_LANE_KEEPING_SYSTEMS_TEXT,
        Text.SETTINGS_DRIVER_ASSISTANCE_PRE_COLLISION_ASSIST_TEXT,
        Text.SETTINGS_DRIVER_ASSISTANCE_REAR_VIEW_CAMERA_DELAY_TEXT,
        Text.SETTINGS_DRIVER_ASSISTANCE_GRADE_ASSIST_TEXT, Text.SETTINGS_DRIVER_ASSISTANCE_HILL_DESCENT_CONTROL_TEXT,
        Text.SETTINGS_DRIVER_ASSISTANCE_WRONG_WAY_ALERT_TEXT, Text.SETTINGS_DRIVER_ASSISTANCE_WRONG_WAY_ALERT_TEXT,
        Text.SETTINGS_DRIVER_ASSISTANCE_DRIVER_ALERT_TEXT, Text.SETTINGS_DRIVER_ASSISTANCE_HILL_START_ASSIST_TEXT,
        Text.SETTINGS_DRIVER_ASSISTANCE_AUTO_START_STOP_TEXT
    ])
    def test_settings_driver_assistance_text(self, settings_driver_assistance_menlo, text):
        assert get_exist_result(text)


class TestSettingsVehicle(object):
    @jira_test("VHAT-2163")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_TOGLE_INACTIVE,
        Icons.GENERAL_INFO
    ])
    def test_settings_vehicle_images(self, settings_vehicle_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-164")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_VEHICLE_TITLE_TEXT, Text.SETTINGS_VEHICLE_VEHICLE_POWER_DOWN_TIMER,
        Text.SETTINGS_VEHICLE_EASY_ENTRY_EXIT_TEXT, Text.SETTINGS_VEHICLE_PARK_LOCK_CONTROL_TEXT,
        Text.SETTINGS_VEHICLE_SILENT_MODE_TEXT, Text.SETTINGS_VEHICLE_MYKEY_TEXT,
        Text.SETTINGS_VEHICLE_ONBOARD_MODEM_SERIAL_NUMBER_TEXT, Text.SETTINGS_VEHICLE_ALARM_SYSTEM_TEXT,
        Text.SETTINGS_VEHICLE_CHIMES_TEXT, Text.SETTINGS_VEHICLE_REMOTE_START_SETUP_TEXT,
        Text.SETTINGS_VEHICLE_WINDOWS_TEXT, Text.SETTINGS_VEHICLE_WIPERS_TEXT
    ])
    def test_settings_vehicle_text(self, settings_vehicle_menlo, text):
        assert get_exist_result(text)


class TestSettingsGeneral(object):
    @jira_test("VHAT-2165")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_TOGLE_ACTIVE,
        Icons.GENERAL_INFO
    ])
    def test_settings_general_images(self, settings_general_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2166")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_GENERAL_TITLE_TEXT, Text.SETTINGS_GENERAL_LANGUAGE_TEXT,
        Text.SETTINGS_GENERAL_LANG_ENGLISH_US_TEXT, Text.SETTINGS_GENERAL_TEMPERATURE_UNITS_TEXT,
        Text. SETTINGS_GENERAL_TEMP_CELSIUS_TEXT, Text.SETTINGS_GENERAL_MEASUREMENT_UNITS_TEXT,
        Text. SETTINGS_GENERAL_MEASUR_MILES_AND_MPG_TEXT, Text.SETTINGS_GENERAL_TIRE_PRESSURE_UNITS_TEXT,
        Text.SETTINGS_GENERAL_TIRE_PRESSURE_PSI_TEXT, Text.SETTINGS_GENERAL_TOUCHSCREEN_BEEP_TEXT,
        Text.SETTINGS_GENERAL_ABOUT_SYNC_TEXT, Text.SETTINGS_GENERAL_SOFTWARE_LICENSES_TEXT,
        Text.SETTINGS_GENERAL_SUBMIT_FEEDBACK_TEXT, Text.SETTINGS_GENERAL_RESET_TEXT
    ])
    def test_settings_general_text(self, settings_general_menlo, text):
        assert get_exist_result(text)


class TestSettingsSystemUpdates(object):
    @jira_test("VHAT-2167")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_UP,
        Icons.GENERAL_TOGLE_INACTIVE,
        Icons.GENERAL_INFO
    ])
    def test_settings_system_updates_images(self, settings_system_updates_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2168")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_SYSTEM_UPDATES_TITLE_TEXT, Text.SETTINGS_SYSTEM_UPDATES_AUTOMATIC_UPDATES_TEXT,
        Text.SETTINGS_SYSTEM_UPDATES_NOTIFICATIONS_TEXT, Text.SETTINGS_SYSTEM_UPDATES_SCHEDULE_UPDATES_TEXT,
        Text.SETTINGS_SYSTEM_UPDATES_UPDATE_DETAILS_TEXT
    ])
    def test_settings_system_updates_text(self, settings_system_updates_menlo, text):
        assert get_exist_result(text)


class TestSettingsDisplay(object):
    @jira_test("VHAT-2078")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_INFO
    ])
    def test_settings_display_images(self, settings_display_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2079")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_DISPLAY_TITLE_TEXT, Text.SETTINGS_DISPLAY_BRIGHTNESS,
        Text.SETTINGS_DISPLAY_MODE_CLUSTER, Text.SETTINGS_DISPLAY_MODE_CLUSTER_STATE_AUTO,
        Text.SETTINGS_DISPLAY_MODE_CENTER_STACK, Text.SETTINGS_DISPLAY_MODE_CENTER_STACK_STATE_DAY
    ])
    def test_settings_display_text(self, settings_display_menlo, text):
        assert get_exist_result(text)


class TestSettingsCharge(object):
    @jira_test("VHAT-2139")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.SETTINGS_CHARGE_PERCENTE_ICON,
        Icons.GENERAL_INFO,
        Icons.GENERAL_DRAWER_HANDLE
    ])
    def test_settings_charge_images(self, settings_charge_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2138")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_CHARGE_STATUS_TEXT, Text.SETTINGS_CHARGE_PREFERENCES_TEXT,
        Text.SETTINGS_CHARGE_CHARGE_SCHEDULING_TEXT, Text.SETTINGS_CHARGE_DEPARTURE_AND_COMFORT_TEXT
    ])
    def test_settings_charge_text(self, settings_charge_menlo, text):
        assert get_exist_result(text)


class TestSettingsVehicleHotspot(object):
    @jira_test("VHAT-2140")
    @pytest.mark.image_recognition_menlo
    def test_settings_vehile_hotspot_images(self, settings_vehile_hotspot_menlo):
        assert get_exist_result(Icons.SETTINGS_VEHICLE_HOTSPOT_SIGNAL_STRENGTH_ICON)

    @jira_test("VHAT-2141")
    @pytest.mark.text_recognition_menlo
    def test_settings_vehicle_hotspot_text(self, settings_vehile_hotspot_menlo):
        assert get_exist_result(Text.SETTINGS_VEHICLE_HOTSPOT_TEXT)


class TestSettingsPersonalProfiles(object):
    @jira_test("VHAT-2142")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.SETTINGS_PERSONAL_PROFILES_NAME_PROFILE_ICON,
        Icons.SETTINGS_PERSONAL_PROFILES_LINK_MEMORY_ICON,
        Icons.SETTINGS_PERSONAL_PROFILES_ADD_PHOTO_ICON,
        Icons.SETTINGS_PERSONAL_PROFILES_LINK_KEY_FOB_ICON,
        Icons.SETTINGS_PERSONAL_PROFILES_LINK_PHONE_ICON,
        Icons.GENERAL_INFO
    ])
    def test_settings_personal_profiles_images(self, settings_personal_profiles_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2143")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_PERSONAL_PROFILES_NAME_PROFILE_TEXT,
        Text.SETTINGS_PERSONAL_PROFILES_LINK_MEMORY_TEXT,
        Text.SETTINGS_PERSONAL_PROFILES_ADD_PHOTO_TEXT,
        Text.SETTINGS_PERSONAL_PROFILES_LINK_KEY_FOB_TEXT,
        Text.SETTINGS_PERSONAL_PROFILES_LINK_PHONE_TEXT,
        Text.SETTINGS_PERSONAL_PROFILES_TEXT,
        Text.SETTINGS_PERSONAL_PROFILES_CREATE_PROFILE_BUTTON_TEXT
    ])
    def test_settings_personal_profiles_text(self, settings_personal_profiles_menlo, text):
        assert get_exist_result(text)


class TestSettingsAmbientLight(object):
    @jira_test("VHAT-2144")
    @pytest.mark.image_recognition_menlo
    def test_settings_ambient_light_images(self, settings_ambient_light_menlo):
        assert get_exist_result(Icons.SETTINGS_AMBIENT_LIGHT_SLIDER_ICON)

    @jira_test("VHAT-2145")
    @pytest.mark.text_recognition_menlo
    def test_settings_ambient_light_text(self, settings_ambient_light_menlo):
        assert get_exist_result(Text.SETTINGS_AMBIENT_LIGHT_RESET_BUTTON)
