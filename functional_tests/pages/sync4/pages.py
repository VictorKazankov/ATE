import logging

from functional_tests.pages import hmi
from functional_tests.pages.interaction import (tap, tap_if_visible,
                                                tap_on_coordinates)
from functional_tests.utils.sync4.constants import Icons, Text


class SettingsPage:
    def open_settings_page(self):
        if hmi.obj_exists(Icons.SOUND_SETTINGS_BUTTON) and hmi.obj_exists(Icons.GENERAL_SETTINGS_BUTTON):
            logging.info("Settings page is already open")
        elif tap_if_visible(Icons.MAIN_PANEL_SETTINGS_BUTTON_ACTIVE):
            logging.info('Open settings page')
        else:
            logging.info('Open settings page222')
            tap(Icons.MAIN_PANEL_SETTINGS_BUTTON_INACTIVE)

    def open_sound_settings_page(self):
        if hmi.obj_exists(Text.SOUND_SETTINGS_TITLE_TEXT):
            logging.info("Sound settings page is already open")
        elif tap_if_visible(Icons.SOUND_SETTINGS_BUTTON):
            logging.info('Open sound settings page')
        else:
            self.open_settings_page()
            tap(Icons.SOUND_SETTINGS_BUTTON)

    def open_setting_clock_page(self):
        if hmi.obj_exists(Text.HOUR_MODE_24_TEXT) and hmi.obj_exists(Text.CLOCK_SETTINGS_TEXT):
            logging.info('Clock settings page is already open')

        elif tap_if_visible(Icons.SETTINGS_CLOCK_BUTTON):
            logging.info('Open clock settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_CLOCK_BUTTON)

    def open_connection_settings(self):
        if hmi.obj_exists(Text.CONNECTION_SETTINGS_TITLE_TEXT):
            logging.info("Connection settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_CONNECTION_SETTINGS_BUTTON):
            logging.info('Open connection settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_CONNECTION_SETTINGS_BUTTON)

    def open_radio_settings(self):
        if hmi.obj_exists(Text.RADIO_SETTINGS_TITLE_TEXT):
            logging.info("Radio settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_RADIO_BUTTON):
            logging.info('Open Radio settings')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_RADIO_BUTTON)

    def open_mobile_apps_settings(self):
        if hmi.obj_exists(Text.MOBILE_APPS_SETTINGS_TITLE_TEXT):
            logging.info("Mobile Apps settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_MOBILE_APPS_BUTTON):
            logging.info('Open Mobile Apps settings')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_MOBILE_APPS_BUTTON)

    def open_general_settings(self):
        if hmi.obj_exists(Text.GENERAL_SETTINGS_TITLE_TEXT):
            logging.info("General settings page is already open")
        elif tap_if_visible(Icons.SETTINGS_GENERAL_SETTINGS_BUTTON):
            logging.info('Open general settings page')
        else:
            self.open_settings_page()
            tap(Icons.SETTINGS_GENERAL_SETTINGS_BUTTON)

    def settings_page_is_active(self):
        return hmi.obj_exists(Icons.MAIN_PANEL_SETTINGS_BUTTON_ACTIVE)

    def tap_on_back_button(self):
        tap(Icons.BACK_BUTTON)


class GeneralSettingsPage(SettingsPage):
    def open_language_page(self):
        if hmi.obj_exists(Text.LANGUAGE_TEXT) and hmi.obj_exists(Text.ESPANOL_TEXT):
            logging.info("Language settings page is already open")
        elif tap_if_visible(Text.LANGUAGE_TEXT):
            logging.info('Open language settings page')
        else:
            self.open_general_settings()
            tap(Text.LANGUAGE_TEXT)


class FeaturesPage:
    def open_features_page(self):
        return hmi.obj_exists(Icons.MAIN_FEATURES_BUTTON_INACTIVE)


class ClimatePage:
    def climate_page_is_active(self):
        return hmi.obj_exists(Text.CLIMATE_PAGE_CONTROLS_TITLE_TEXT)

    def open_climate_page(self):
        # coordinates for click on Climate panel button
        x = 250
        y = 1146
        tap_on_coordinates(x, y)

    def open_climate_menu(self):
        if hmi.obj_exists(Icons.CLIMATE_MENU_ADDITIONAL_CLIMATE_CONTROLS_TITLE):
            logging.info('Climate menu page pop up is already open')
        elif tap_if_visible(Icons.CLIMATE_PAGE_MENU_BUTTON):
            logging.info('Open climate menu pop up')
        else:
            self.open_climate_page()
            tap(Icons.CLIMATE_PAGE_MENU_BUTTON)

    def tap_on_close_button(self):
        tap(Icons.CLIMATE_CLOSE_BUTTON)


class PhonePage:
    def phone_page_is_active(self):
        return hmi.obj_exists(Text.PHONE_CONNECT_PHONE_TITLE_TEXT)

    def open_phone_page(self):
        if self.phone_page_is_active():
            logging.info('Phone page is already open')
        elif tap_if_visible(Icons.MAIN_PHONE_BUTTON_INACTIVE):
            logging.info('Open phone page')
        elif tap_if_visible(Text.CANCEL_TEXT):
            logging.info('Closing popup window on add phone page')
            self.open_phone_page()
        elif tap_if_visible(Icons.BACK_BUTTON):
            self.open_phone_page()
        else:
            raise Exception('Can`t open phone page')

    #     Pair bluetooth service page
    def open_add_phone_page(self):
        if hmi.obj_exists(Icons.ADD_PHONE_TITLE_ICON):
            logging.info('Add phone page is already open')
        elif tap_if_visible(Icons.PHONE_PAGE_ADD_PHONE_BUTTON):
            logging.info('Open add phone page')
        else:
            self.open_phone_page()
            tap(Icons.PHONE_PAGE_ADD_PHONE_BUTTON)


class SettingsAudioPage(SettingsPage):
    def settings_audio_page_is_active(self):
        return hmi.obj_exists(Icons.SOUND_SETTINGS_TITLE_ICON)

    def open_tone_settings_page(self):
        if hmi.obj_exists(Icons.SETTINGS_SOUND_RESET_ALL_BUTTON):
            logging.info('Tone settings page is already open')
        elif tap_if_visible(Text.TONE_SETTINGS_TEXT):
            logging.info('Open tone settings page')
        else:
            self.open_sound_settings_page()
            tap(Text.TONE_SETTINGS_TEXT)

    def open_occupancy_mode_page(self):
        if hmi.obj_exists(Text.SOUND_SETTINGS_OCCUPANCY_MODE_TITLE_TEXT) and hmi.obj_exists(Text.SOUND_SETTINGS_DRIVER_TEXT):
            logging.info("Occupancy mode page is already open")
        elif tap_if_visible(Text.SOUND_SETTINGS_OCCUPANCY_MODE_TEXT):
            logging.info("Open speed compensated volume page")
        else:
            self.open_sound_settings_page()
            tap(Text.SOUND_SETTINGS_OCCUPANCY_MODE_TEXT)

    def open_balance_fade_page(self):
        if hmi.obj_exists(Icons.SETTINGS_SOUND_BALANCE_RESET_BUTTON):
            logging.info('Balance / Fade page is already open')
        elif tap_if_visible(Text.SOUND_SETTINGS_BALANCE_FADE_TEXT):
            logging.info('Open balance / fade page')
        else:
            self.open_sound_settings_page()
            tap(Text.SOUND_SETTINGS_BALANCE_FADE_TEXT)

    def open_speed_compensated_volume(self):
        if hmi.obj_exists(Text.SOUND_SETTINGS_SPEED_COMPENSATED_TEXT) and hmi.obj_exists(Text.SPEED_COMPENSATED_VOLUME_PAGE_OFF_TEXT):
            logging.info("Speed compensated volume page is already open")
        elif tap_if_visible(Text.SOUND_SETTINGS_SPEED_COMPENSATED_TEXT):
            logging.info("Open speed compensated volume page")
        else:
            self.open_sound_settings_page()
            tap(Text.SOUND_SETTINGS_SPEED_COMPENSATED_TEXT)


class ConnectionSettingsPage(SettingsPage):
    def open_bluetooth_page(self):
        if hmi.obj_exists(Text.BLUETOOTH_SETTINGS_TITLE_TEXT):
            logging.info('Bluetooth page is already open')
        elif tap_if_visible(Text.BLUETOOTH_TEXT):
            logging.info('Open bluetooth page')
        else:
            self.open_connection_settings()
            tap(Text.BLUETOOTH_TEXT)


class BluetoothPage(ConnectionSettingsPage):
    def open_change_vehicle_name_page(self):
        if hmi.obj_exists(Text.CHANGE_VEHICLE_NAME_PAGE_TITLE_TEXT):
            logging.info('Change vehicle name page is already open')

        elif tap_if_visible(Text.BLUETOOTH_SETTINGS_CHANGE_VEHICLE_NAME_TEXT):
            logging.info('Open change vehicle name page')
        else:
            self.open_bluetooth_page()
            tap(Text.BLUETOOTH_SETTINGS_CHANGE_VEHICLE_NAME_TEXT)


class SettingsClockPage:
    def setting_clock_page_is_active(self):
        return hmi.obj_exists(Icons.ARROW_UP_THIN_BUTTON)

    def clock_24h_button_is_active(self):
        return hmi.obj_exists(Icons.CLOCK_SETTINGS_24H_ENABLE)

    def tap_on_24h_button(self):
        tap(Icons.CLOCK_SETTINGS_24H_ENABLE)

    def tap_on_12h_button(self):
        tap(Icons.CLOCK_SETTINGS_24H_DISABLE)


class AudioPage:
    def audio_page_is_active(self):
        return hmi.obj_exists(Text.AUDIO_PAGE_SOURCES_TEXT)

    def open_audio_page(self):
        # coordinates for click on Audio panel button
        x = 140
        y = 1146
        tap_on_coordinates(x, y)

    def tap_on_direct_tune_button(self):
        if hmi.obj_exists(Text.ENTER_TEXT) and hmi.obj_exists(Icons.AUDIO_DIRECT_CANCEL_BUTTON_ACTIVE):
            logging.info('Direct tune page is already open')
        elif tap_if_visible(Text.AUDIO_PAGE_DIRECT_TUNE_TEXT):
            logging.info('Open direct tune page')
        else:
            self.open_audio_page()
            tap(Text.AUDIO_PAGE_DIRECT_TUNE_TEXT)

    def tap_on_cancel_button(self):
        tap(Icons.AUDIO_DIRECT_CANCEL_BUTTON_ACTIVE)

    def tap_on_sources_page(self):
        if hmi.obj_exists(Text.ENTERTAINMENT_SOURCES_TITLE_TEXT):
            logging.info('Sources page is already open')
        elif tap_if_visible(Icons.AUDIO_SOURCES_FM_BUTTON):
            logging.info('Open sources page')
        elif tap_if_visible(Icons.AUDIO_SOURCES_AM_BUTTON):
            logging.info('Open sources page')
        else:
            self.open_audio_page()
            tap(Text.AUDIO_PAGE_SOURCES_TEXT)


class AppsPage:
    def apps_page_is_active(self):
        return hmi.obj_exists(Text.HELP_TEXT)

    def open_apps_page(self):
        if hmi.obj_exists(Text.APPS_PAGE_FIND_APPS_TEXT):
            logging.info('Apps is already open')
        elif tap_if_visible(Icons.MAIN_APPS_BUTTON_INACTIVE):
            logging.info('Open Apps page')
        else:
            tap(Icons.MAIN_APPS_BUTTON_ACTIVE)
