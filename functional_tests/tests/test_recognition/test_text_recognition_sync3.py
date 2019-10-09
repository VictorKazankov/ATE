import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import (TASK_LINK,
                                                    audio_direct_tune_text,
                                                    audio_page_text,
                                                    audio_sirius_page_text,
                                                    audio_sources_page_text,
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
