import pytest

from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.menlo.constants import Text, Icons

pytestmark = pytest.mark.recognition_menlo


class TestSettingsFirstPart(object):
    @pytest.mark.image_recognition
    @pytest.mark.parametrize('icon', [
        Icons.SETTINGS_SOUND_BUTTON, Icons.SETTINGS_CLOCK_BUTTON,
        Icons.SETTINGS_PHONE_LIST_BUTTON, Icons.SETTINGS_CONNECTIVITY_BUTTON,
        Icons.SETTINGS_RADIO_BUTTON, Icons.SETTINGS_VEHICLE_BUTTON
    ])
    def test_settings_first_part_icons(self, settings_menlo, icon):
        assert get_exist_result(icon)

    @pytest.mark.text_recognition
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_TAB_TITLE_TEXT, Text.SETTINGS_SOUND_BUTTON_TEXT,
        Text.SETTINGS_CLOCK_BUTTON_TEXT, Text.SETTINGS_PHONE_LIST_BUTTON_TEXT,
        Text.SETTINGS_CONNECTIVITY_BUTTON_TEXT, Text.SETTINGS_RADIO_BUTTON_TEXT,
        Text.SETTINGS_VEHICLE_BUTTON_TEXT
    ])
    def test_settings_first_part_text(self, settings_menlo, text):
        assert get_exist_result(text)


class TestSettingsSound(object):
    @pytest.mark.image_recognition
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_INFO
    ])
    def test_settings_sound_icons(self, settings_sound_menlo, icon):
        assert get_exist_result(icon)

    @pytest.mark.text_recognition
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_SOUND_TITLE_TEXT, Text.SETTINGS_SOUND_TONE_TEXT,
        Text.SETTINGS_SOUND_BALANCE_FADE_TEXT, Text.SETTINGS_SOUND_DSP_OCCUPANCY_TEXT

    ])
    def test_settings_sound_text(self, settings_sound_menlo, text):
        assert get_exist_result(text)


class TestSettingsClock(object):
    @pytest.mark.image_recognition
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_UP, Icons.GENERAL_ARROW_DOWN_DISABLED,
        Icons.GENERAL_INFO, Icons.GENERAL_TOGLE_INACTIVE,
        Icons.SETTINGS_CLOCK_AM_BUTTON_ACTIVE, Icons.SETTINGS_CLOCK_PM_BUTTON_INACTIVE
    ])
    def test_settings_clock_icons(self, settings_clock_menlo, icon):
        assert get_exist_result(icon)

    @pytest.mark.text_recognition
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_CLOCK_TITLE_TEXT,
        Text.SETTINGS_CLOCK_MODE_TEXT,
        Text.SETTINGS_CLOCK_TIME_ZONE_TEXT

    ])
    def test_settings_clock_text(self, settings_clock_menlo, text):
        assert get_exist_result(text)


class TestSettingsPhoneList(object):
    @pytest.mark.image_recognition
    @pytest.mark.parametrize('icon', [
        Icons.SETTINGS_PHONE_LIST_ADD_PHONE_BUTTON,
        Icons.GENERAL_INFO
    ])
    def test_settings_phone_list_icons(self, settings_phone_list_menlo, icon):
        assert get_exist_result(icon)

    @pytest.mark.text_recognition
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_PHONE_LIST_TITLE_TEXT,
        Text.SETTINGS_PHONE_LIST_ADD_PHONE_TEXT
    ])
    def test_settings_phone_list_text(self, settings_phone_list_menlo, text):
        assert get_exist_result(text)


class TestSettingsConnectivity(object):
    @pytest.mark.image_recognition
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_INFO
    ])
    def test_settings_connectivity_icons(self, settings_connectivity_menlo, icon):
        assert get_exist_result(icon)

    @pytest.mark.text_recognition
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_CONNECTIVITY_TITLE_TEXT, Text.SETTINGS_CONNECTIVITY_FEATURES_TEXT,
        Text.SETTINGS_CONNECTIVITY_BLUETOOTH_TEXT, Text.SETTINGS_CONNECTIVITY_PROJECTION_TEXT,
        Text.SETTINGS_CONNECTIVITY_NETWORKS_TEXT

    ])
    def test_settings_connectivity_text(self, settings_connectivity_menlo, text):
        assert get_exist_result(text)


class TestSettingsDisplay(object):
    @pytest.mark.image_recognition
    @pytest.mark.parametrize('icon', [
        Icons.GENERAL_ARROW_DOWN,
        Icons.GENERAL_INFO
    ])
    def test_settings_display_icons(self, settings_display_menlo, icon):
        assert get_exist_result(icon)

    @pytest.mark.text_recognition
    @pytest.mark.parametrize('text', [
        Text.SETTINGS_DISPLAY_TITLE_TEXT, Text.SETTINGS_DISPLAY_BRIGHTNESS,
        Text.SETTINGS_DISPLAY_MODE_CLUSTER, Text.SETTINGS_DISPLAY_MODE_CLUSTER_STATE_AUTO,
        Text.SETTINGS_DISPLAY_MODE_CENTER_STACK, Text.SETTINGS_DISPLAY_MODE_CENTER_STACK_STATE_DAY

    ])
    def test_settings_display_text(self, settings_display_menlo, text):
        assert get_exist_result(text)
