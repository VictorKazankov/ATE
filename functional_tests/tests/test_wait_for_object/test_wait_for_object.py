import logging
import allure
import pytest
import vhat_client
from functional_tests.pages import hmi
from functional_tests.tests.helpers import check_wildcard_results_amount
from functional_tests.tests.import_helper_functions import data
from functional_tests.utils.sync4.constants import TASK_LINK
from vhat_client import ScreenPoint

pytestmark = [pytest.mark.regression_sync4, pytest.mark.regression_sync3, pytest.mark.waitForObject]

path = 'test_wait_for_object'

icon = data(path, name='icon')

DEF_LEFT_POINT = ScreenPoint(0, 0)
DEF_RIGHT_POINT = ScreenPoint(0, 0)
DEF_TIMEOUT = 1000

TEST_DATA_INVALID_RECTANGLE_COORDINATES = [
    (ScreenPoint(-10, 10), DEF_RIGHT_POINT),
    (DEF_LEFT_POINT, ScreenPoint(10, -10)),
    (ScreenPoint(9999, 9999), ScreenPoint(9999, 9999)),
    (ScreenPoint(9999, 10), ScreenPoint(10, 10)),
    (ScreenPoint(10, 9999), ScreenPoint(10, 10)),
    (ScreenPoint(10, 9999), ScreenPoint(9999, 10)),
    (ScreenPoint(10, 20), ScreenPoint(15, 20)),
    (ScreenPoint(10, 15), ScreenPoint(10, 20)),
    (ScreenPoint(15, 10), ScreenPoint(15, 10)),
]


def extract_object_coordinates(object):
    x_top_left = object.x
    y_top_left = object.y
    x_bottom_right = x_top_left + object.width
    y_bottom_right = y_top_left + object.height

    return x_top_left, y_top_left, x_bottom_right, y_bottom_right


def get_possible_expand_status(x, y, expand_size):
    expand_top = y - expand_size > 0
    expand_left = x - expand_size > 0
    return expand_top, expand_left


def get_top_left_bottom_right_from_object(object):
    '''
    Gets the coordinates of the object to search in the area where this object exists.
    Since it is impossible to specify coordinates to search for an object already found due to an error when searching
    on a frame of the same size as the pattern, you need to specify a slightly larger area for the search.
    This function slightly expands the already known search area.
    It selects the correct direction of expansion, and always returns the correct coordinates.
    :param object: Existing object
    :return: top_left and bottom_right Point
    '''

    x_top_left, y_top_left, x_bottom_right, y_bottom_right = extract_object_coordinates(object)
    expand_size = 10
    min_expand_size = 2
    expand_top, expand_left = get_possible_expand_status(x_top_left, y_top_left, expand_size)

    if expand_top and expand_left:
        y_top_left -= expand_size
        x_top_left -= expand_size
        # Be careful in the future, this hack avoids the restrictions associated with inaccurate coordinates after the search
        x_bottom_right += expand_size
        y_bottom_right += min_expand_size
    elif expand_top and not expand_left:
        y_top_left -= expand_size
        x_bottom_right += expand_size
        # Be careful in the future, this hack avoids the restrictions associated with inaccurate coordinates after the search
        y_bottom_right += min_expand_size
    elif not expand_top and expand_left:
        y_bottom_right += expand_size
        x_top_left -= expand_size
        # Be careful in the future, this hack avoids the restrictions associated with inaccurate coordinates after the search
        x_bottom_right += expand_size
    elif not expand_top and not expand_left:
        y_bottom_right += expand_size
        x_bottom_right += expand_size

    return ScreenPoint(x_top_left, y_top_left), ScreenPoint(x_bottom_right, y_bottom_right)


def get_negative_top_left_bottom_right_from_object(object):
    '''
    Gets the coordinates of the object to search in the area where this object don't exists.
    This function moves the existing coordinates of the object to another place so that the search in the area
    does not find this object again.
    It selects the correct direction of the move, and always returns the correct coordinates.
    :param object: Existing object
    :return: top_left and bottom_right Point
    '''

    x_top_left, y_top_left, x_bottom_right, y_bottom_right = extract_object_coordinates(object)
    expand_size = 50
    additional_expand = 10
    expand_top, expand_left = get_possible_expand_status(x_top_left, y_top_left, expand_size)

    if expand_left:
        x_top_left -= expand_size
        x_bottom_right -= (expand_size - additional_expand)
    else:
        x_top_left += expand_size
        x_bottom_right += (expand_size + additional_expand)

    if expand_top:
        y_top_left -= expand_size
        y_bottom_right -= (expand_size - additional_expand)
    else:
        y_top_left += expand_size
        y_bottom_right += (expand_size + additional_expand)

    return ScreenPoint(x_top_left, y_top_left), ScreenPoint(x_bottom_right, y_bottom_right)


def create_vhat_object():
    test_object = vhat_client.object()
    test_object.name = icon
    return test_object


@pytest.fixture(scope='module', autouse=True)
def open_page(app_connector):
    open_function = data(path, name='open_page_method')
    open_function()


@allure.testcase(TASK_LINK.format("VHAT-1599"), "VHAT-1599")
@pytest.mark.parametrize('string_data', [
    [icon],
    # object to wait, timeout milliseconds for object to be found
    [icon, DEF_TIMEOUT]
])
def test_wait_for_object_by_string_data(string_data):
    obj = hmi.wait_for_object(*string_data)
    assert isinstance(obj, vhat_client.object)


