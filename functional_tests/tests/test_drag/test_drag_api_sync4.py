import vhat_client

import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync4.constants import TASK_LINK, Icons, Text


@allure.testcase(TASK_LINK.format("VHAT-475"), "VHAT-475")
@pytest.mark.image_drag_api
def test_drag_api_horizontally(driver_sync4):
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    helpers.touch_and_drag_page('settings_page', x=1000, y=1000, dx=-700, dy=0)
    assert helpers.get_exist_result(Icons.SETTINGS_PAGE_VEHICLE_BUTTON)
    # return the page to previous state and check it
    helpers.touch_and_drag_page('settings_page_vehicle_button', x=100, y=600, dx=700, dy=0)
    assert helpers.get_exist_result(Text.CLOCK_SETTINGS_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-475"), "VHAT-475")
@pytest.mark.text_drag_api
def test_drag_api_vertically(driver_sync4):
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    driver_sync4.settings_page.open_general_settings()
    helpers.touch_and_drag_page('general_settings_page', x=1000, y=1000, dx=0, dy=-700)
    assert helpers.get_exist_result(Text.MASTER_RESET_TEXT)
    driver_sync4.settings_page.tap_on_back_button()


@allure.testcase(TASK_LINK.format("VHAT-475"), "VHAT-475")
@pytest.mark.image_drag_api_with_modifier
def test_drag_api_horizontally_with_modifier_state_argument(driver_sync4):
    modifier_value = vhat_client.ModifierState.ALT
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    helpers.touch_and_drag_page('settings page', x=1000, y=1000, dx=-700, dy=0, modifier=modifier_value)
    assert helpers.get_exist_result(Icons.SETTINGS_PAGE_VEHICLE_BUTTON)
    # return the page to previous state and check it
    helpers.touch_and_drag_page('settings_page', x=100, y=600, dx=700, dy=0)
    assert helpers.get_exist_result(Text.CLOCK_SETTINGS_TEXT)