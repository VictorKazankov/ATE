import logging

import pytest
from functional_tests.pages import hmi
from functional_tests.pages.sync3 import page_supervisor_sync3
from functional_tests.pages.sync4 import page_supervisor_sync4
from functional_tests.utils import ssh_connect, wait_for_obj_benchmark

CURRENT_SYNC_VERSION = r"sed -n -e '/Target/ s/.*\= *//p' /etc/vdp/ate_server.ini"
CURRENT_BUILD_VERSION = r"sed -n -e '/Build/ s/.*\= *//p' /etc/vdp/ate_server.ini"
START = 'sudo systemctl start vdp_ate_server'
STOP = 'sudo systemctl stop vdp_ate_server'
RESTART = 'sudo systemctl restart vdp_ate_server'

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
def get_current_sync_build_version():
    # get version sync from config file
    client = ssh_connect.start()
    stdin, stdout, stderr = client.exec_command(CURRENT_SYNC_VERSION)
    sync = stdout.read()[:-1]
    stdin, stdout, stderr = client.exec_command(CURRENT_BUILD_VERSION)
    build = stdout.read()[:-1]
    yield sync, build
    ssh_connect.execute_command(client, RESTART, passwd_required=True)


@pytest.fixture()
def add_night_mode(get_current_sync_build_version):
    client = ssh_connect.start()
    current_sync, current_build = get_current_sync_build_version
    path_icon_storage = '/var/lib/vdp/vhat/icon_storage/{}/{}'.format(current_sync, current_build)
    client.exec_command("mkdir {}/night_mode/ && touch {}/night_mode/test.png && touch {}/night_mode.json"
                        .format(path_icon_storage, path_icon_storage, path_icon_storage))
    yield
    client.exec_command("rm -r {}/night_mode && rm {}/night_mode.json".format(path_icon_storage, path_icon_storage))
    ssh_connect.execute_command(client, RESTART, passwd_required=True)


@pytest.fixture(autouse=True)
def logger():
    logger = logging.getLogger('Test Log')
    logger.setLevel(logging.INFO)

    return logger
