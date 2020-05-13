import pytest
from functional_tests.pages.hmi import get_images_discrepancy
from functional_tests.utils.report import jira_issue, jira_test
from vhat_client import (ComparingImageIncorrectSize, ComparingImageNotExist,
                         EmptyScreenshotFileName, InvalidRectangleCoordinates,
                         ScreenPoint, UnsupportedFileType)

pytestmark = [pytest.mark.imagesDiscrepancy, pytest.mark.no_sync_dependence,
              pytest.mark.regression_sync4, pytest.mark.regression_sync3]


folder_name = 'screenshots'
prepared_image_name1 = 'image1.png'
prepared_image_name2 = 'image2.png'  # NB: image should be totally different (compare with 1st)


def get_path_for_images(image1=prepared_image_name1, image2=prepared_image_name2):
    path_1 = '{}/{}'.format(folder_name, image1)
    path_2 = '{}/{}'.format(folder_name, image2)
    return path_1, path_2


@jira_issue("VHAT-1923", description="Issue for testset #8")
@jira_test("VHAT-1922")
@pytest.mark.parametrize('test_data', [
    # image1, image2, left_point, right_point, percentage
    (prepared_image_name1, prepared_image_name1, ScreenPoint(0, 0), ScreenPoint(0, 0), 0),
    (prepared_image_name1, 'image4.png', ScreenPoint(0, 0), ScreenPoint(0, 0), 25),
    (prepared_image_name1, 'image3.png', ScreenPoint(0, 0), ScreenPoint(0, 0), 45),
    (prepared_image_name1, 'image5.png', ScreenPoint(0, 0), ScreenPoint(0, 0), 50),
    (prepared_image_name1, 'image6.png', ScreenPoint(0, 0), ScreenPoint(0, 0), 75),
    (prepared_image_name1, 'image2.png', ScreenPoint(0, 0), ScreenPoint(0, 0), 100),
    (prepared_image_name1, prepared_image_name1, ScreenPoint(0, 0), ScreenPoint(10, 10), 0),
    (prepared_image_name1, prepared_image_name2, ScreenPoint(0, 0), ScreenPoint(10, 10), 100),
    (prepared_image_name1, 'image3.png', ScreenPoint(0, 0), ScreenPoint(50, 100), 90),
])
def test_comparison_pictures(copy_test_files_to_board, test_data):
    image1, image2, left_point, right_point, expected_percentage = test_data
    images_list = copy_test_files_to_board
    # file validation
    if image1 not in images_list or image2 not in images_list:
        raise ValueError(
            'Test images do not contain provided test files: [{} | {}]\n Possible list is {}'.format(image1, image2,
                                                                                                     images_list))

    path_1, path_2 = get_path_for_images(image1, image2)
    discrepancy_percentage = get_images_discrepancy(path_1, path_2, left_point, right_point)

    assert discrepancy_percentage == expected_percentage


# positive tests
@jira_test("VHAT-1652")
def test_comparison_different_whole_screenshots(copy_test_files_to_board):
    path_1, path_2 = get_path_for_images()
    discrepancy_percentage = get_images_discrepancy(path_1, path_2)
    assert 0 < discrepancy_percentage <= 100


@jira_test("VHAT-1653")
def test_comparison_same_whole_screenshots(copy_test_files_to_board):
    path_1, path_2 = get_path_for_images()
    # add equal images to compare
    discrepancy_percentage = get_images_discrepancy(path_1, path_1)
    assert discrepancy_percentage == 0


@jira_test("VHAT-1654")
def test_comparison_different_part_screenshots(copy_test_files_to_board):
    path_1, path_2 = get_path_for_images()
    discrepancy_percentage = get_images_discrepancy(path_1, path_2, ScreenPoint(10, 10), ScreenPoint(50, 50))
    assert 0 < discrepancy_percentage <= 100


@jira_test("VHAT-1655")
def test_comparison_same_part_screenshots(copy_test_files_to_board):
    path_1, path_2 = get_path_for_images()
    # add equal images to compare
    discrepancy_percentage = get_images_discrepancy(path_1, path_1, ScreenPoint(10, 10), ScreenPoint(50, 50))
    assert discrepancy_percentage == 0


@jira_test("VHAT-1666")
def test_comparison_different_whole_screenshots_if_zero_coordinates(copy_test_files_to_board):
    path_1, path_2 = get_path_for_images()
    # add equal images to compare
    discrepancy_percentage = get_images_discrepancy(path_1, path_2, ScreenPoint(0, 0), ScreenPoint(0, 0))
    assert 0 < discrepancy_percentage <= 100


# negative tests
@jira_test("VHAT-1656")
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
def test_get_image_discrepancy_coordinates_exceptions(copy_test_files_to_board, test_data):
    x1, y1, x2, y2, error = test_data
    path_1, path_2 = get_path_for_images()
    with pytest.raises(error):
        # set incorrect coordinates values
        get_images_discrepancy(path_1, path_1, ScreenPoint(x1, y1), ScreenPoint(x2, y2))


@jira_test("VHAT-1664")
def test_exception_not_exist_image(app_connector):
    incorrect_path = 'incorrect_folder/incorrect_file.png'
    with pytest.raises(ComparingImageNotExist):
        get_images_discrepancy(incorrect_path, incorrect_path)


@jira_test("VHAT-1665")
def test_exceptionfor_file_zero_size(create_zero_size_file):
    path = create_zero_size_file
    with pytest.raises(ComparingImageIncorrectSize):
        get_images_discrepancy(path, path)


@jira_test("VHAT-1666")
def test_unsupport_file(create_unsupport_file):
    path = create_unsupport_file
    with pytest.raises(UnsupportedFileType):
        get_images_discrepancy(path, path)


@jira_test("VHAT-1667")
def test_exception_empty_path_to_file(app_connector):
    with pytest.raises(EmptyScreenshotFileName):
        get_images_discrepancy('', '')
