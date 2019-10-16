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
                                                    home_page_text,
                                                    mobile_apps_page_texts,
                                                    pair_bluetooth_device_page,
                                                    siriusxm_travel_link_texts,
                                                    subscription_info_texts)


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


# Navigation page
@allure.testcase(TASK_LINK.format("VHAT-587"), "VHAT-587")
@pytest.mark.parametrize('testdata', [Text.NAVIGATION_FAULT_TEXT, Text.NAVIGATION_FAULT_DESCRIPTION_TEXT])
@pytest.mark.smoke_text_recognition
def test_navigation_dialog_text(driver_sync3, testdata):
    driver_sync3.navigation_page.open_navigation_page()
    result = helpers.get_exist_result(testdata)
    driver_sync3.navigation_page.close_information_dialog()
    assert result


# Phone page
@allure.testcase(TASK_LINK.format("VHAT-586"), "VHAT-586")
@pytest.mark.smoke_text_recognition
def test_phone_page_text(phone_sync3):
    assert helpers.get_exist_result(Text.PAIR_PHONE_TEXT)


@allure.testcase(TASK_LINK.format("VHAT-586"), "VHAT-586")
@pytest.mark.parametrize('testdata', pair_bluetooth_device_page)
@pytest.mark.smoke_text_recognition
def test_phone_pair_page_text(phone_pair_sync3, testdata):
    assert helpers.get_exist_result(testdata)


# Mobile Apps
@allure.testcase(TASK_LINK.format("VHAT-588"), "VHAT-588")
@pytest.mark.parametrize('testdata', mobile_apps_page_texts)
@pytest.mark.smoke_text_recognition
def test_text_on_mobile_apps_page(mobile_apps_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-588"), "VHAT-588")
@pytest.mark.parametrize('testdata', siriusxm_travel_link_texts)
@pytest.mark.smoke_text_recognition
def test_text_siriusxm_travel_link_page(mobile_apps_siriusxm_travel_link_sync3, testdata):
    assert helpers.get_exist_result(testdata)


@allure.testcase(TASK_LINK.format("VHAT-588"), "VHAT-588")
@pytest.mark.parametrize('testdata', subscription_info_texts)
@pytest.mark.smoke_text_recognition
def test_text_subscription_info_page(mobile_apps_subscription_info_sync3, testdata):
    assert helpers.get_exist_result(testdata)
