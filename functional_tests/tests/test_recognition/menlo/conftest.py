import pytest
from functional_tests.pages.menlo import page_supervisor_menlo


@pytest.fixture(scope='module')
def apps_menlo(app_connector):
    api = page_supervisor_menlo.PageSupervisor()
    api.apps.open_apps_page()
    yield api
    api.apps.open_apps_page()


@pytest.fixture(scope='module')
def settings_general_page_menlo(app_connector):
    api = page_supervisor_menlo.PageSupervisor()
    api.settings_page.open_settings_page()
    yield api


@pytest.fixture(scope='module')
def settings_clock_menlo(settings_general_page_menlo):
    api = page_supervisor_menlo.PageSupervisor()
    api.settings_page.open_settings_clock_page()
