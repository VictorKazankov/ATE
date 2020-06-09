import pytest

from functional_tests.pages.menlo.page_supervisor_menlo import PageSupervisor


@pytest.fixture(scope='session')
def menlo(app_connector):
    yield PageSupervisor()


@pytest.fixture(scope='class')
def main_menlo(menlo):
    menlo.settings.close()
    menlo.apps.close()


@pytest.fixture(scope='class')
def apps_menlo(menlo):
    page = menlo.apps
    yield page.open()
    page.close()


@pytest.fixture(scope='class')
def tire_pressure_app(menlo):
    menlo.apps.tire_pressure.open()


@pytest.fixture(scope='class')
def trip_iod_menlo(menlo):
    menlo.apps.trip_iod.open()


@pytest.fixture(scope='class')
def radio_app(menlo):
    menlo.apps.radio.open()


@pytest.fixture(scope='module')
def settings_menlo(menlo):
    page = menlo.settings
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
def settings_radio_menlo(settings_menlo):
    yield settings_menlo.radio.open()


@pytest.fixture(scope='class')
def settings_driver_assistance_menlo(settings_menlo):
    yield settings_menlo.driver_assistance.open()


@pytest.fixture(scope='class')
def settings_vehicle_menlo(settings_menlo):
    yield settings_menlo.vehicle.open()


@pytest.fixture(scope='class')
def settings_general_menlo(settings_menlo):
    yield settings_menlo.general.open()


@pytest.fixture(scope='class')
def settings_system_updates_menlo(settings_menlo):
    yield settings_menlo.system_updates.open()


@pytest.fixture(scope='class')
def settings_display_menlo(settings_menlo):
    yield settings_menlo.display.open()


@pytest.fixture(scope='class')
def settings_charge_menlo(settings_menlo):
    yield settings_menlo.charge.open()


@pytest.fixture(scope='class')
def settings_vehile_hotspot_menlo(settings_menlo):
    yield settings_menlo.vehicle_hotspot.open()


@pytest.fixture(scope='class')
def settings_personal_profiles_menlo(settings_menlo):
    yield settings_menlo.personal_profiles.open()


@pytest.fixture(scope='class')
def settings_ambient_light_menlo(settings_menlo):
    yield settings_menlo.ambient_light.open()
