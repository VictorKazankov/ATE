import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import TASK_LINK, home_page_text


@allure.testcase(TASK_LINK.format("VHAT-546"), "VHAT-546")
@pytest.mark.parametrize('testdata', home_page_text)
@pytest.mark.smoke_text_recognition
def test_home_page_text(driver_sync3, testdata):
    assert helpers.get_exist_result(testdata)
