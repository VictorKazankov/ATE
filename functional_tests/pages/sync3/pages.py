import logging
from time import sleep

from functional_tests.pages import hmi
from functional_tests.utils.sync3.constants import Icons, Text
from vhat_client import LookupError as ObjectNotFoundError


class HomePage:

    def open_home_page(self):
        if self.home_page_is_active():
            logging.info('Home page is already open')
        elif tap_if_visible(Icons.HOME_PAGE_ICON):
            return
        elif tap_if_visible(Icons.BACK_BUTTON):
            self.open_home_page()
        elif tap_if_visible(Icons.BACK_PROPERTY):
            self.open_home_page()
        elif tap_if_visible(Icons.CLOSE_BUTTON):
            self.open_home_page()
        elif tap_if_visible(Text.CANCEL):
            self.open_home_page()
        else:
            raise Exception('Can`t open home page')

    def home_page_is_active(self):
        if check_visibility(Text.NAVIGATION_FAULT_TEXT):
            return True
        return False

    def switch_to_fm_sources_on_home_page(self):
        if tap_if_visible(Icons.HOME_SIRIUS_ICON):
            tap(Text.AUDIO_HIGHWAY_TEXT)
            tap(Text.SOURCES_TEXT)
            tap(Icons.AUDIO_FM_SOURCE_BUTTON)
            tap(Icons.HOME_PAGE_ICON)


class ClimatePage:

    def open_climate_page(self):
        tap(Icons.INACTIVE_CLIMATE_PAGE_ICON)

    def climate_page_is_active(self):
        return check_visibility(Icons.ACTIVE_CLIMATE_PAGE_ICON)

    def tap_on_climate_a_c_button(self):
        tap(Icons.A_C_ACTIVE)

    def tap_on_climate_auto_button(self):
        tap(Icons.CLIMATE_AUTO_BUTTON)

    def tap_on_climate_defrost_button(self):
        tap(Icons.CLIMATE_DEFROST_BUTTON)

    def close_information_dialog(self):
        tap(Icons.CLOSE_BUTTON)


class AudioPage:

    def open_audio_page(self):
        tap(Icons.INACTIVE_AUDIO_PAGE_BUTTON)

    def audio_page_is_active(self):
        return check_visibility(Icons.ACTIVE_AUDIO_BUTTON)

    def set_frequency(self, frequency):
        frequency_buttons = {0: "key_0_active", 1: "key_1_active", 2: "key_2_active",
                             3: "key_3_active", 4: "key_4_active", 5: 'key_5_active',
                             6: "key_6_active", 7: "key_7_active", 8: "key_8_active",
                             9: "key_9_active"}

        tap(Icons.AUDIO_FREQUENCY_BUTTON)
        for n in frequency:
            if n != ".":
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
                hmi.tap_object(frequency_buttons[int(n)])
        tap(Icons.ENTER_BUTTON_IN_SET_FREQUENCY)

    def open_audio_sources(self):
        tap(Text.SOURCES_TEXT)

    def tap_siriusxm_button(self):
        tap(Icons.AUDIO_SIRIUS_SOURCES_BUTTON)

    def open_fm_type(self):
        tap_if_visible(Text.AUDIO_FM_TEXT)

    def tap_clsvinyl_direction(self):
        tap(Text.AUDIO_CLSVINYL_TEXT)

    def open_direct_tune(self):
        tap(Icons.AUDIO_FREQUENCY_BUTTON)

    def tap_cancel_text(self):
        tap(Text.AUDIO_DIRECT_TUNE_CANCEL)

    def tap_on_back_button(self):
        tap(Icons.BACK_BUTTON)


class PhonePage:

    def open_phone_page(self):
        tap(Icons.INACTIVE_PHONE_PAGE_BUTTON)

    def phone_page_is_active(self):
        return check_visibility(Icons.ACTIVE_PHONE_PAGE_BUTTON)

    def tap_pair_phone_button(self):
        tap(Icons.PHONE_PAIR_PHONE_BUTTON)

    def phone_pair_phone_page_is_active(self):
        return hmi.obj_exists(Icons.PHONE_PAIR_DISCOVER_BUTTON)


