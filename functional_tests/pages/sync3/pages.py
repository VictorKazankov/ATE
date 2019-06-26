import logging

from functional_tests.pages import hmi
from functional_tests.utils.sync3 import constants
from vhat_client import LookupError


class HomePage:

    def open_home_page(self):
        if tap_if_visible(constants.HOME_PAGE_ICON):
            return
        elif tap_if_visible(constants.BACK_BUTTON):
            self.open_home_page()
        elif tap_if_visible(constants.BACK_PROPERTY):
            self.open_home_page()
        elif tap_if_visible(constants.CLOSE_BUTTON):
            self.open_home_page()
        else:
            raise Exception('Can`t open home page')

    def home_page_is_active(self):
        if check_visibility('fault'):
            return True
        return False


class ClimatePage:

    def open_climate_page(self):
        tap(constants.INACTIVE_CLIMATE_PAGE_ICON)

    def climate_page_is_active(self):
        return check_visibility(constants.ACTIVE_CLIMATE_PAGE_ICON)

    def ac_control_button_is_active(self):
        return check_visibility(constants.ACTIVE_CLIMATE_PAGE_ICON)

    def tap_on_climate_auto_button(self):
        tap(constants.CLIMATE_AUTO_BUTTON)

    def tap_on_climate_defrost_button(self):
        tap(constants.CLIMATE_DEFROST_BUTTON)

    def close_information_dialog(self):
        tap(constants.CLOSE_BUTTON)


class AudioPage:

    def open_audio_page(self):
        tap(constants.INACTIVE_AUDIO_PAGE_BUTTON)

    def audio_page_is_active(self):
        return check_visibility(constants.ACTIVE_AUDIO_BUTTON)

    def set_frequency(self, frequency):
        frequency_buttons = {0: "key_0_active", 1: "key_1_active", 2: "key_2_active",
                             3: "key_3_active", 4: "key_4_active", 5: 'key_5_active',
                             6: "key_6_active", 7: "key_7_active", 8: "key_8_active",
                             9: "key_9_active"}

        tap(constants.AUDIO_FREQUENCY_BUTTON)
        for n in frequency:
            if n != ".":
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
        tap(constants.ENTER_BUTTON_IN_SET_FREQUENCY)

    def verify_frequency_value(self, frequency):
        return check_visibility(frequency)


class PhonePage:

    def open_phone_page(self):
        tap(constants.INACTIVE_PHONE_PAGE_BUTTON)

    def phone_page_is_active(self):
        return check_visibility(constants.ACTIVE_PHONE_PAGE_BUTTON)


class MobileApppsPage:

    def open_mobile_apps_page(self):
        tap(constants.INACTIVE_MOBILE_APPS_PAGE_BUTTON)


class SettingsPage:

    def open_settings_page(self):
        tap(constants.INACTIVE_SETTINGS_PAGE_BUTTON)

    def setting_page_is_active(self):
        return check_visibility(constants.SETTINGS_SOUND_BUTTON)

    def open_setting_clock_page(self):
        tap(constants.SETTINGS_CLOCK_BUTTON)

    def open_setting_audio_page(self):
        tap(constants.SETTINGS_SOUND_BUTTON)

    def open_setting_navigation_page(self):
        tap(constants.SETTINGS_NAVIGATION_BUTTON)

    def open_settings_bluetooth_page(self):
        tap(constants.MOBILE_BLUETOOTH_BUTTON)

    def open_settings_radio_page(self):
        tap(constants.SETTINGS_RADIO_BUTTON)


class SettingsClockPage:

    def setting_clock_page_is_active(self):
        return check_visibility(constants.CLOCK_SETTINGS_TITLE_TEXT)

    def clock_12h_button_is_active(self):
        return check_visibility(constants.CLOCK_SETTINGS_12H_TEXT)

    def tap_on_24h_button(self):
        tap(constants.CLOCK_SETTINGS_24H_TEXT)

    def open_information_dialog(self):
        tap(constants.INFORMATION_DIALOG_BUTTON)

    def close_information_dialog(self):
        tap(constants.CLOSE_BUTTON)

    def tap_on_back_button(self):
        tap(constants.BACK_BUTTON)


class SettingsAudioPage:

    def setting_audio_page_is_active(self):
        return check_visibility(constants.SOUND_SETTINGS_TITLE_TEXT)

    def open_settings_audio_property_field_page(self):
        tap(constants.ADAPTIVE_VOLUME_MEDIUM_OPTION)

    def tap_on_down_scroll_button(self):
        tap(constants.DOWN_SCROLL_BUTTON_ACTIVE)

    def tap_on_back_button(self):
        tap(constants.BACK_BUTTON)


class SettingsBluetoothPage:

    def tap_on_back_button(self):
        tap(constants.BACK_BUTTON)


class SettingsRadioPage:

    def tap_on_back_button(self):
        tap(constants.BACK_BUTTON)


class SettingsNavigationPage:

    def settings_navigation_page_is_active(self):
        return check_visibility(constants.SETTINGS_NAVIGATION_TEXT)

    def open_route_preferences_page(self):
        tap(constants.ROUTE_PREFERENCES_TEXT)

    def open_navigation_preferences_page(self):
        tap(constants.NAVIGATION_PREFERENCES_TEXT)

    def open_guidance_prompts_page(self):
        tap(constants.DOWN_ARROW_SMALL)

    def tap_on_down_arrow(self):
        tap(constants.DOWN_SCROLL_BUTTON_ACTIVE)

    def tap_on_back_property_button_on_guidance_prompts_page(self):
        tap(constants.BACK_PROPERTY)

    def tap_on_back_button(self):
        tap(constants.BACK_BUTTON)


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


def tap_if_visible(name):
    """ Tap on object if it visible else return false.
     Used to return to home page """
    obj = check_visibility(name)
    if obj:
        hmi.tap_object(obj)
        logging.info("Tap on x:{}, y:{}".format(obj.x, obj.y))
        return True
    return False
