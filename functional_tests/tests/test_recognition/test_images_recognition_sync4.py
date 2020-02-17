import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils.sync4.constants import (TASK_LINK,
                                                    apps_page_images,
                                                    audio_page_images,
                                                    audio_sources_page_images,
                                                    key_numbers_images,
                                                    main_panel_images,
                                                    settings_page_images, Icons)


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


# Apps page
@allure.testcase(TASK_LINK.format("VHAT-1405"), "VHAT-1405")
@pytest.mark.image_recognition
@pytest.mark.parametrize("testdata", apps_page_images)
def test_apps_page_images(apps_sync4, testdata):
    assert get_exist_result(testdata)


# Settings page
@allure.testcase(TASK_LINK.format("VHAT-1405"), "VHAT-1405")
@pytest.mark.image_recognition
@pytest.mark.parametrize("testdata", settings_page_images)
def test_settings_page_images(settings_sync4, testdata):
    if testdata == Icons.SETIINGS_911_ASSIST_BUTTON:
        helpers.touch_and_drag_page('', x=1000, y=600, dx=-700, dy=0)
    assert get_exist_result(testdata)
    if testdata == Icons.SETTINGS_TOWING_BUTTON:
        helpers.touch_and_drag_page('', x=100, y=600, dx=700, dy=0)
