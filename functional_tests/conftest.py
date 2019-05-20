import configparser
import logging

import pytest
from functional_tests import ate
from functional_tests.pages.hmi import HMI


@pytest.fixture(scope='session')
def config():
    configuration = configparser.ConfigParser()
    configuration.read('test_config.ini')
    return configuration


# TODO: add attach to application import
@pytest.fixture(scope='session', autouse=True)
def app_connector(config):
    sync_config = config["sync"]
    sync = ate.attachToApplication(
        app=sync_config['name'],
        host=sync_config['host'],
        port=sync_config['port'])
    try:
        yield sync
    finally:
        ate.detach()


@pytest.fixture(scope='module')
def driver():
    hmi = HMI()
    if not hmi.home_page.verify_action_home_page():
        hmi.home_page.open_home_page()
    yield hmi
    hmi.home_page.open_home_page()


@pytest.fixture(autouse=True)
def logger():
    logger = logging.getLogger('Test Log')
    logger.setLevel(logging.INFO)

    return logger
