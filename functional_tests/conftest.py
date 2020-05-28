import logging

import pytest

from functional_tests import params_handler
from functional_tests.pages import hmi
from functional_tests.pages.sync3 import page_supervisor_sync3
from functional_tests.pages.sync4 import page_supervisor_sync4
from functional_tests.utils import get_benchmark
from functional_tests.utils import ssh_commands as cmd
from functional_tests.utils import ssh_connect
from functional_tests.utils.get_benchmark import attach_stats_file_to_allure, save_stat_to_file
from functional_tests.utils.server_setup import EnvSetup

CURRENT_SYNC_VERSION = r"sed -n -e '/Target/ s/.*\= *//p' /etc/vdp/ate_server.ini"
CURRENT_BUILD_VERSION = r"sed -n -e '/Build/ s/.*\= *//p' /etc/vdp/ate_server.ini"

pytest_plugins = "functional_tests.utils.logger"


def pytest_addoption(parser):
    for key in params_handler.support_keys.keys():
        parser.addoption(key, action="store", default=None)


@pytest.fixture(scope='session')
def getting_params(pytestconfig, app_connector):
    params = dict()
    params_handler.__HOST = app_connector.host
    logging.info('Connected to board [{}]'.format(params_handler.get_host()))

    for key, value in params_handler.support_keys.items():
        data = pytestconfig.getoption(key)
        params[value] = data

    res = params_handler.get_config_for_params(params)
    logging.info('Provided data for sync [{}]'.format(params_handler.get_sync()))
    logging.info('Params: {}'.format(params))

    yield res


@pytest.fixture(scope='session')
def setup_environment(getting_params):
    config, text_to_log = getting_params
    logging.info(text_to_log)
    try:
        obj = EnvSetup(config)
        obj.apply_new_config()
        yield obj
    except Exception as e:
        logging.info('Something was wrong! ')
        logging.exception(e)


@pytest.fixture(scope='session')
def driver_sync(setup_environment):
    # TODO start using auto config for autotests will be done under VHAT-1845
    logging.info('Getting appropriate sync driver [{}]'.format(params_handler.__SYNC))
    if params_handler.__SYNC == 'sync3':
        api = page_supervisor_sync3.PageSupervisor()
        yield api
    elif params_handler.__SYNC == 'sync4':
        api = page_supervisor_sync4.PageSupervisor()
        yield api
    else:
        raise EnvironmentError('Wrong sync version')


@pytest.fixture(scope='session', autouse=True)
def attach_to_app():
    sync = hmi.attach_to_application()
    # TODO delete line below after implementation of VHAT-1845
    params_handler.__HOST = sync.host
    yield sync


@pytest.fixture(scope='session')
def app_connector(attach_to_app):
    try:
        yield attach_to_app
    finally:
        logging.info('Average recognition time for images: {}'.format(get_benchmark.get_image_average_time()))
        logging.info('Average recognition time for text: {}'.format(get_benchmark.get_text_average_time()))
        attach_stats_file_to_allure(save_stat_to_file())


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
    api.climate_page.open_climate_page()
    yield api
    api.climate_page.open_climate_page()


@pytest.fixture(scope='module')
def get_current_sync_build_version(app_connector):
    # get version sync from config file
    client = ssh_connect.start()
    stdin, stdout, stderr = client.exec_command(CURRENT_SYNC_VERSION)
    sync = stdout.read()[:-1]
    stdin, stdout, stderr = client.exec_command(CURRENT_BUILD_VERSION)
    build = stdout.read()[:-1]
    yield sync, build
    ssh_connect.execute_command(client, cmd.VDP_ATE_SERVER_RESTART, passwd_required=True)


@pytest.fixture(autouse=True)
def logger():
    logger = logging.getLogger('Test Log')
    logger.setLevel(logging.INFO)

    return logger
