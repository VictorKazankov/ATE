import logging

import pytest
from functional_tests.pages import hmi
from functional_tests.pages.sync3 import page_supervisor_sync3
from functional_tests.pages.sync4 import page_supervisor_sync4
from functional_tests.utils import wait_for_obj_benchmark

pytest_plugins = "functional_tests.utils.logger"


@pytest.fixture(scope='module')
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
    if not api.phone_page.phone_page_is_active():
        api.phone_page.open_phone_page()
    yield api
    if not api.phone_page.phone_page_is_active():
        api.phone_page.open_phone_page()


@pytest.fixture(autouse=True)
def logger():
    logger = logging.getLogger('Test Log')
    logger.setLevel(logging.INFO)

    return logger
