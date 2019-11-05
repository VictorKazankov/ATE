import time

import allure
import pytest
from functional_tests.tests.helpers import get_exist_result
from functional_tests.tests.test_change_resolution.helpers import (
    change_resolution, check_resolution_in_response)
from functional_tests.utils.sync4.constants import TASK_LINK, Icons

RESOLUTION_12 = {'type': 8, 'height': 1200, 'weight': 1920}
RESOLUTION_6 = {'type': 4, 'height': 480, 'width': 800}


@pytest.mark.filterwarnings('ignore::urllib3.exceptions.InsecureRequestWarning')
@allure.testcase(TASK_LINK.format("VHAT-969"), "VHAT-969")
@pytest.mark.skip(reason='Waiting for API for sync reboot')
def test_change_resolution_sync4(driver_sync4):
    driver_sync4.settings_audio_page.open_sound_settings_page()
    assert get_exist_result(Icons.SOUND_SETTINGS_TONE_SETTINGS_BUTTON)
    response = change_resolution(RESOLUTION_6)
    check_resolution_in_response(response, RESOLUTION_6)
    time.sleep(1)
    result = get_exist_result(Icons.SOUND_SETTINGS_TONE_SETTINGS_BUTTON)
    change_resolution(RESOLUTION_12)
    assert result
