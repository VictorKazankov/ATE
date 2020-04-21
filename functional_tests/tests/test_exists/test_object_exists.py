import pytest

from functional_tests.pages.hmi import obj_exists
from functional_tests.tests.import_helper_functions import Data
from functional_tests.utils.report import jira_test

pytestmark = [pytest.mark.regression_sync4, pytest.mark.regression_sync3, pytest.mark.existObjectApi]

path = "test_exists"


@pytest.fixture(scope="function", autouse=True)
def open_page(attach_to_app):
    """
    opens predefined page
    """
    Data(path).open_method()


@jira_test("VHAT-970")
def test_exists_icon():
    """
    icon object().exists() API verification
    """
    assert obj_exists(Data(path).icon)


@jira_test("VHAT-971")
def test_exists_text():
    """
    text object().exists() API verification
    """
    assert obj_exists(Data(path).text)
