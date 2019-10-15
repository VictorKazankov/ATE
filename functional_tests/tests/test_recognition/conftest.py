import pytest
from functional_tests.pages.sync3 import page_supervisor_sync3
from functional_tests.pages.sync4 import page_supervisor_sync4


@pytest.fixture(scope='module')
def settings_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.settings_page.open_settings_page()
    return api.settings_page


@pytest.fixture(scope='module')
def sound_settings_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.settings_audio_page.open_sound_settings_page()
    return api.settings_audio_page


@pytest.fixture(scope='module')
def occupancy_mode_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.settings_audio_page.open_occupancy_mode_page()


@pytest.fixture(scope='module')
def clock_settings_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.settings_page.open_setting_clock_page()
    yield api.settings_clock_page


@pytest.fixture(scope='module')
def connection_settings_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.settings_page.open_connection_settings()


@pytest.fixture(scope='module')
def bluetooth_settings_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.connection_settings_page.open_bluetooth_page()


@pytest.fixture(scope='module')
def change_vehicle_name_page_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.bluetooth_page.open_change_vehicle_name_page()
    return api


@pytest.fixture(scope='module')
def general_settings_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.settings_page.open_general_settings()
    return api.general_settings_page


@pytest.fixture(scope='module')
def audio_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.audio_page.open_audio_page()


@pytest.fixture(scope='module')
def direct_tune_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.audio_page.tap_on_direct_tune_button()
    return api.audio_page


@pytest.fixture(scope='module')
def apps_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.apps_page.open_apps_page()


@pytest.fixture(scope='module')
def audio_sync3(driver_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.open_audio_page()
    return api.audio_page


@pytest.fixture(scope='module')
def audio_sources_sync3(audio_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.open_audio_sources()
    return api.audio_page


@pytest.fixture(scope='module')
def audio_sirius_sync3(audio_sources_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.tap_siriusxm_button()
    api.audio_page.tap_clsvinyl_direction()
    return api.audio_page


@pytest.fixture(scope='function')
def audio_direct_tune_sync3(audio_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.open_direct_tune()
    yield api.audio_page
    api.audio_page.tap_cancel_text()


@pytest.fixture(scope='module')
def climate_sync3(driver_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.climate_page.open_climate_page()
    return api.climate_page


@pytest.fixture(scope='function')
def climate_defrost_sync3(climate_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.climate_page.tap_on_climate_defrost_button()
    yield api.climate_page
    api.climate_page.close_information_dialog()


@pytest.fixture(scope='function')
def climate_a_c_sync3(climate_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.climate_page.tap_on_climate_a_c_button()
    yield api.climate_page
    api.climate_page.close_information_dialog()


@pytest.fixture(scope='module')
def phone_sync3(driver_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.phone_page.open_phone_page()
    return api.phone_page


@pytest.fixture(scope='module')
def phone_pair_sync3(phone_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.phone_page.tap_pair_phone_button()
    return api.phone_page
