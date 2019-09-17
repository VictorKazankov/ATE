import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync4.constants import TASK_LINK, Icons, Text


@allure.testcase(TASK_LINK.format("VHAT-477"), "VHAT-477")
@pytest.mark.exist_image
def test_exist_phone_settings_button(driver_sync4):
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
        assert helpers.get_exist_result(Icons.ADD_PHONE_SETTINGS_BUTTON)


@allure.testcase(TASK_LINK.format("VHAT-477"), "VHAT-477")
@pytest.mark.exist_text
def test_exist_add_phone_text(driver_sync4):
    if not driver_sync4.phone_page.phone_page_is_active():
        driver_sync4.phone_page.open_phone_page()
    assert helpers.get_exist_result(Text.ADD_PHONE_TEXT)
