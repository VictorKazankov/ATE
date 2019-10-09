import allure
import pytest
import vhat_client
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import TASK_LINK, Icons


@allure.testcase(TASK_LINK.format("VHAT-475"), "VHAT-475")
@pytest.mark.image_drag_api
def test_drag_api_horizontally(driver_sync3):
    if not driver_sync3.settings_page.setting_page_is_active():
        driver_sync3.settings_page.open_settings_page()
    helpers.touch_and_drag_page('settings_page', x=400, y=300, dx=-300, dy=0)
    assert helpers.get_exist_result(Icons.SETTINGS_GENERAL_BUTTON)
    # return the page to previous state and check it
    helpers.touch_and_drag_page('settings_page', x=50, y=200, dx=400, dy=0)
    assert helpers.get_exist_result(Icons.SETTINGS_SOUND_BUTTON)


@allure.testcase(TASK_LINK.format("VHAT-475"), "VHAT-475")
@pytest.mark.text_drag_api
def test_drag_api_vertically(driver_sync3):
    if not driver_sync3.settings_page.setting_page_is_active():
        driver_sync3.settings_page.open_settings_page()
    driver_sync3.settings_page.open_setting_audio_page()
    helpers.touch_and_drag_page('sound settings page', x=400, y=300, dx=0, dy=-200)
    assert helpers.get_exist_result(Icons.INFORMATION_DIALOG_BUTTON)
    driver_sync3.settings_audio_page.tap_on_back_button()


@allure.testcase(TASK_LINK.format("VHAT-475"), "VHAT-475")
@pytest.mark.drag_api_modifier
def test_drag_api_horizontally_with_modifier_state_argument(driver_sync3):
    modifier_value = vhat_client.ModifierState.ALT
    if not driver_sync3.settings_page.setting_page_is_active():
        driver_sync3.settings_page.open_settings_page()
    helpers.touch_and_drag_page('settings_page', x=400, y=300, dx=-300, dy=0, modifier=modifier_value)
    assert helpers.get_exist_result(Icons.SETTINGS_GENERAL_BUTTON)
    # return the page to previous state and check it
    helpers.touch_and_drag_page('settings_page', x=50, y=200, dx=400, dy=0)
    assert helpers.get_exist_result(Icons.SETTINGS_SOUND_BUTTON)
