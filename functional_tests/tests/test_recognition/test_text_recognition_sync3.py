import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import (TASK_LINK, Text,
                                                    a_c_controls_page_text,
                                                    audio_direct_tune_text,
                                                    audio_page_text,
                                                    audio_sirius_page_text,
                                                    audio_sources_page_text,
                                                    climate_page_text,
                                                    defrost_controls_page_text,
                                                    home_page_text)


# Home page
@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.parametrize('testdata', home_page_text)
@pytest.mark.smoke_text_recognition
def test_home_page_text(driver_sync3, testdata):
    assert helpers.get_exist_result(testdata)


# Audio page
@allure.testcase(TASK_LINK.format("VHAT-576"), "VHAT-576")
@pytest.mark.parametrize('testdata', audio_page_text)
@pytest.mark.smoke_text_recognition
def test_audio_page_texts(audio_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-576"), "VHAT-576")
@pytest.mark.parametrize('testdata', audio_direct_tune_text)
@pytest.mark.smoke_text_recognition
def test_audio_direct_tune_texts(audio_direct_tune_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-576"), "VHAT-576")
@pytest.mark.parametrize('testdata', audio_sources_page_text)
@pytest.mark.smoke_text_recognition
def test_audio_sources_page_texts(audio_sources_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-576"), "VHAT-576")
@pytest.mark.parametrize('testdata', audio_sirius_page_text)
@pytest.mark.smoke_text_recognition
def test_audio_sirius_page_texts(audio_sirius_sync3, testdata):
    assert helpers.get_exist_result(testdata)


# Climate page
@allure.testcase(TASK_LINK.format("VHAT-585"), "VHAT-585")
@pytest.mark.parametrize('testdata', climate_page_text)
@pytest.mark.smoke_text_recognition
def test_climate_page_text(climate_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-585"), "VHAT-585")
@pytest.mark.parametrize('testdata', defrost_controls_page_text)
@pytest.mark.smoke_text_recognition
def test_defrost_controls_page_text(climate_defrost_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-585"), "VHAT-585")
@pytest.mark.parametrize('testdata', a_c_controls_page_text)
@pytest.mark.smoke_text_recognition
def test_a_c_controls_page_text(climate_a_c_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-587"), "VHAT-587")
@pytest.mark.parametrize('testdata', [Text.NAVIGATION_FAULT_TEXT, Text.NAVIGATION_FAULT_DESCRIPTION_TEXT])
@pytest.mark.smoke_text_recognition
def test_navigation_dialog_text(driver_sync3, testdata):
    driver_sync3.navigation_page.open_navigation_page()
    result = helpers.get_exist_result(testdata)
    driver_sync3.navigation_page.close_information_dialog()
    assert result
