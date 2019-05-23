from functional_tests.tests.test_recognition import helpers

NAVIGATION_TEXT = 'Navigation fault'
CONNECTING_TEXT = 'Connecting phone...'
PAIR_PHONE_TEXT = 'Pair phone'
ADAPTIVE_VOLUME_TEXT = 'Adaptive volume'
AUTO_TIME_ZONE_TEXT = "If enabled, automatically adjusts the vehicle's clock when entering a new time zone."
AC_CONTROL_TEXT = 'A/C'
CLOCK_FORMAT_12H_TEXT = '12h'
FREQUENCY_105_50 = '105.50'

ATTEMPTS = 100


def test_recognition_navigation_text(driver):
    if not driver.home_page.home_page_is_active():
        driver.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, NAVIGATION_TEXT, counter)
    assert (counter['success'] / ATTEMPTS * 100) > 98, "Recognized text < 98%"


def test_recognition_connecting_text(driver):
    if not driver.home_page.home_page_is_active():
        driver.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, CONNECTING_TEXT, counter)
    assert (counter['success'] / ATTEMPTS * 100) > 98, "Recognized text < 98%"


def test_recognition_pair_phone_text(driver):
    if not driver.phone_page.phone_page_is_active():
        driver.phone_page.open_phone_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, PAIR_PHONE_TEXT, counter)
    assert (counter['success'] / ATTEMPTS * 100) > 98, "Recognized text < 98%"


def test_recognition_adaptive_volume_text(driver):
    if not driver.settings_audio_page.setting_audio_page_is_active:
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, ADAPTIVE_VOLUME_TEXT, counter)
    assert (counter['success'] / ATTEMPTS * 100) > 98, "Recognized text < 98%"


def test_recognition_text_on_auto_time_zone_update_dialog(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
        driver.settings_clock_page.close_information_dialog()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, AUTO_TIME_ZONE_TEXT, counter)
    assert (counter['success'] / ATTEMPTS * 100) > 98, "Recognized text < 98%"
    driver.settings_clock_page.close_information_dialog()


def test_recognition_ac_control_text_for_active_button(driver):
    if not driver.climat_page.climate_page_is_active():
        driver.climat_page.open_climate_page()
        if not driver.climat_page.ac_control_button_is_active():
            driver.climat_page.tap_on_climate_auto_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, AC_CONTROL_TEXT, counter)
    assert (counter['success'] / ATTEMPTS * 100) > 98, "Recognized text < 98%"


def test_recognition_12h_text_on_clock_format_button(driver):
    if not driver.settings_clock_page.setting_clock_page_is_active():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
        if not driver.settings_clock_page.clock_12h_button_is_active():
            driver.settings_clock_page.tap_on_24h_button()
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, CLOCK_FORMAT_12H_TEXT, counter)
    assert (counter['success'] / ATTEMPTS * 100) > 98, "Recognized text < 98%"


def test_recognition_105_50_frequency_text(driver):
    if not driver.audio_page.audio_page_is_active():
        driver.audio_page.open_audio_page()
    if not driver.audio_page.verify_frequency_value("105.50"):
        driver.audio_page.set_frequency("105.50")
    counter = {'success': 0, 'fail': 0}
    counter = helpers.get_result_recognition(ATTEMPTS, FREQUENCY_105_50, counter)
    assert (counter['success'] / ATTEMPTS * 100) > 98, "Recognized text < 98%"
