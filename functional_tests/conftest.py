import configparser
import logging

import pytest
from functional_tests import api


@pytest.fixture(scope='session')
def config():
    configuration = configparser.ConfigParser()
    configuration.read('test_config.ini')
    return configuration


# TODO: add attach to application import
@pytest.fixture(scope='session', autouse=True)
def app_connector(config):
    sync_config = config["sync"]
    sync = attachToApplication(
        sync_config['name'], host=sync_config['host'], port=sync_config['port'])
    try:
        yield sync
    finally:
        sync.detach()


@pytest.fixture(scope='module')
def driver():
    return api


@pytest.fixture(autouse=True)
def logger():
    logger = logging.getLogger('Test Log')
    logger.setLevel(logging.INFO)

    return logger