class MobileApppsPage:

    def open_mobile_apps_page(self):
        tap(Icons.INACTIVE_MOBILE_APPS_PAGE_BUTTON)

    def open_siriusxm_travel_link(self):
        tap(Icons.SIRIUSXM_BUTTON)

    def open_subscription_info(self):
        tap(Text.MOBILE_APPS_SIRIUSXM_SUBSCRIPTION_INFO_TEXT)


class NavigationPage:

    def open_navigation_page(self):
        tap(Icons.INACTIVE_NAVIGATION_PAGE_BUTTON)

    def close_information_dialog(self):
        tap(Icons.CLOSE_BUTTON)


class SettingsPage:

    def open_settings_page(self):
        if check_visibility(Icons.SETTINGS_SOUND_BUTTON) or check_visibility(Icons.SETTINGS_GENERAL_BUTTON):
            logging.info("Settings page is already open")
        elif tap_if_visible(Icons.ACTIVE_SETTINGS_PAGE_BUTTON):
            logging.info('Open settings page')
        else:
            tap(Icons.INACTIVE_SETTINGS_PAGE_BUTTON)

    def setting_page_is_active(self):
        return check_visibility(Icons.SETTINGS_SOUND_BUTTON)

    def open_setting_clock_page(self):
        if check_visibility(Text.CLOCK_SETTINGS_TITLE_TEXT):
            logging.info("Clock settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_CLOCK_BUTTON):
            logging.info('Open clock settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_CLOCK_BUTTON)

    def open_setting_audio_page(self):
        if check_visibility(Text.SOUND_SETTINGS_TITLE_TEXT):
            logging.info("Sound settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_SOUND_BUTTON):
            logging.info('Open sound settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_SOUND_BUTTON)

    def open_setting_navigation_page(self):
        if check_visibility(Text.NAVIGATION_SETTINGS_TEXT):
            logging.info("Navigation settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_NAVIGATION_BUTTON):
            logging.info('Open navigation settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_NAVIGATION_BUTTON)

    def open_settings_bluetooth_page(self):
        if check_visibility(Text.BLUETOOTH_SETTINGS_TEXT):
            logging.info("Bluetooth settings page is already open")
        elif tap_if_visible(Icons.MOBILE_BLUETOOTH_BUTTON):
            logging.info('Open bluetooth settings page')
        else:
            self.open_settings_page()
            tap(Icons.MOBILE_BLUETOOTH_BUTTON)

    def open_settings_radio_page(self):
        if check_visibility(Text.RADIO_SETTINGS_TEXT):
            logging.info("Radio settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_RADIO_BUTTON):
            logging.info('Open radio settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_RADIO_BUTTON)

    def open_setting_mobile_apps_page(self):
        if check_visibility(Text.SETTINGS_MOBILE_APPS_SETTINGS_TITLE_TEXT):
            logging.info("Mobile apps settings page is already open")
        elif tap_if_visible(Icons.MOBILE_APPS_BUTTON):
            logging.info('Open mobile apps settings page')
        else:
            self.open_settings_page()
            tap(Icons.MOBILE_APPS_BUTTON)

    def open_setting_sync_connect_page(self):
        if check_visibility(Text.SETTING_CONNECTIVITY_SETTINGS_TEXT):
            logging.info("SYNC connect settings page is already open")
        elif tap_if_visible(Icons.SYNC_CONNECT_BUTTON):
            logging.info('Open SYNC connect settings page')
        else:
            self.open_settings_page()
            tap(Icons.SYNC_CONNECT_BUTTON)

    def open_settings_general_page(self):
        if check_visibility(Text.SETTINGS_GENERAL_SETTINGS_LABEL_TEXT):
            logging.info("General settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_GENERAL_BUTTON):
            logging.info('Open general settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_GENERAL_BUTTON)

    def open_settings_automatic_updates_page(self):
        if check_visibility(Text.SETTINGS_AUTOMATIC_SYSTEM_UPDATES_TEXT):
            logging.info("Automatic system settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_AUTOMATIC_UPDATES_BUTTON):
            logging.info('Open automatic system settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_AUTOMATIC_UPDATES_BUTTON)

    def open_settings_vehicle_page(self):
        if check_visibility(Text.SETTINGS_VEHICLE_TITLE_TEXT):
            logging.info("Vehicle settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_VEHICLE_BUTTON):
            logging.info('Open vehicle settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_VEHICLE_BUTTON)

    def open_settings_display_page(self):
        if check_visibility(Text.SETTINGS_DISPLAY_SETTINGS_TITLE_TEXT):
            logging.info("Display settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_DISPLAY_BUTTON):
            logging.info('Open display settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_DISPLAY_BUTTON)

    def open_settings_voice_control_page(self):
        if check_visibility(Text.SETTINGS_VOICE_CONTROL_SETTINGS_TITLE_TEXT):
            logging.info("Voice control settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_VOICE_CONTROL_BUTTON):
            logging.info('Open voice control settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_VOICE_CONTROL_BUTTON)


class SettingsClockPage:

    def setting_clock_page_is_active(self):
        return check_visibility(Text.CLOCK_SETTINGS_TITLE_TEXT)

    def clock_12h_button_is_active(self):
        return check_visibility(Icons.CLOCK_SETTINGS_12H)

    def tap_on_24h_button(self):
        tap(Icons.CLOCK_SETTINGS_24H)

    def tap_on_12h_button(self):
        tap(Icons.CLOCK_SETTINGS_12H)

    def open_information_dialog(self):
        tap(Icons.INFORMATION_DIALOG_BUTTON)

    def close_information_dialog(self):
        tap(Icons.CLOSE_BUTTON)

    def tap_on_back_button(self):
        tap(Icons.BACK_BUTTON)


class SettingsAudioPage:

    def setting_audio_page_is_active(self):
        return check_visibility(Text.SOUND_SETTINGS_TITLE_TEXT)

    def open_settings_audio_property_field_page(self):
        tap(Icons.ADAPTIVE_VOLUME_MEDIUM_OPTION)

    def tap_on_down_scroll_button(self):
        tap(Icons.DOWN_SCROLL_BUTTON_ACTIVE)

    def tap_on_back_button(self):
        tap(Icons.BACK_BUTTON)


class SettingsBluetoothPage:

    def tap_on_back_button(self):
        tap(Icons.BACK_BUTTON)


class SettingsRadioPage:

    def tap_on_back_button(self):
        tap(Icons.BACK_BUTTON)


class SettingsNavigationPage(SettingsPage):

    def settings_navigation_page_is_active(self):
        return check_visibility(Text.NAVIGATION_SETTINGS_TEXT)

    def open_map_preferences_page(self):
        if check_visibility(Text.CITY_MODEL_TEXT):
            logging.info('Map preferences page is already open')
        elif tap_if_visible(Text.MAP_PREFERENCES_TEXT):
            logging.info('Open map preferences settings page')
        else:
            self.open_setting_navigation_page()
            tap(Text.MAP_PREFERENCES_TEXT)

    def open_route_preferences_page(self):
        if check_visibility(Text.PREFERRED_ROUTE_TEXT):
            logging.info('Route preferences page is already open')
        elif tap_if_visible(Text.ROUTE_PREFERENCES_TEXT):
            logging.info('Open route preferences settings page')
        else:
            self.open_setting_navigation_page()
            tap(Text.ROUTE_PREFERENCES_TEXT)

    def open_navigation_preferences_page(self):
        tap(Text.NAVIGATION_PREFERENCES_TEXT)

    def open_guidance_prompts_page(self):
        tap(Icons.DOWN_ARROW_SMALL)

    def tap_on_down_arrow(self):
        tap(Icons.DOWN_SCROLL_BUTTON_ACTIVE)

    def tap_on_back_property_button_on_guidance_prompts_page(self):
        tap(Icons.BACK_PROPERTY)

    def tap_on_back_button(self):
        tap(Icons.BACK_BUTTON)


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
        sleep(2)
        return hmi.wait_for_object(name)
    except ObjectNotFoundError:
        return False


def tap_if_visible(name):
    """ Tap on object if it visible else return false.
     Used to return to home page """
    obj = check_visibility(name)
    if obj:
        hmi.tap_object(obj)
        sleep(1)
        logging.info("Tap on x:{}, y:{}".format(obj.x, obj.y))
        return True
    return False
