import pytest
from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.menlo.constants import Text

pytestmark = pytest.mark.recognition_menlo

# Data
settings_page_general_button_text = [Text.SETTINGS_PANEL_CLOCK_BUTTON_TEXT,
                                     Text.SETTINGS_PANEL_PHONE_LIST_BUTTON_TEXT,
                                     Text.SETTINGS_PANEL_CONNECTIVITY_BUTTON_TEXT,
                                     Text.SETTINGS_PANEL_RADIO_BUTTON_TEXT,
                                     Text.SETTINGS_PANEL_VEHICLE_BUTTON_TEXT]

settings_sound_page_text = [Text.SETTINGS_SOUND_TITLE_TEXT,
                            Text.SETTINGS_SOUND_TONE_TEXT,
                            Text.SETTINGS_SOUND_BALANCE_FADE_TEXT,
                            Text.RESET_TEXT]

settings_clock_page_text = [Text.SETTINGS_CLOCK_TITLE_TEXT,
                            Text.SETTINGS_CLOCK_MODE_TEXT,
                            Text.SETTINGS_CLOCK_TIME_ZONE_TEXT]

settings_phone_page_text = [Text.SETTINGS_PHONE_TITLE_TEXT,
                            Text.SETTINGS_PHONE_ADD_PHONE_TEXT]

settings_connectivity_page_text = [Text.SETTINGS_CONNECTIVITY_TITLE_TEXT,
                                   Text.SETTINGS_CONNECTIVITY_FEATURES_TEXT,
                                   Text.SETTINGS_CONNECTIVITY_BLUETOOTH_TEXT,
                                   Text.SETTINGS_CONNECTIVITY_PROJECTION_TEXT,
                                   Text.SETTINGS_CONNECTIVITY_NETWORKS_TEXT]


# Tests
@pytest.mark.parametrize('testdata', settings_page_general_button_text)
def test_settings_general_buttons_text(settings_general_page_menlo, testdata):
    assert get_exist_result(testdata)


@pytest.mark.parametrize('testdata', settings_sound_page_text)
def test_settings_sound_text(settings_general_page_menlo, testdata):
    assert get_exist_result(testdata)


@pytest.mark.parametrize('testdata', settings_clock_page_text)
def test_settings_clock_text(settings_general_page_menlo, testdata):
    settings_general_page_menlo.settings_page.open_settings_clock_page()
    assert get_exist_result(testdata)


@pytest.mark.parametrize('testdata', settings_phone_page_text)
def test_settings_phone_text(settings_general_page_menlo, testdata):
    settings_general_page_menlo.settings_page.open_settings_phone_list_page()
    assert get_exist_result(testdata)


@pytest.mark.parametrize('testdata', settings_connectivity_page_text)
def test_settings_connectivity_text(settings_general_page_menlo, testdata):
    settings_general_page_menlo.settings_page.open_settings_connectivity_page()
    assert get_exist_result(testdata)
