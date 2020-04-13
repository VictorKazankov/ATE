import allure
import pytest
import vhat_client

from functional_tests.pages.interaction import return_obj_if_exist, tap_until_visible
from functional_tests.tests import helpers
from functional_tests.utils.sync4.constants import TASK_LINK, Icons, Text

pytestmark = [pytest.mark.regression_sync4, pytest.mark.touchAndDrag]


@pytest.fixture()
def open_settings_page(settings_sync4):
    yield settings_sync4
    obj2 = return_obj_if_exist(Text.SETTINGS_VEHICLE_BUTTON_TEXT)
    if obj2:
        helpers.touch_and_drag_page('settings_page_vehicle_button', x=obj2.x, y=obj2.y, dx=obj2.width * 2, dy=obj2.y)


@pytest.fixture()
def open_settings_general_send_feedback(settings_sync4):
    settings_sync4.open_send_feedback()
    yield
    tap_until_visible(Icons.BACK_BUTTON)


def do_drag_horizontal(item1, item2, modifier_value=None):
    def do_swipe(obj1, item_to_check_after_swipe, item_to_check_after_swipe_back, marker=-1):
        # from left to right is positive marker
        helpers.touch_and_drag_page('settings_page', x=obj1.x, y=obj1.y, dx=obj1.width * 2 * marker, dy=obj1.y,
                                    modifier=modifier_value)
        assert helpers.get_exist_result(item_to_check_after_swipe)

        obj2 = return_obj_if_exist(item_to_check_after_swipe)
        # from right to left is negative marker
        helpers.touch_and_drag_page('settings_page', x=obj2.x, y=obj2.y, dx=obj2.width * 2 * -marker, dy=obj2.y,
                                    modifier=modifier_value)
        assert helpers.get_exist_result(item_to_check_after_swipe_back)

    obj1 = return_obj_if_exist(item1)
    obj2 = return_obj_if_exist(item2)

    if obj1:
        do_swipe(obj1, item2, item1, -1)
    else:
        do_swipe(obj2, item1, item2, 1)


@allure.testcase(TASK_LINK.format("VHAT-967"), "VHAT-967")
@pytest.mark.image_drag_api
def test_drag_api_horizontally(open_settings_page):
    do_drag_horizontal(Text.CLOCK_SETTINGS_TEXT, Text.SETTINGS_VEHICLE_BUTTON_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-968"), "VHAT-968")
@pytest.mark.text_drag_api
def test_drag_api_vertically(open_settings_general_send_feedback):

    obj = return_obj_if_exist(Text.SETTINGS_GENERAL_FEEDBACK_VOICE)
    obj2 = return_obj_if_exist(Text.SETTINGS_GENERAL_FEEDBACK_USER_INTERFACE)

    assert obj and obj2
    helpers.touch_and_drag_page('settings_general_feedback_page', x=obj.x, y=obj.y, dx=0, dy=-obj.height * 3)
    assert not return_obj_if_exist(Text.SETTINGS_GENERAL_FEEDBACK_USER_INTERFACE)
    helpers.touch_and_drag_page('settings_general_feedback_page', x=obj.x, y=obj.y, dx=0, dy=obj.height * 3)
    assert return_obj_if_exist(Text.SETTINGS_GENERAL_FEEDBACK_USER_INTERFACE)


@allure.testcase(TASK_LINK.format("VHAT-966"), "VHAT-966")
@pytest.mark.image_drag_api_with_modifier
def test_drag_api_horizontally_with_modifier_state_argument(open_settings_page):
    modifier_value = vhat_client.ModifierState.ALT
    do_drag_horizontal(Text.CLOCK_SETTINGS_TEXT, Text.SETTINGS_VEHICLE_BUTTON_TEXT, modifier_value)
