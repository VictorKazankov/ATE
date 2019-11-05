import allure
import pytest
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import TASK_LINK, Icons, Text

pytestmark = pytest.mark.regression_sync3


@allure.testcase(TASK_LINK.format("VHAT-959"), "VHAT-959")
def test_exist_home_icon(driver_sync3):
    assert helpers.get_exist_result(Icons.HOME_PAGE_ICON)


@allure.testcase(TASK_LINK.format("VHAT-960"), "VHAT-960")
def test_exist_navigation_fault_text(driver_sync3):
    assert helpers.get_exist_result(Text.NAVIGATION_FAULT_TEXT)
