import allure
import pytest
from functional_tests.tests.test_change_resolution.helpers import (
    change_resolution, check_resolution_in_response)
from functional_tests.utils.sync3.constants import TASK_LINK

RESOLUTION_12 = {'type': 8, 'height': 1200, 'weight': 1920}
RESOLUTION_6 = {'type': 4, 'height': 480, 'width': 800}


@pytest.mark.filterwarnings('ignore::urllib3.exceptions.InsecureRequestWarning')
@allure.testcase(TASK_LINK.format("VHAT-535"), "VHAT-535")
def test_change_resolution_sync4(driver_sync4):
    driver_sync4.settings_page.open_settings_page()
    assert driver_sync4.settings_page.settings_page_is_active()
    response = change_resolution(RESOLUTION_6)
    check_resolution_in_response(response, RESOLUTION_6)
    driver_sync4.audio_page.open_audio_page()
    assert driver_sync4.audio_page.audio_page_is_active()
    change_resolution(RESOLUTION_12)
