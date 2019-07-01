import logging
from time import sleep

from functional_tests.pages import hmi
from functional_tests.utils.sync4 import constants
from vhat_client import LookupError, Object


class SettingsPage:
    def open_settings_page(self):
        tap(constants.INACTIVE_SETTINGS_PAGE_BUTTON)

    def settings_page_is_active(self):
        return check_visibility(constants.SOUND_SETTINGS_BUTTON)

    def open_setting_audio_page(self):
        tap(constants.SOUND_SETTINGS_BUTTON)

    def open_setting_clock_page(self):
        tap(constants.CLOCK_SETTINGS_BUTTON)

    def tap_on_back_button(self):
        tap(constants.BACK_BUTTON)

    def open_connection_settings(self):
        tap(constants.CONNECTION_SETTINGS_BUTTON_TEXT)

    def open_radio_settings(self):
        tap(constants.RADIO_SETTINGS_ICON)

    def open_general_settings(self):
        tap(constants.GENERAL_SETTINGS_BUTTON)


class FeaturesPage:
    def open_features_page(self):
        return check_visibility(constants.FEATURES_PAGE_BUTTON)


class ClimatePage:
    def climate_page_is_active(self):
        return check_visibility(constants.CLIMATE_CONTROLS_TEXT)

    def open_climate_page(self):
        # coordinates for click on Climate panel button
        x = 250
        y = 1146
        tap_on_coordinates(x, y)

    def open_climate_menu(self):
        tap(constants.MENU_BUTTON)

    def tap_on_close_button(self):
        tap(constants.CLOSE_TEXT)


class PhonePage:
    def phone_page_is_active(self):
        return check_visibility(constants.CONNECT_PHONE_TITLE_TEXT)

    def open_phone_page(self):
        if tap_if_visible(constants.PHONE_PAGE_PANEL_BUTTON):
            return
        elif tap_if_visible(constants.CLOSE_TEXT):
            self.open_phone_page()
        else:
            raise Exception('Can`t open phone page')


class SettingsAudioPage:
    def settings_audio_page_is_active(self):
        return check_visibility(constants.TONE_SETTINGS_TEXT)

    def open_tone_settings_page(self):
        tap(constants.ARROW_RIGHT_BUTTON)

    def open_occupancy_mode_page(self):
        tap(constants.SOUND_SETTINGS_OCCUPANCY_MODE_TEXT)

    def open_balance_fade_page(self):
        tap(constants.SOUND_SETTINGS_BALANCE_FADE)


class SettingsClockPage:
    def setting_clock_page_is_active(self):
        return check_visibility(constants.UP_ARROW_THIN)

    def clock_12h_button_is_active(self):
        return check_visibility(constants.MODE_BUTTON_INACTIVITY)

    def clock_24h_button_is_active(self):
        return check_visibility(constants.MODE_BUTTON_ACTIVITY)

    def tap_on_24h_button(self):
        tap(constants.MODE_BUTTON_ACTIVITY)

    def tap_on_12h_button(self):
        tap(constants.MODE_BUTTON_INACTIVITY)


class AudioPage:
    def audio_page_is_active(self):
        return check_visibility(constants.SOURCES_TEXT)

    def open_audio_page(self):
        # coordinates for click on Audio panel button
        x = 140
        y = 1146
        tap_on_coordinates(x, y)

    def tap_on_direct_tune_button(self):
        tap(constants.DIRECT_TEXT)

    def tap_on_cancel_button(self):
        tap(constants.CANCEL_TEXT)

    def tap_on_sources_page(self):
        tap(constants.RADIO_SETTINGS_ICON)


class AppsPage:
    def apps_page_is_active(self):
        return check_visibility(constants.HELP_TEXT)

    def open_apps_page(self):
        tap(constants.APPS_BUTTON_PANEL_INACTIVE)


def tap(name):
    """ Tap on object if it visible. """
    obj = check_visibility(name)
    if not obj:
        raise Exception("Can`t tap on {}".format(name))
    hmi.tap_object(obj)
    sleep(2)
    logging.info("Tap on x:{}, y:{}".format(obj.x, obj.y))


def check_visibility(name):
    """ Checks if the object or text is visible. """
    try:
        return hmi.wait_for_object(name)
    except LookupError:
        return False


def tap_if_visible(name):
    """ Tap on object if it visible else return false.
     Used to return to home page """
    obj = check_visibility(name)
    if obj:
        hmi.tap_object(obj)
        sleep(2)
        logging.info("Tap on x:{}, y:{}".format(obj.x, obj.y))
        return True
    logging.info("Can`t found {} object".format(name))
    return False


def tap_on_coordinates(x, y):
    obj = Object()
    obj.x = x
    obj.y = y
    if not obj:
        raise Exception("Can`t tap on {}".format(obj))
    hmi.tap_object(obj)
    sleep(2)
    logging.info("Tap on x:{}, y:{}".format(obj.x, obj.y))
