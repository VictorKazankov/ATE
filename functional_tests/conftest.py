import logging

import pytest
from functional_tests.pages import hmi
from functional_tests.pages.sync3 import page_supervisor_sync3
from functional_tests.pages.sync4 import page_supervisor_sync4
from functional_tests.utils import ssh_connect, wait_for_obj_benchmark

CURRENT_SYNC_VERSION = r"sed -n -e '/Target/ s/.*\= *//p' /usr/etc/vdp/vhat_server.ini"
CURRENT_BUILD_VERSION = r"sed -n -e '/Build/ s/.*\= *//p' /usr/etc/vdp/vhat_server.ini"
START = 'sudo systemctl start vhat_server'
STOP = 'sudo systemctl stop vhat_server'

pytest_plugins = "functional_tests.utils.logger"


@pytest.fixture(scope='session')
def app_connector():
    sync = hmi.attach_to_application()
    try:
        yield sync
    finally:
        logging.info('Average recognition time for images: {}'.format(wait_for_obj_benchmark.get_image_average_time()))
        logging.info('Average recognition time for text: {}'.format(wait_for_obj_benchmark.get_text_average_time()))


@pytest.fixture(scope='module')
def driver_sync3(app_connector):
    api = page_supervisor_sync3.PageSupervisor()
    if not api.home_page.home_page_is_active():
        api.home_page.open_home_page()
    yield api
    api.home_page.open_home_page()
    api.home_page.switch_to_fm_sources_on_home_page()


@pytest.fixture(scope='module')
def driver_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.phone_page.open_phone_page()
    yield api
    api.phone_page.open_phone_page()


@pytest.fixture(scope='module')
def get_current_sync_build_version(app_connector):
    # get version sync from config file
    client = ssh_connect.start()
    stdin, stdout, stderr = client.exec_command(CURRENT_SYNC_VERSION)
    sync = stdout.read()[:-1]
    stdin, stdout, stderr = client.exec_command(CURRENT_BUILD_VERSION)
    build = stdout.read()[:-1]
    yield sync, build
    ssh_connect.execute_command(client, STOP, passwd_required=True)
    ssh_connect.execute_command(client, START, passwd_required=True)


@pytest.fixture(autouse=True)
def logger():
    logger = logging.getLogger('Test Log')
    logger.setLevel(logging.INFO)

    return logger
