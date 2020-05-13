import logging

import allure
import pytest
from functional_tests.pages import hmi
from functional_tests.tests.import_helper_functions import Data
from functional_tests.utils.report import jira_test
from vhat_client import LookupError

pytestmark = [pytest.mark.regression_sync4, pytest.mark.regression_sync3, pytest.mark.recognition_100_times]

path = "test_recognition_100_times"


@jira_test("VHAT-276")
@pytest.mark.image_recognition
def test_icon_recognition_100_times():
    RecognitionCounter(Data(path).icon).recognize_in_loop().check_result()


@jira_test("VHAT-347")
@pytest.mark.text_recognition
def test_text_recognition_100_times():
    RecognitionCounter(Data(path).text).recognize_in_loop().check_result()


@pytest.fixture(scope="function", autouse=True)
def open_page(attach_to_app):
    Data(path).open_page()


class RecognitionCounter(object):
    ATTEMPTS = 100
    PERCENT = 98

    def __init__(self, name):
        self.name = name
        self.success = 0
        self.fail = 0

    def recognize_in_loop(self):
        for i in range(self.ATTEMPTS):
            self._try_to_recognize(i + 1, self.ATTEMPTS)
        return self

    @allure.step('"{current}" attempt from "{total}"')
    def _try_to_recognize(self, current, total):
        try:
            hmi.wait_for_object(self.name)
            self.success += 1
            logging.info('{} object recognized {} times'.format(self.name, self.success))
        except LookupError:
            self.fail += 1
            logging.warning('{} object not recognized {} times'.format(self.name, self.fail))

    def check_result(self):
        logging.info("{} recognized successfully {} times".format(self.name, self.success))
        assert (self.success / self.ATTEMPTS * 100) > self.PERCENT, \
            " {} recognition percentage: {}. Expected: {}%".format(self.name, self.success, self.PERCENT)
