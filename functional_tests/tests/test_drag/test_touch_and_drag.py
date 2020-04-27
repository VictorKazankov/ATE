import pytest
import vhat_client

from functional_tests.pages import hmi
from functional_tests.pages.interaction import return_obj_if_exist, tap_until_visible
from functional_tests.tests.import_helper_functions import Data
from functional_tests.utils.report import jira_test

pytestmark = [pytest.mark.touchAndDrag, pytest.mark.regression_sync4, pytest.mark.regression_sync3]

path = "test_touch_and_drag"

page_to_check_horizontal = Data(path).page_to_check_horizontal
page_to_check_vertical = Data(path).page_to_check_vertical
back_icon = Data(path).back_icon

marker_for_1st_settings_page = Data(path).marker_for_1st_settings_page
marker_for_2nd_setting_page = Data(path).marker_for_2nd_setting_page
vertical_item_top = Data(path).vertical_item_top
vertical_item_bottom = Data(path).vertical_item_bottom


@pytest.fixture(scope="function")
def open_settings_page(attach_to_app):
    yield page_to_check_horizontal()
    obj2 = return_obj_if_exist(marker_for_2nd_setting_page)
    if obj2:
        hmi.touch_and_drag('settings_page_vehicle_button', x=obj2.x, y=obj2.y, dx=obj2.width * 2, dy=obj2.y)


@pytest.fixture(scope="function")
def open_page_for_vertical_check(attach_to_app):
    yield page_to_check_vertical()
    tap_until_visible(back_icon)


def do_drag_horizontal(item1, item2, modifier_value=None):
    def do_swipe(obj1, item_to_check_after_swipe, item_to_check_after_swipe_back, marker=-1):
        # from left to right is positive marker
        hmi.touch_and_drag('settings_page', x=obj1.x, y=obj1.y, dx=obj1.width * 2 * marker, dy=obj1.y,
                                    modifier=modifier_value)
        assert hmi.obj_exists(item_to_check_after_swipe)

        obj2 = return_obj_if_exist(item_to_check_after_swipe)
        # from right to left is negative marker
        hmi.touch_and_drag('settings_page', x=obj2.x, y=obj2.y, dx=obj2.width * 2 * -marker, dy=obj2.y,
                                    modifier=modifier_value)
        assert hmi.obj_exists(item_to_check_after_swipe_back)

    obj1 = return_obj_if_exist(item1)
    obj2 = return_obj_if_exist(item2)

    if obj1:
        do_swipe(obj1, item2, item1, -1)
    else:
        do_swipe(obj2, item1, item2, 1)


@jira_test("VHAT-967")
def test_drag_api_horizontally(open_settings_page):
    do_drag_horizontal(marker_for_1st_settings_page, marker_for_2nd_setting_page)


@jira_test("VHAT-968")
def test_drag_api_vertically(open_page_for_vertical_check):

    obj = return_obj_if_exist(vertical_item_bottom)
    obj2 = return_obj_if_exist(vertical_item_top)

    assert obj and obj2
    hmi.touch_and_drag('settings_page', x=obj.x, y=obj.y, dx=0, dy=-obj.height * 3)
    assert not hmi.obj_exists(vertical_item_top)
    hmi.touch_and_drag('settings_page', x=obj.x, y=obj.y, dx=0, dy=obj.height * 3)
    assert hmi.obj_exists(vertical_item_top)


@jira_test("VHAT-966")
def test_drag_api_horizontally_with_modifier_state_argument(open_settings_page):
    modifier_value = vhat_client.ModifierState.ALT
    do_drag_horizontal(marker_for_1st_settings_page, marker_for_2nd_setting_page, modifier_value)
