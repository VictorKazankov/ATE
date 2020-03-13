import functools

import allure
import pytest
from functional_tests.pages.hmi import get_images_discrepancy
from functional_tests.pages.sync3.pages import AudioPage, ClimatePage
from functional_tests.tests.test_images_discrepancy.helpers import (
    get_path_screenshots, set_point)
from functional_tests.utils.sync4.constants import TASK_LINK
from vhat_client import (ComparingImageIncorrectSize, ComparingImageNotExist,
                         EmptyScreenshotFileName, InvalidRectangleCoordinates,
                         UnsupportedFileType)

pytestmark = [pytest.mark.get_images_discrepancy_api, pytest.mark.regression_sync3]

get_path_screenshots = functools.partial(get_path_screenshots,
                                         audio_page_open=AudioPage().open_audio_page,
                                         climate_page_open=ClimatePage().open_climate_page)


# TODO:: add check correct calculation of discrepancy_percentage using for it hardcore images

# positive tests
@allure.testcase(TASK_LINK.format("VHAT-1652"), "VHAT-1652")
def test_comparison_different_whole_screenshots(create_temporary_folder):
    path_1, path_2 = get_path_screenshots()
    discrepancy_percentage = get_images_discrepancy(path_1, path_2)
    assert 0 < discrepancy_percentage <= 100


@allure.testcase(TASK_LINK.format("VHAT-1653"), "VHAT-1653")
def test_comparison_same_whole_screenshots(create_temporary_folder):
    path_1, path_2 = get_path_screenshots()
    # add equal images to compare
    discrepancy_percentage = get_images_discrepancy(path_1, path_1)
    assert discrepancy_percentage == 0


@allure.testcase(TASK_LINK.format("VHAT-1654"), "VHAT-1654")
def test_comparison_different_part_screenshots(create_temporary_folder):
    path_1, path_2 = get_path_screenshots()
    discrepancy_percentage = get_images_discrepancy(path_1, path_2, set_point(10, 10), set_point(300, 300))
    assert 0 < discrepancy_percentage <= 100


@allure.testcase(TASK_LINK.format("VHAT-1655"), "VHAT-1655")
def test_comparison_same_part_screenshots(create_temporary_folder):
    path_1, path_2 = get_path_screenshots()
    # add equal images to compare
    discrepancy_percentage = get_images_discrepancy(path_1, path_1, set_point(10, 10), set_point(300, 300))
    assert discrepancy_percentage == 0


@allure.testcase(TASK_LINK.format("VHAT-1666"), "VHAT-1666")
def test_comparison_different_whole_screenshots_if_zero_coordinates(create_temporary_folder):
    path_1, path_2 = get_path_screenshots()
    # add equal images to compare
    discrepancy_percentage = get_images_discrepancy(path_1, path_2, set_point(0, 0), set_point(0, 0))
    assert 0 < discrepancy_percentage <= 100


# negative tests
# # TODO:: need added images for tests as hardcode
@allure.testcase(TASK_LINK.format("VHAT-1656"), "VHAT-1656")
@pytest.mark.parametrize('test_data', [
    # border overseeded
    (10000, 10000, 20000, 20000, InvalidRectangleCoordinates),
    # negative coordinates
    (-1, -10, -1, -10, InvalidRectangleCoordinates),
    # zero height
    (10, 50, 15, 50, InvalidRectangleCoordinates),
    # zero width
    (10, 50, 10, 55, InvalidRectangleCoordinates),
    # top left lower than right bottom
    (50, 10, 10, 50, InvalidRectangleCoordinates),
    ('str', -10, -1, -10, TypeError),
    (None, 10, 50, 50, TypeError),
    (10.001, 10, 50, 50, TypeError)
])
def test_get_image_discrepancy_coordinates_exceptions(create_temporary_folder, test_data):
    x1, y1, x2, y2, error = test_data
    path_1, path_2 = get_path_screenshots()
    with pytest.raises(error):
        # set incorrect coordinates values
        get_images_discrepancy(path_1, path_2, set_point(x1, y1), set_point(x2, y2))


@allure.testcase(TASK_LINK.format("VHAT-1664"), "VHAT-1664")
def test_exception_not_exist_image(app_connector):
    incorrect_path = 'incorrect_folder/incorrect_file.png'
    with pytest.raises(ComparingImageNotExist):
        get_images_discrepancy(incorrect_path, incorrect_path)


@allure.testcase(TASK_LINK.format("VHAT-1665"), "VHAT-1665")
def test_exceptionfor_file_zero_size(create_zero_size_file):
    path = create_zero_size_file
    with pytest.raises(ComparingImageIncorrectSize):
        get_images_discrepancy(path, path)


@allure.testcase(TASK_LINK.format("VHAT-1666"), "VHAT-1666")
def test_unsupport_file(create_unsupport_file):
    path = create_unsupport_file
    with pytest.raises(UnsupportedFileType):
        get_images_discrepancy(path, path)


@allure.testcase(TASK_LINK.format("VHAT-1667"), "VHAT-1667")
def test_exception_empty_path_to_file(app_connector):
    with pytest.raises(EmptyScreenshotFileName):
        get_images_discrepancy('', '')
