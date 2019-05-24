from functional_tests.tests.test_recognition import helpers
from functional_tests.utils import constants

ATTEMPTS = 100
PERCENT = 98

'''
    Home page
'''


def test_recognition_home_icon(driver):
    if not driver.home_page.home_page_is_active():
        driver.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    # run recognition and counting successful and failed attempts
    counter = helpers.get_result_recognition(ATTEMPTS, constants.HOME_PAGE_ICON, counter)
    # verify that successful percent > 98
    check_recognition(counter)


'''
    Climate page
'''


def test_recognition_climate_up_arrow_temperature(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    # run recognition and counting successful and failed attempts
    counter = helpers.get_result_recognition(ATTEMPTS, constants.CLIMATE_ARROW_UP, counter)
    # verify that successful percent > 98
    check_recognition(counter)


def test_recognition_climate_down_arrow_temperature(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.CLIMATE_ARROW_DOWN, counter)
    check_recognition(counter)


def test_recognition_climate_switch_on_off_button(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.CLIMATE_ON_OFF_BUTTON, counter)
    check_recognition(counter)


def test_recognition_climate_top_airflow_button(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.TOP_AIRFLOW_BUTTON, counter)
    check_recognition(counter)


'''
Audio page
'''


def test_recognition_frequency_button(driver):
    if not driver.audio_page.audio_page_is_active():
        driver.audio_page.open_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.AUDIO_FREQUENCY_BUTTON, counter)
    check_recognition(counter)


def test_recognition_presets_button(driver):
    if not driver.audio_page.audio_page_is_active():
        driver.audio_page.open_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.PRESETS_BUTTON, counter)
    check_recognition(counter)


'''
Settings page
'''


def test_recognition_back_settings_button(driver):
    if not driver.settings_audio_page.setting_audio_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.BACK_BUTTON, counter)
    check_recognition(counter)


def test_recognition_down_scroll_button_active(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.DOWN_SCROLL_BUTTON_ACTIVE, counter)
    check_recognition(counter)


def test_recognition_up_scroll_button_inactive(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.UP_SCROLL_BUTTON_INACTIVE, counter)
    check_recognition(counter)


def test_recognition_info_button(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.INFORMATION_DIALOG_BUTTON, counter)
    check_recognition(counter)


def test_recognition_scroll(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.SCROOL, counter)
    check_recognition(counter)


def test_recognition_slider(driver):
    if not driver.settings_audio_page.setting_audio_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.SLIDER, counter)
    check_recognition(counter)


def test_recognition_balance_fade_button(driver):
    if not driver.settings_audio_page.setting_audio_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.BALANCE_FADE_BUTTON, counter)
    check_recognition(counter)


def test_back_property_field_button(driver):
    driver.settings_audio_page.open_settings_audio_property_field_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, constants.BACK_PROPERTY, counter)
    check_recognition(counter)


def check_recognition(counter):
    assert (counter['success'] / ATTEMPTS * 100) > PERCENT, "Image recognition < {}%".format(PERCENT)
