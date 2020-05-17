import pytest
from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.menlo.constants import Text

pytestmark = pytest.mark.recognition_menlo

# Data
apps_page_text = [Text.APPS_RADIO_TEXT,
                  Text.APPS_PHONE_TEXT,
                  Text.APPS_TIRE_PRESSURE_TEXT,
                  Text.APPS_TRIP_IOD_TEXT,
                  Text.APPS_SEATBELT_TEXT,
                  Text.APPS_BLUETOOTH_STEREO_TEXT,
                  Text.APPS_OWNERS_MANUAL_TEXT]


# Tests
@pytest.mark.parametrize('testdata', apps_page_text)
def test_apps_buttons_text(apps_menlo, testdata):
    assert get_exist_result(testdata)
