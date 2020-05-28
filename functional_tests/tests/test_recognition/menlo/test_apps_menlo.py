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


class TestTirePressureApp(object):
    @jira_test("VHAT-2099")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.TIRE_PRESSURE_APP_TITLE_ICON,
        Icons.GENERAL_MAXIMIZE_APP_BUTTON
    ])
    def test_tire_pressure_app_images(self, tire_pressure_app, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-2100")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.TIRE_PRESSURE_APP_TITLE_TEXT,
        Text.TIRE_PRESSURE_APP_DEFAULT_VALUE_TEXT
    ])
    def test_tire_pressure_app_text(self, tire_pressure_app, text):
        assert get_exist_result(text)


class TestTripIodApp(object):
    @jira_test("VHAT-")
    @pytest.mark.image_recognition_menlo
    @pytest.mark.parametrize('icon', [
        Icons.TRIP_IOD_APP_TITLE_ICON,
        Icons.GENERAL_MAXIMIZE_APP_BUTTON,
        Icons.TRIP_IOD_APP_TIMER_ICON,
        Icons.TRIP_IOD_APP_BATTERY_ICON
    ])
    def test_trip_iod_app_images(self, tire_pressure_app, icon):
        assert get_exist_result(icon)

    @jira_test("VHAT-")
    @pytest.mark.text_recognition_menlo
    @pytest.mark.parametrize('text', [
        Text.TRIP_IOD_APP_TITLE_TEXT, Text.TRIP_IOD_APP_TRIP_COMPUTER_1_TAB_TEXT,
        Text.TRIP_IOD_APP_TRIP_COMPUTER_2_TAB_TEXT, Text.TRIP_IOD_APP_INACTIVE_TEXT,
        Text.TRIP_IOD_APP_TIME_SECONDS_DEFAULT_TEXT, Text.TRIP_IOD_APP_NONE_PERCENTAGE_TEXT,
        Text.TRIP_IOD_APP_CLIMATE_USE_TEXT, Text.TRIP_IOD_APP_ROUTE_TEXT,
        Text.TRIP_IOD_APP_ACCESSORIES_TEXT, Text.TRIP_IOD_APP_EXT_TEMP_TEXT
    ])
    def test_trip_iod_app_text(self, tire_pressure_app, text):
        assert get_exist_result(text)
