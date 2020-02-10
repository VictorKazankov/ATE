import allure
import pytest
from vhat_client import InvalidRectangleCoordinates

from functional_tests.tests.test_get_text.helpers import (
    get_coordinates_of_sound_text_on_settings_page,
    get_coordinates_rectangle_for_empty_area, get_text_from_screen)
from functional_tests.utils.sync4.constants import TASK_LINK

pytestmark = pytest.mark.regression_sync4


# positive test
@allure.testcase(TASK_LINK.format("VHAT-1317"), "VHAT-1317")
def test_get_text_from_settings_page(driver_sync4):
    driver_sync4.settings_page.open_settings_page()
    get_text = get_text_from_screen(*get_coordinates_of_sound_text_on_settings_page())
    # TODO:check validity of spaces, next line symbols existence
    assert 'Sound' == get_text


# negative tests
@allure.testcase(TASK_LINK.format("VHAT-1317"), "VHAT-1317")
def test_get_text_from_empty_area(driver_sync4):
    driver_sync4.settings_page.open_settings_page()
    get_text = get_text_from_screen(*get_coordinates_rectangle_for_empty_area())
    assert get_text == u''


@allure.testcase(TASK_LINK.format("VHAT-1317"), "VHAT-1317")
@pytest.mark.parametrize('test_data', [
    # border overseeded
    (10000, 10000, 20000, 20000, InvalidRectangleCoordinates),
    # negative coordinates
    (-1, -10, -1, -10, TypeError),
    # zero height
    (10, 50, 15, 50, InvalidRectangleCoordinates),
    # zero width
    (10, 50, 10, 55, InvalidRectangleCoordinates),
    # top left lower than right bottom
    (50, 10, 10, 50, InvalidRectangleCoordinates),
    # point
    (0, 0, 0, 0, InvalidRectangleCoordinates),
    ('str', -10, -1, -10, TypeError),
    (None, 10, 50, 50, TypeError),
    (10.001, 10, 50, 50, TypeError)
])
def test_error_invalid_rectangle_cordinations(driver_sync4, test_data):
    x1, y1, x2, y2, error = test_data
    with pytest.raises(error):
        # set incorrect coordinates values
        get_text_from_screen(x1, y1, x2, y2)
