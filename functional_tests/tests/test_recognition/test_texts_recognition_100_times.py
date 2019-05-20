NAVIGATION_TEXT = 'Navigation fault'
CONNECTING_TEXT = 'Connecting phone...'
PAIR_PHONE_TEXT = 'Pair phone'
ADAPTIVE_VOLUME_TEXT = 'Adaptive volume'
AUTO_TIME_ZONE_TEXT = "If enabled, automatically adjusts the vehicle's clock when entering a new time zone."
AC_CONTROL_TEXT = 'A/C'
CLOCK_FORMAT_12H_TEXT = '12h'
FREQUENCY_105_50 = '105.50'


def test_recognition_navigation_text(driver):
    if not driver.home_page.verify_action_home_page():
        driver.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(NAVIGATION_TEXT, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized text < 98%"


def test_recognition_connecting_text(driver):
    if not driver.home_page.verify_action_home_page():
        driver.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(CONNECTING_TEXT, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized text < 98%"


def test_recognition_pair_phone_text(driver):
    if not driver.phone_page.verify_active_phone_page():
        driver.phone_page.open_phone_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(PAIR_PHONE_TEXT, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized text < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_adaptive_volume_text(driver):
    if not driver.settings_audio_page.verify_active_setting_audio_page:
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(ADAPTIVE_VOLUME_TEXT, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized text < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_text_on_auto_time_zone_update_dialog(driver):
    if not driver.settings_clock_page.verify_active_setting_clock_page():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
        driver.settings_clock_page.close_information_dialog()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(AUTO_TIME_ZONE_TEXT, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized text < 98%"
    driver.settings_clock_page.close_information_dialog()
    print(counter['success'], counter['fail'])


def test_recognition_ac_control_text_for_active_button(driver):
    if not driver.climat_page.verify_active_climate_page():
        driver.climat_page.open_climate_page()
        driver.climat_page.verify_active_ac_control_button()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(AC_CONTROL_TEXT, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized text < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_12h_text_on_clock_format_button(driver):
    if not driver.settings_clock_page.verify_active_setting_clock_page():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
        driver.settings_clock_page.verify_active_12h_button()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(CLOCK_FORMAT_12H_TEXT, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized text < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_105_50_frequency_text(driver):
    if not driver.audio_page.verify_active_audio_page():
        driver.audio_page.open_audio_page()
        driver.audio_page.verify_active_105_50_frequency()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(FREQUENCY_105_50, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized text < 98%"
    print(counter['success'], counter['fail'])
