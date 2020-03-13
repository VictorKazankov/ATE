import allure
import pytest
from vhat_client import Wildcard

from functional_tests.pages.hmi import get_objects_data_by_pattern
from functional_tests.tests.helpers import check_wildcard_results_content
from functional_tests.tests.import_helper_functions import data
from functional_tests.utils.sync3.constants import TASK_LINK

pytestmark = [pytest.mark.regression_sync4, pytest.mark.regression_sync3, pytest.mark.getObjectsDataByPattern]

path = 'test_get_objects_data_by_pattern'


@allure.testcase(TASK_LINK.format("VHAT-1586"), "VHAT-1586")
@pytest.mark.parametrize('name', data(path, 'test_data_name'))
def test_get_objects_data_by_pattern_name(name):
    objects_list = get_objects_data_by_pattern(name)
    # TODO: update once question VHAT-1699 is answered
    assert len(objects_list) >= 1
    for item in objects_list:
        assert item.name == name


@allure.testcase(TASK_LINK.format("VHAT-1587"), "VHAT-1587")
@pytest.mark.parametrize('wildcard_pattern', data(path, 'test_data_wildcard'))
def test_get_objects_data_by_pattern_wildcard(wildcard_pattern):
    objects_list = get_objects_data_by_pattern(Wildcard(wildcard_pattern))
    check_wildcard_results_content(objects_list, wildcard_pattern)


@allure.issue(TASK_LINK.format("VHAT-1698"), "VHAT-1698")
@allure.testcase(TASK_LINK.format("VHAT-1588"), "VHAT-1588")
def test_empty_list_of_objects_name():
    objects_list = get_objects_data_by_pattern('')
    assert not len(objects_list)


@allure.testcase(TASK_LINK.format("VHAT-1700"), "VHAT-1700")
@pytest.mark.parametrize('incorrect_data', [
    # none data
    None,
    # several objects
    (),
    1,
    True
])
def test_exception_with_unexpected_data(app_connector, incorrect_data):
    with pytest.raises(TypeError):
        get_objects_data_by_pattern(incorrect_data)
