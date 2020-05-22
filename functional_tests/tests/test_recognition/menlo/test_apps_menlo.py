import pytest

from functional_tests.pages.menlo.constants import Text, Icons
from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.report import jira_test

pytestmark = [pytest.mark.recognition_menlo,
              pytest.mark.regression_menlo]


class TestAppsScreen(object):
    @jira_test("VHAT-2063")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.MAIN_APPS_BUTTON_ACTIVE,
        Icons.APPS_RADIO_BUTTON,
        Icons.APPS_PHONE_BUTTON,
        Icons.APPS_TIRE_PRESSURE_BUTTON,
        Icons.APPS_TRIP_IOD_BUTTON,
        Icons.APPS_SEATBELT_BUTTON,
        Icons.APPS_BLUETOOTH_STEREO_BUTTON,
        Icons.APPS_OWNERS_MANUAL_BUTTON,
        Icons.GENERAL_DRAWER_HANDLE
    ])
    def test_apps_images(self, apps_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2064")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.APPS_RADIO_TEXT,
        Text.APPS_PHONE_TEXT,
        Text.APPS_TIRE_PRESSURE_TEXT,
        Text.APPS_TRIP_IOD_TEXT,
        Text.APPS_SEATBELT_TEXT,
        Text.APPS_BLUETOOTH_STEREO_TEXT,
        Text.APPS_OWNERS_MANUAL_TEXT
    ])
    def test_apps_text(self, apps_menlo, text):
        assert get_exist_result(text)
