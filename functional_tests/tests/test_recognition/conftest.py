import pytest
from functional_tests.pages import hmi
from functional_tests.pages.sync3 import page_supervisor_sync3
from functional_tests.pages.sync4 import page_supervisor_sync4
from functional_tests.tests import helpers
from functional_tests.utils import ssh_connect
from functional_tests.utils.sync3.constants import Icons


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
def speed_compensated_volume_page(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.settings_audio_page.open_speed_compensated_volume()
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
def settings_first_screen_sync3(driver_sync3):
    touch_drag_left = ['settings_page', 50, 200, 400, 0]
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_settings_page()
    if not helpers.get_exist_result(Icons.SETTINGS_SOUND_BUTTON):
        helpers.touch_and_drag_page(*touch_drag_left)


@pytest.fixture(scope='module')
def settings_second_screen_sync3(driver_sync3):
    touch_drag_right = ['settings_page', 400, 300, -300, 0]
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_settings_page()
    if not helpers.get_exist_result(Icons.SETTINGS_GENERAL_BUTTON):
        helpers.touch_and_drag_page(*touch_drag_right)


@pytest.fixture(scope='module')
def settings_sound_sync3(settings_first_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_setting_audio_page()


@pytest.fixture(scope='module')
def settings_clock_sync3(settings_first_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_setting_clock_page()
    return api.settings_clock_page


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
    return api.bluetooth_page


@pytest.fixture(scope='module')
def general_settings_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.settings_page.open_general_settings()
    return api.general_settings_page


@pytest.fixture(scope='module')
def audio_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.audio_page.open_audio_page()
    return api.audio_page


@pytest.fixture(scope='module')
def phone_sync4(app_connector):
    api = page_supervisor_sync4.PageSupervisor()
    api.phone_page.open_phone_page()
    return api.phone_page


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


@pytest.fixture(scope='module')
def audio_sources_sync3(audio_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.open_audio_sources()


@pytest.fixture(scope='module')
def audio_sirius_sync3(audio_sources_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.tap_siriusxm_button()


@pytest.fixture(scope='function')
def audio_direct_tune_sync3(audio_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.open_direct_tune()
    yield
    api.audio_page.tap_cancel_text()


@pytest.fixture(scope='module')
def climate_sync3(driver_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.climate_page.open_climate_page()


@pytest.fixture(scope='function')
def climate_defrost_sync3(climate_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.climate_page.tap_on_climate_defrost_button()
    yield
    api.climate_page.close_information_dialog()


@pytest.fixture(scope='function')
def climate_a_c_sync3(climate_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.climate_page.tap_on_climate_a_c_button()
    yield
    api.climate_page.close_information_dialog()


@pytest.fixture(scope='function')
def navigation_sync3(driver_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.navigation_page.open_navigation_dialog_page()
    yield
    api.climate_page.close_information_dialog()


@pytest.fixture(scope='module')
def phone_sync3(driver_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.phone_page.open_phone_page()


@pytest.fixture(scope='module')
def phone_pair_sync3(phone_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.phone_page.tap_pair_phone_button()


@pytest.fixture(scope='module')
def mobile_apps_sync3(driver_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.mobile_apps_page.open_mobile_apps_page()


@pytest.fixture(scope='module')
def mobile_apps_siriusxm_travel_link_sync3(mobile_apps_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.mobile_apps_page.open_siriusxm_travel_link()


@pytest.fixture(scope='module')
def mobile_apps_subscription_info_sync3(mobile_apps_siriusxm_travel_link_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.mobile_apps_page.open_subscription_info()


@pytest.fixture(scope='module')
def settings_bluetooth_sync3(settings_first_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_settings_bluetooth_page()


@pytest.fixture(scope='module')
def settings_radio_sync3(settings_first_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_settings_radio_page()


@pytest.fixture(scope='module')
def settings_navigation_sync3(settings_first_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_setting_navigation_page()


@pytest.fixture(scope='module')
def settings_mobile_apps_sync3(settings_first_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_setting_mobile_apps_page()


@pytest.fixture(scope='module')
def settings_sync_connect_sync3(settings_first_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_setting_sync_connect_page()


@pytest.fixture(scope='module')
def settings_navigation_map_sync3(settings_navigation_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_navigation_page.open_map_preferences_page()


@pytest.fixture(scope='module')
def settings_navigation_route_sync3(settings_navigation_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_navigation_page.open_route_preferences_page()


@pytest.fixture(scope='module')
def settings_general_sync3(settings_second_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_settings_general_page()


@pytest.fixture(scope='module')
def settings_automatic_updates_sync3(settings_second_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_settings_automatic_updates_page()


@pytest.fixture(scope='module')
def settings_vehicle_sync3(settings_second_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_settings_vehicle_page()


@pytest.fixture(scope='module')
def settings_display_sync3(settings_second_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_settings_display_page()


@pytest.fixture(scope='module')
def settings_voice_control_sync3(settings_second_screen_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.settings_page.open_settings_voice_control_page()


@pytest.fixture()
def settings_change_icon_storage(get_current_sync_build_version):
    # getting sync and build versions from ini file
    current_sync, current_build = get_current_sync_build_version
    name_json_file = 'day_mode.json'
    path_icon_storage = '/var/lib/vdp/vhat/icon_storage/{}/{}/'.format(current_sync, current_build)
    old_image_name = 'phone_add_phone_button'
    new_image_name = 'test_phone_add_phone'

    client = ssh_connect.start()
    # change image name in json file
    client.exec_command(
        r"sed -i 's/{}/{}/g' {}".format(old_image_name, new_image_name, path_icon_storage + name_json_file))
    # change image name file in icon_storage
    client.exec_command(r"mv {} {}".format(path_icon_storage + 'day_mode/' + old_image_name + '.png',
                                           path_icon_storage + 'day_mode/' + new_image_name + '.png'))
    hmi.attach_to_application()
    yield
    # restore name icon in json file and icon_storage
    client.exec_command(
        r"sed -i 's/{}/{}/g' {}".format(new_image_name, old_image_name, path_icon_storage + name_json_file))
    client.exec_command(r"mv {} {}".format(path_icon_storage + 'day_mode/' + new_image_name + '.png',
                                           path_icon_storage + 'day_mode/' + old_image_name + '.png'))
