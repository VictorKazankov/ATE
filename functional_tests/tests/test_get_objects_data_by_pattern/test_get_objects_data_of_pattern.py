import allure
import vhat_client
from functional_tests.pages import hmi
from functional_tests.utils.sync3.constants import TASK_LINK, Icons


@allure.testcase(TASK_LINK.format("VHAT-1377"), "VHAT-1377")
def test_get_object_of_pattern_from_name(app_connector):
    objects_list = hmi.get_object_data_pattern(Icons.BACK_BUTTON)
    assert objects_list[0].name == Icons.BACK_BUTTON


@allure.testcase(TASK_LINK.format("VHAT-1377"), "VHAT-1377")
def test_get_object_of_pattern_from_object(app_connector):
    test_object = create_vhat_object()
    objects_list = hmi.get_object_data_pattern(test_object)
    assert objects_list[0].name == Icons.MAIN_AUDIO_BUTTON_INACTIVE


@allure.testcase(TASK_LINK.format("VHAT-1377"), "VHAT-1377")
def test_get_empty_list_of_objects_from_name(app_connector):
    objects_list = hmi.get_object_data_pattern('')
    assert not len(objects_list)


# TODO:: uncomment once invalid argument exeption is done
# @allure.testcase(TASK_LINK.format("VHAT-1377"), "VHAT-1377")
# @pytest.mark.parametrize('incorrect_data', [
#     # none data
#     None,
#     # several objects
#     (),
#     1,
#     True
# ])
# def test_exception_with_none_data(app_connector, incorrect_data):
#     with pytest.raises(TypeError):
#         hmi.get_object_data_pattern(incorrect_data)

def create_vhat_object():
    test_object = vhat_client.object()
    test_object.name = Icons.MAIN_AUDIO_BUTTON_INACTIVE
    return test_object
