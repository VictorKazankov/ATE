import pytest

from functional_tests.pages.menlo.constants import Icons, Text
from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.report import jira_test

pytestmark = [pytest.mark.recognition_menlo,
              pytest.mark.regression_menlo]


class TestMainScreen(object):
    @jira_test("VHAT-2067")
    @pytest.mark.image_recognition_menlo
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
    def test_main_screen_images(self, main_menlo, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2066")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.MAIN_AUTO_TEXT,
        Text.MAIN_MAX_TEXT,
        Text.MAIN_VOL_TEXT
    ])
    def test_main_screen_text(self, main_menlo, text):
        assert get_exist_result(text)
