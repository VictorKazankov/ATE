import pytest

from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.menlo.constants import Icons, Text

pytestmark = pytest.mark.recognition_menlo


class TestMainScreen(object):
    @pytest.mark.image_recognition
    @pytest.mark.parametrize('icon', [
        Icons.MAIN_APPS_BUTTON_INACTIVE,
        Icons.MAIN_CLIMATE_EXTENDED_BUTTON_INACTIVE,
        Icons.MAIN_FAN_INACTIVE,
        Icons.MAIN_HEAT_WINDSHIELD,
        Icons.MAIN_MINUS_BUTTON,
        Icons.MAIN_PLUS_BUTTON,
        Icons.MAIN_R_HEAT_INACTIVE,
        Icons.MAIN_SETTINGS_CONTROLS_BUTTON_INACTIVE,
        Icons.MAIN_VOL_ON_OFF
    ])
    def test_main_screen_images(self, menlo, icon):
        assert get_exist_result(icon)

    @pytest.mark.text_recognition
    @pytest.mark.parametrize('text', [
        Text.MAIN_AUTO_TEXT,
        Text.MAIN_MAX_TEXT,
        Text.MAIN_VOL_TEXT
    ])
    def test_main_screen_text(self, menlo, text):
        assert get_exist_result(text)
