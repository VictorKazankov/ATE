import allure
import pytest
from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.sync4.constants import (TASK_LINK,
                                                    audio_page_images,
                                                    audio_sources_page_images,
                                                    key_numbers_images,
                                                    main_panel_images)


# Main panel
@allure.testcase(TASK_LINK.format("VHAT-1285"), "VHAT-1285")
@pytest.mark.image_recognition
@pytest.mark.parametrize("testdata", main_panel_images)
def test_main_panel_images(driver_sync4, testdata):
    assert get_exist_result(testdata)


# Audio page
@allure.testcase(TASK_LINK.format("VHAT-1285"), "VHAT-1285")
@pytest.mark.image_recognition
@pytest.mark.parametrize("testdata", audio_page_images)
def test_audio_page_images(audio_sync4, testdata):
    assert get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-1285"), "VHAT-1285")
@pytest.mark.image_recognition
@pytest.mark.parametrize("testdata", audio_sources_page_images)
def test_audio_sources_page_images(audio_sync4, testdata):
    audio_sync4.tap_on_sources_page()
    assert get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-1285"), "VHAT-1285")
@pytest.mark.image_recognition
@pytest.mark.parametrize("testdata", key_numbers_images)
def test_direct_tune_page_images(audio_sync4, testdata):
    audio_sync4.tap_on_direct_tune_button()
    assert get_exist_result(testdata)
    # tune page closes after verify last elements
    if testdata == key_numbers_images[-1]:
        audio_sync4.tap_on_cancel_button()
