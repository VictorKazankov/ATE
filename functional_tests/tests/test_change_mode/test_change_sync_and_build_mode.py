import logging

import allure
import pytest
from functional_tests.pages import hmi
from functional_tests.tests import helpers
from functional_tests.utils.sync3.constants import TASK_LINK
from functional_tests.utils.sync3.constants import Icons as sync3_icon
from functional_tests.utils.sync4.constants import Icons as sync4_icon
from vhat_client import InvalidSyncBuildVersion, InvalidSyncVersion

SYNC3_VERSION = 'sync3'
SYNC4_VERSION = 'sync4'
SYNC3_BUILD_NAME = 'version3.2_build17288_devtest'
SYNC4_BUILD_NAME = 'unknown_build_version'
SYNC_INCORRECT_VERSION = 'sync_incorrect'
SYNC_INCORRECT_BUILD = 'build_incorrect'


@allure.issue(TASK_LINK.format("VHAT-1529"),  "VHAT-1529 for {}".format('sync3'))
@allure.testcase(TASK_LINK.format("VHAT-698"), "VHAT-698")
def test_change_sync_and_build_version(get_current_sync_build_version):
    # getting sync and builds values from config file
    current_sync, current_build = get_current_sync_build_version
    if SYNC3_VERSION in current_sync:
        # change sync3->sync4 and sync_build3->sync_build4
        hmi.change_sync_icon_db(SYNC4_VERSION, SYNC4_BUILD_NAME)
        logging.info("Change mode is successfully done")
        # check that sync3 image isn't recognized
        assert not helpers.get_exist_result(sync3_icon.MAIN_PHONE_BUTTON_ACTIVE)
    elif SYNC4_VERSION in current_sync:
        # change sync4->sync3 and sync_build4->sync_build3
        hmi.change_sync_icon_db(SYNC3_VERSION, SYNC3_BUILD_NAME)
        logging.info("Change mode is successfully done")
        # check that sync4 image isn't recognized
        assert not helpers.get_exist_result(sync4_icon.MAIN_PHONE_BUTTON_INACTIVE)
    else:
        raise Exception('Can`t get value of sync and build version from config file')


@allure.testcase(TASK_LINK.format("VHAT-698"), "VHAT-698")
def test_check_empty_sync_and_build_values(get_current_sync_build_version):
    with pytest.raises(ValueError) as error:
        hmi.change_sync_icon_db("", "")
    logging.info(error.value.args[0])


@allure.testcase(TASK_LINK.format("VHAT-698"), "VHAT-698")
def test_check_incorrect_sync_value(get_current_sync_build_version):
    with pytest.raises(InvalidSyncVersion) as error:
        hmi.change_sync_icon_db(SYNC_INCORRECT_VERSION, SYNC3_BUILD_NAME)
    logging.info(error.value.args[0])


@allure.testcase(TASK_LINK.format("VHAT-698"), "VHAT-698")
def test_check_incorrect_build_value(get_current_sync_build_version):
    with pytest.raises(InvalidSyncBuildVersion) as error:
        hmi.change_sync_icon_db(SYNC3_VERSION, SYNC_INCORRECT_BUILD)
    logging.info(error.value.args[0])
