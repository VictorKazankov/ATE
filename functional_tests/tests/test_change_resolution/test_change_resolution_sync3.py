import allure
import pytest
from functional_tests.tests.test_change_resolution.helpers import (
    change_resolution, check_resolution_in_response)
from functional_tests.utils.sync3.constants import TASK_LINK

RESOLUTION_8 = {'type': 1, 'height': 480, 'weight': 800}
RESOLUTION_10 = {'type': 3, 'height': 800, 'width': 1280}


@pytest.mark.filterwarnings('ignore::urllib3.exceptions.InsecureRequestWarning')
@allure.testcase(TASK_LINK.format("VHAT-535"), "VHAT-535")
def test_change_resolution_sync3(driver_sync3):
    driver_sync3.settings_page.open_settings_page()
    assert driver_sync3.settings_page.setting_page_is_active()
    response = change_resolution(RESOLUTION_10)
    check_resolution_in_response(response, RESOLUTION_10)
    driver_sync3.audio_page.open_audio_page()
    assert driver_sync3.audio_page.audio_page_is_active()
    change_resolution(RESOLUTION_8)
