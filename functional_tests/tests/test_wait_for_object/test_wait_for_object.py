import allure
import pytest
import vhat_client

from functional_tests.pages import hmi
from functional_tests.pages.hmi import wait_for_object
from functional_tests.tests.helpers import check_wildcard_results_amount
from functional_tests.tests.import_helper_functions import data
from functional_tests.utils.sync4.constants import TASK_LINK

pytestmark = [pytest.mark.regression_sync4, pytest.mark.regression_sync3, pytest.mark.waitForObject]

path = 'test_wait_for_object'

icon = data(path, name='icon')


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
    [icon, 1000]
])
def test_wait_for_object_by_string_data(string_data):
    obj = hmi.wait_for_object(*string_data)
    assert isinstance(obj, vhat_client.object)


@allure.testcase(TASK_LINK.format("VHAT-1601"), "VHAT-1601")
@pytest.mark.parametrize('object_data', [
    [create_vhat_object()],
    [create_vhat_object(), 1000]
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
@pytest.mark.parametrize('test_data', data(path, 'wildcard_test_data'))
def test_wildcard_wait_for_object(test_data):
    wildcard = vhat_client.Wildcard(test_data)
    # to check several results
    check_wildcard_results_amount(wildcard.getMatchObjects(), test_data, 2)
    try:
        wait_for_object(wildcard)
    except LookupError:
        assert False, 'Failed waitForObject for wildcard with params {}'.format(test_data)
