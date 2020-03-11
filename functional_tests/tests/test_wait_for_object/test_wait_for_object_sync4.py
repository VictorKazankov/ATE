import allure
import pytest
import vhat_client

from functional_tests.pages import hmi
from functional_tests.utils.sync4.constants import TASK_LINK, Icons

pytestmark = pytest.mark.regression_sync4


@allure.testcase(TASK_LINK.format("VHAT-1599"), "VHAT-1599")
@pytest.mark.parametrize('string_data', [
    [Icons.MAIN_AUDIO_BUTTON_INACTIVE],
    # object to wait, timeout milliseconds for object to be found
    [Icons.MAIN_AUDIO_BUTTON_INACTIVE, 10000]
])
def test_wait_for_object_by_string_data(app_connector, string_data):
    obj = hmi.wait_for_object(*string_data)
    assert isinstance(obj, object)


def create_vhat_object():
    # create vhat object for Audio button on Main panel
    test_object = vhat_client.object()
    test_object.name = Icons.MAIN_AUDIO_BUTTON_INACTIVE
    return test_object


@allure.testcase(TASK_LINK.format("VHAT-1601"), "VHAT-1601")
@pytest.mark.parametrize('object_data', [
    [create_vhat_object()],
    [create_vhat_object(), 10000]
])
def test_wait_for_object_by_object_data(app_connector, object_data):
    obj = hmi.wait_for_object(*object_data)
    assert isinstance(obj, object)


@allure.testcase(TASK_LINK.format("VHAT-1602"), "VHAT-1602")
@pytest.mark.parametrize('exception_data', [
    ('incorrect_string', vhat_client.LookupError),
    (None, TypeError)
])
def test_wait_for_object_exceptions(app_connector, exception_data):
    data, error = exception_data
    with pytest.raises(error):
        hmi.wait_for_object(data)
