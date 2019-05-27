import configparser
import logging

import pytest
from functional_tests import ate
from functional_tests.pages import page_supervisor

pytest_plugins = "functional_tests.utils.logger"


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
    api = page_supervisor.PageSupervisor()

    if not api.home_page.home_page_is_active():
        api.home_page.open_home_page()
    yield api
    api.home_page.open_home_page()


@pytest.fixture(autouse=True)
def logger():
    logger = logging.getLogger('Test Log')
    logger.setLevel(logging.INFO)

    return logger
