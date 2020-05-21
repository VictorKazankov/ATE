import pytest

from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.menlo.constants import Text, Icons

pytestmark = pytest.mark.recognition_menlo


class TestAppsScreen(object):

    @pytest.mark.image_recognition
    @pytest.mark.parametrize('icon', [
        Icons.MAIN_APPS_BUTTON_ACTIVE,
        Icons.APPS_RADIO_BUTTON,
        Icons.APPS_PHONE_BUTTON,
        Icons.APPS_TIRE_PRESSURE_BUTTON,
        Icons.APPS_TRIP_IOD_BUTTON,
        Icons.APPS_SEATBELT_BUTTON,
        Icons.APPS_BLUETOOTH_STEREO_BUTTON,
        Icons.APPS_OWNERS_MANUAL_BUTTON
    ])
    def test_apps_images(self, apps_menlo, icon):
        assert get_exist_result(icon)

    @pytest.mark.text_recognition
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
