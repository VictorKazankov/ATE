import logging

import allure
import pytest
from functional_tests.pages import hmi
from functional_tests.utils.sync3.constants import TASK_LINK
from vhat_client import CollectionMode, InvalidSyncCollectionMode


@allure.testcase(TASK_LINK.format("VHAT-697"), "VHAT-697")
def test_invalid_sync_collection_mode(app_connector):
    with pytest.raises(InvalidSyncCollectionMode):
        hmi.change_sync_mode(CollectionMode.NIGHT)
    logging.info("Invalid sync collection mode exception is catched")


@allure.testcase(TASK_LINK.format("VHAT-697"), "VHAT-697")
def test_correct_collection_mode(add_night_mode):
    try:
        hmi.change_sync_mode(CollectionMode.NIGHT)
        logging.info("Night collection mode installed successfully")
    except InvalidSyncCollectionMode:
        logging.info("Night collection mode install failled")
