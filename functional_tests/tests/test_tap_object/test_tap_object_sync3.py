import allure
import pytest
import vhat_client
from functional_tests.pages import hmi
from functional_tests.utils.sync3.constants import TASK_LINK, Icons, Text

pytestmark = pytest.mark.regression_sync3


@allure.testcase(TASK_LINK.format("VHAT-1610"), "VHAT-1610")
def test_tap_object_by_object_data(get_state_audio_button_object_sync3):
    test_object = get_state_audio_button_object_sync3
    # tabObject returns nothing therefore getting existing text after going to page
    hmi.tap_object(test_object)
    assert hmi.obj_exists(Text.SOURCES_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-1611"), "VHAT-1611")
def test_tap_object_by_screen_point_object_data(get_state_audio_button_object_sync3):
    screen_point_object = vhat_client.ScreenPoint()
    icon_object = get_state_audio_button_object_sync3
    # copy coordinates x and y from icon_object to screen_point_object for click on button
    screen_point_object.x, screen_point_object.y = icon_object.x, icon_object.y
    hmi.tap_object(screen_point_object)
    assert hmi.obj_exists(Text.SOURCES_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-1612"), "VHAT-1612")
def test_tap_object_by_screen_rectangle_object_data(get_state_audio_button_object_sync3):
    screen_rectangle_object = vhat_client.ScreenRectangle()
    icon_object = get_state_audio_button_object_sync3
    # copy coordinates x, y, height, width from icon_object to screen_rectangle_object for click on button
    screen_rectangle_object.x, screen_rectangle_object.y = icon_object.x, icon_object.y
    screen_rectangle_object.height, screen_rectangle_object.width = icon_object.height, icon_object.width
    hmi.tap_object(screen_rectangle_object)
    assert hmi.obj_exists(Text.SOURCES_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-1613"), "VHAT-1613")
def test_none_value_for_tap_object(get_state_audio_button_object_sync3):
    test_object = get_state_audio_button_object_sync3
    assert None == hmi.tap_object(test_object)
