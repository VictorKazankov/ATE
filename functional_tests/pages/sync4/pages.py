import logging

from functional_tests.pages import hmi
from functional_tests.utils.sync4 import constants
from vhat_client import LookupError


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


class FeaturesPage:
    def open_features_page(self):
        return check_visibility(constants.FEATURES_PAGE_BUTTON)


class ClimatePage:
    def climate_page_is_active(self):
        return check_visibility(constants.CLIMATE_CONTROLS_TEXT)

    def open_climate_page(self):
        tap(constants.CLIMATE_PAGE_PANEL_BUTTON)


class PhonePage:
    def phone_page_is_active(self):
        return check_visibility(constants.CONNECT_PHONE_TITLE_TEXT)

    def open_phone_page(self):
        tap(constants.PHONE_PAGE_PANEL_BUTTON)


class SettingsAudioPage:
    def settings_audio_page_is_active(self):
        return check_visibility(constants.TONE_SETTINGS_TITLE_TEXT)

    def open_tone_settings_page(self):
        tap(constants.ARROW_RIGHT_BUTTON)


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
    logging.info("Tap on x:{}, y:{}".format(obj.x, obj.y))


def check_visibility(name):
    """ Checks if the object or text is visible. """
    try:
        return hmi.wait_for_object(name)
    except LookupError:
        return False