@allure.testcase(TASK_LINK.format("VHAT-1601"), "VHAT-1601")
@pytest.mark.parametrize('object_data', [
    [create_vhat_object()],
    [create_vhat_object(), DEF_TIMEOUT],
    [create_vhat_object(), DEF_TIMEOUT, vhat_client.ScreenPoint(0, 0), vhat_client.ScreenPoint(0, 0)]
])
def test_wait_for_object_by_object_data(object_data):
    obj = hmi.wait_for_object(*object_data)
    assert isinstance(obj, object)


@allure.testcase(TASK_LINK.format("VHAT-1602"), "VHAT-1602")
@pytest.mark.parametrize('exception_data', [
    ('incorrect_string', vhat_client.LookupError),
    (None, TypeError)
])
def test_wait_for_object_exceptions(exception_data):
    param, error = exception_data
    with pytest.raises(error):
        hmi.wait_for_object(param)


@allure.testcase(TASK_LINK.format("VHAT-1685"), "VHAT-1685")
def test_wildcard_wait_for_object():
    wildcard_data = data(path, 'wildcard_test_data')
    wildcard = vhat_client.Wildcard(*wildcard_data)
    check_wildcard_results_amount(wildcard.getMatchObjects(), wildcard_data, 2)

    try:
        hmi.wait_for_object(wildcard)
    except vhat_client.LookupError:
        assert False, 'Failed waitForObject for wildcard with params {}'.format(wildcard_data)


@allure.testcase(TASK_LINK.format("VHAT-2045"), "VHAT-2045")
def test_wait_for_object_by_string_data_with_searching_area_positive():
    obj = hmi.wait_for_object(icon)
    top_left, bottom_right = get_top_left_bottom_right_from_object(obj)
    result_obj = hmi.wait_for_object(icon, DEF_TIMEOUT, top_left, bottom_right)
    assert isinstance(result_obj, vhat_client.object)


@allure.testcase(TASK_LINK.format("VHAT-2092"), "VHAT-2092")
def test_wait_for_object_by_string_data_with_searching_area_negative():
    with pytest.raises(vhat_client.LookupError) as exc:
        obj = hmi.wait_for_object(icon)
        top_left, bottom_right = get_negative_top_left_bottom_right_from_object(obj)
        hmi.wait_for_object(icon, DEF_TIMEOUT, top_left, bottom_right)
    logging.exception(exc)


@allure.testcase(TASK_LINK.format("VHAT-2054"), "VHAT-2054")
@pytest.mark.parametrize('test_data', TEST_DATA_INVALID_RECTANGLE_COORDINATES)
def test_neg_invalid_rectangle_coordinates_for_object(test_data):
    top_left_coordinate, bottom_right_coordinate = test_data
    with pytest.raises(vhat_client.InvalidRectangleCoordinates) as exc:
        hmi.wait_for_object(icon, DEF_TIMEOUT, top_left_coordinate, bottom_right_coordinate)
    logging.exception(exc)


@allure.testcase(TASK_LINK.format("VHAT-2057"), "VHAT-2057")
@pytest.mark.parametrize('test_data', TEST_DATA_INVALID_RECTANGLE_COORDINATES)
def test_neg_invalid_rectangle_coordinates_for_wildcard(test_data):
    top_left_coordinate, bottom_right_coordinate = test_data
    wildcard = vhat_client.Wildcard(*data(path, 'wildcard_test_data'))

    with pytest.raises(vhat_client.InvalidRectangleCoordinates) as exc:
        hmi.wait_for_object(wildcard, DEF_TIMEOUT, top_left_coordinate, bottom_right_coordinate)
    logging.exception(exc)


@allure.testcase(TASK_LINK.format("VHAT-2058"), "VHAT-2058")
def test_wildcard_wait_for_object_with_searching_area_positive():
    wildcard_data = data(path, 'wildcard_test_data')
    wildcard = vhat_client.Wildcard(*wildcard_data)
    # to check several results
    check_wildcard_results_amount(wildcard.getMatchObjects(), wildcard_data, 2)

    try:
        obj = hmi.wait_for_object(wildcard)
        top_left, bottom_right = get_top_left_bottom_right_from_object(obj)
        hmi.wait_for_object(wildcard, DEF_TIMEOUT, top_left, bottom_right)
    except vhat_client.LookupError:
        assert False, 'Failed waitForObject for wildcard with params {}'.format(data(path, 'wildcard_test_data'))


@allure.testcase(TASK_LINK.format("VHAT-2093"), "VHAT-2093")
def test_wildcard_wait_for_object_with_searching_area_negative():
    wildcard_data = data(path, 'wildcard_test_data')
    wildcard = vhat_client.Wildcard(*wildcard_data)
    # to check several results
    check_wildcard_results_amount(wildcard.getMatchObjects(), wildcard_data, 2)

    with pytest.raises(vhat_client.LookupError) as exc:
        obj = hmi.wait_for_object(wildcard)
        top_left, bottom_right = get_negative_top_left_bottom_right_from_object(obj)
        hmi.wait_for_object(wildcard, DEF_TIMEOUT, top_left, bottom_right)
    logging.exception(exc)
