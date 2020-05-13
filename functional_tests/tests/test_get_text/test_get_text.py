import pytest
from functional_tests.pages import hmi
from functional_tests.tests.import_helper_functions import Data, data
from functional_tests.utils.report import jira_issue, jira_test
from vhat_client import InvalidRectangleCoordinates

pytestmark = [pytest.mark.regression_sync4, pytest.mark.regression_sync3, pytest.mark.get_text]

path = "test_get_text"

check_object = data(path, name='check_object')
expected_text = data(path, name='expected_text')


@pytest.fixture(scope="function", name='open_test_page')
def open_page(attach_to_app):
    """
    opens predefined page
    """
    Data(path).open_method()


def get_coordinates_of_text_on_page():
    target = hmi.wait_for_object(check_object)
    # get left-up and right-down coordinates text +- 2pc as a recognition distance buffer
    recognition_distance_buffer = 2
    x1 = int(target.x - target.width / 2 - recognition_distance_buffer)
    y1 = int(target.y - target.height / 2 - recognition_distance_buffer)
    x2 = int(target.x + target.width / 2 + recognition_distance_buffer)
    y2 = int(target.y + target.height / 2 + recognition_distance_buffer)
    return [x1, y1, x2, y2]


def get_coordinates_rectangle_for_empty_area():
    target = hmi.wait_for_object(check_object)
    # get right-down coordinates Sound text
    # and use this point as left-up coordinate for rectangle in empty area
    empty_area_size = 5
    x1 = int(target.x + target.width / 2)
    y1 = int(target.y + target.height / 2)
    x2 = x1 + empty_area_size
    y2 = y1 + empty_area_size
    return [x1, y1, x2, y2]


# positive test
@jira_test("VHAT-1317")
@jira_issue("VHAT-1904")
def test_get_text_from_page(open_test_page):
    get_text = hmi.get_text(*get_coordinates_of_text_on_page())
    # TODO:check validity of spaces VHAT-1898
    assert expected_text == get_text


# negative tests
@jira_test("VHAT-1317")
def test_get_text_from_empty_area(open_test_page):
    get_text = hmi.get_text(*get_coordinates_rectangle_for_empty_area())
    assert get_text == u''


@jira_test("VHAT-1317")
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
def test_error_invalid_rectangle_cordinations(test_data):
    x1, y1, x2, y2, error = test_data
    with pytest.raises(error):
        # set incorrect coordinates values
        hmi.get_text(x1, y1, x2, y2)
