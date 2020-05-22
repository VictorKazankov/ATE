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
