import pytest

from functional_tests.pages.menlo.page_supervisor_menlo import PageSupervisor


@pytest.fixture(scope='session')
def menlo(app_connector):
    yield PageSupervisor()


@pytest.fixture(scope='class')
def apps_menlo(menlo):
    page = menlo.apps
    yield page.open()
    page.close()


@pytest.fixture(scope='module')
def settings_menlo(menlo):
    page = menlo.settings_page
    yield page.open()
    page.close()


@pytest.fixture(scope='class')
def settings_sound_menlo(settings_menlo):
    yield settings_menlo.sound.open()


@pytest.fixture(scope='class')
def settings_clock_menlo(settings_menlo):
    yield settings_menlo.clock.open()


@pytest.fixture(scope='class')
def settings_phone_list_menlo(settings_menlo):
    yield settings_menlo.phone_list.open()


@pytest.fixture(scope='class')
def settings_connectivity_menlo(settings_menlo):
    yield settings_menlo.connectivity.open()


@pytest.fixture(scope='class')
def settings_display_menlo(settings_menlo):
    yield settings_menlo.display.open()
