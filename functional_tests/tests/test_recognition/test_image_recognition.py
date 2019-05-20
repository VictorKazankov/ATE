HOME_ICON = 'home'
CLIMATE_BUTTON = 'climate'
CLIMATE_ARROW_UP = 'climate_up'
CLIMATE_ARROW_DOWN = 'climate_down'
CLIMATE_ON_OFF_BUTTON = 'climate_on_off'
FREQUENCY_BUTTON = 'audio_frequency'
AUDIO_BUTTON = 'audio'
PRESETS_BUTTON = 'presets'
TOP_AIRFLOW_BUTTON = 'top_airflow'
SETTINGS_PHONE_BUTTON = 'settings_phone'
SETTINGS_SOUND_BUTTON = 'settings_sound'
DOWN_SCROLL_BUTTON_ACTIVE = 'down_active'
UP_SCROLL_BUTTON_INACTIVE = 'up_inactive'
SLIDER = 'slider'
BALANCE_FADE_BUTTON = 'balance_fade'
INFO_BUTTON = 'information'
SCROOL = 'scrool'
BACK_PROPERTY_FIELD_BUTTON = 'back_property_field'
BACK_BUTTON = 'back'

'''
    Home page
'''


def test_recognition_home_icon(driver):
    if not driver.home_page.verify_action_home_page():
        driver.home_page.open_home_page()
    counter = {'success': 0, 'fail': 0}
    # run recognition and counting successful and failed attempts
    counter, attemps = driver.helpers.get_result_recognition(HOME_ICON, counter)
    # verify that successful percent > 98
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"


'''
    Climate page
'''


def test_recognition_climate_up_arrow_temperature(driver):
    if not driver.climat_page.verify_active_climate_page():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    # run recognition and counting successful and failed attempts
    counter, attemps = driver.helpers.get_result_recognition(CLIMATE_ARROW_UP, counter)
    # verify that successful percent > 98
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"


def test_recognition_climate_down_arrow_temperature(driver):
    if not driver.climat_page.verify_active_climate_page():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(CLIMATE_ARROW_DOWN, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"


def test_recognition_climate_switch_on_off_button(driver):
    if not driver.climat_page.verify_active_climate_page():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(CLIMATE_ON_OFF_BUTTON, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"


def test_recognition_climate_top_airflow_button(driver):
    if not driver.climat_page.verify_active_climate_page():
        driver.climat_page.open_climate_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(TOP_AIRFLOW_BUTTON, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


'''
Audio page
'''


def test_recognition_frequency_button(driver):
    if not driver.audio_page.verify_active_audio_page():
        driver.audio_page.open_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(FREQUENCY_BUTTON, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_presets_button(driver):
    if not driver.audio_page.verify_active_audio_page():
        driver.audio_page.open_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(PRESETS_BUTTON, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


'''
Settings page
'''


def test_recognition_back_settings_button(driver):
    if not driver.settings_audio_page.verify_active_setting_audio_page():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(BACK_BUTTON, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_down_scroll_button_active(driver):
    if not driver.settings_clock_page.verify_active_setting_clock_page():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(DOWN_SCROLL_BUTTON_ACTIVE, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_up_scroll_button_inactive(driver):
    if not driver.settings_clock_page.verify_active_setting_clock_page():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(UP_SCROLL_BUTTON_INACTIVE, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_info_button(driver):
    if not driver.settings_clock_page.verify_active_setting_clock_page():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(INFO_BUTTON, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_scroll(driver):
    if not driver.settings_clock_page.verify_active_setting_clock_page():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_clock_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(SCROOL, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_slider(driver):
    if not driver.settings_audio_page.verify_active_setting_audio_page():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(SLIDER, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


def test_recognition_balance_fade_button(driver):
    if not driver.settings_audio_page.verify_active_setting_audio_page():
        driver.settings_page.open_settings_page()
        driver.settings_page.open_setting_audio_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(BALANCE_FADE_BUTTON, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])


def test_back_property_field_button(driver):
    driver.settings_audio_page.open_settings_audio_property_field_page()
    counter = {'success': 0, 'fail': 0}
    counter, attemps = driver.helpers.get_result_recognition(BACK_PROPERTY_FIELD_BUTTON, counter)
    assert (counter['success'] / attemps * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])
