import logging

from functional_tests.pages import hmi
from functional_tests.utils import constants


class HomePage:

    def open_home_page(self):
        if check_visibility(constants.HOME_PAGE_ICON):
            hmi.tap_object(constants.HOME_PAGE_ICON)
        elif check_visibility(constants.BACK_BUTTON):
            hmi.tap_object(constants.BACK_BUTTON)
            self.open_home_page()
        elif check_visibility(constants.BACK_PROPERTY):
            hmi.tap_object(constants.BACK_PROPERTY)
            self.open_home_page()
        elif check_visibility(constants.CLOSE_BUTTON):
            hmi.tap_object(constants.CLOSE_BUTTON)
            self.open_home_page()
        else:
            raise Exception('Can`t open home page')

    def home_page_is_active(self):
        if check_visibility('Navigation fault'):
            return True
        return False


class ClimatePage:

    def open_climate_page(self):
        return tap_if_visible(constants.INACTIVE_CLIMATE_PAGE_ICON)

    def climate_page_is_active(self):
        if check_visibility(constants.ACTIVE_CLIMATE_PAGE_ICON):
            return True
        return False

    def ac_control_button_is_active(self):
        if check_visibility(constants.ACTIVE_CLIMATE_PAGE_ICON):
            return True
        return False

    def tap_on_climate_auto_button(self):
        return tap_if_visible(constants.CLIMATE_AUTO_BUTTON)


class AudioPage:

    def open_audio_page(self):
        return tap_if_visible(constants.INACTIVE_AUDIO_BUTTON)

    def audio_page_is_active(self):
        if check_visibility(constants.ACTIVE_AUDIO_BUTTON):
            return True
        return False

    def set_frequency(self, frequency):
        frequency_buttons = {0: "key_0_active", 1: "key_1_active", 2: "key_2_active",
                             3: "key_3_active", 4: "key_4_active", 5: 'key_5_active',
                             6: "key_6_active", 7: "key_7_active", 8: "key_8_active",
                             9: "key_9_active"}

        tap_if_visible(constants.AUDIO_FREQUENCY_BUTTON)
        for n in frequency:
            if n != ".":
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
        tap_if_visible(constants.ENTER_BUTTON_IN_SET_FREQUENCY)

    def verify_frequency_value(self, frequency):
        if check_visibility(frequency):
            return True
        return False


class PhonePage:

    def open_phone_page(self):
        return tap_if_visible(constants.INACTIVE_PHONE_PAGE_BUTTON)

    def phone_page_is_active(self):
        if check_visibility(constants.ACTIVE_PHONE_PAGE_BUTTON):
            return True
        return False


class SettingsPage:

    def open_settings_page(self):
        return tap_if_visible(constants.INACTIVE_SETTINGS_PAGE_BUTTON)

    def setting_page_is_active(self):
        if check_visibility(constants.SETTINGS_SOUND_BUTTON):
            return True
        return False

    def open_setting_clock_page(self):
        return tap_if_visible(constants.SETTINGS_CLOCK_BUTTON)

    def open_setting_audio_page(self):
        return tap_if_visible(constants.SETTINGS_SOUND_BUTTON)

    def open_setting_navigation_page(self):
        return tap_if_visible(constants.SETTINGS_NAVIGATION_BUTTON)


class SettingsClockPage:

    def setting_clock_page_is_active(self):
        if check_visibility(constants.CLOCK_SETTINGS_TITLE_TEXT):
            return True
        return False

    def clock_12h_button_is_active(self):
        if check_visibility(constants.CLOCK_SETTINGS_12H_TEXT):
            return True
        return False

    def tap_on_24h_button(self):
        return tap_if_visible(constants.CLOCK_SETTINGS_24H_TEXT)

    def open_information_dialog(self):
        return tap_if_visible(constants.INFORMATION_DIALOG_BUTTON)

    def close_information_dialog(self):
        return tap_if_visible(constants.CLOSE_BUTTON)


class SettingsAudioPage:

    def setting_audio_page_is_active(self):
        if check_visibility(constants.SOUND_SETTINGS_TITLE_TEXT):
            return True
        return False

    def open_settings_audio_property_field_page(self):
        return tap_if_visible(constants.ADAPTIVE_VOLUME_MEDIUM_OPTION)


class SettingsNavigationPage:

    def settings_navigation_page_is_active(self):
        if check_visibility(constants.SETTINGS_NAVIGATION_TEXT):
            return True
        return False

    def open_route_preferences_page(self):
        return tap_if_visible(constants.ROUTE_PREFERENCES_TEXT)

    def tap_on_down_arrow(self):
        return tap_if_visible(constants.DOWN_SCROLL_BUTTON_ACTIVE)


def tap_if_visible(name):
    """ Tap on object if it visible. """
    obj = check_visibility(name)
    if obj:
        hmi.tap_object(obj)
        logging.info("Tap on x:{}, y:{}".format(obj[0], obj[1]))
        return
    raise Exception("Can`t tap on {}".format(name))


def check_visibility(name):
    """ Checks if the object or text is visible. """
    try:
        return hmi.wait_for_object(name)
    except LookupError:
        return False
