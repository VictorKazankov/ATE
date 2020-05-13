import pytest
from functional_tests.tests.import_helper_functions import Data
from functional_tests.utils.report import jira_test

path = 'test_integration'


@jira_test("VDP-1234")
@jira_test("VDP-1231")
@pytest.mark.integration
def test_integration():
    Data(path).open_page()
