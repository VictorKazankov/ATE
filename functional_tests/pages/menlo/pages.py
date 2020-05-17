import logging

from functional_tests.pages import hmi
from functional_tests.pages.interaction import tap_if_visible
from functional_tests.utils.menlo.constants import Icons, Text


class AppsPage():
    def open_apps_page(self):
        if tap_if_visible(Icons.MAIN_APPS_BUTTON_INACTIVE):
            logging.info('Open apps page')
        else:
            logging.info('Apps icon no exist')


class SettingsPage():
    def open_settings_page(self):
        if hmi.obj_exists(Icons.SETTINGS_SOUND_BUTTON):
            logging.info('Settings page is already open')
        elif tap_if_visible(Icons.MAIN_SETTINGS_CONTROLS_BUTTON_INACTIVE):
            tap_if_visible(Text.SETTINGS_PANEL_TITLE_TEXT)
            logging.info('Open settings page')
        elif tap_if_visible(Text.SETTINGS_PANEL_TITLE_TEXT):
            logging.info('Open settings page')

    def open_settings_clock_page(self):
        if hmi.obj_exists(Text.SETTINGS_CLOCK_MODE_TEXT):
            logging.info('Settings clock page is already open')
        elif tap_if_visible(Icons.SETTINGS_CLOCK_BUTTON):
            logging.info('Open settings clock page')
        else:
            self.open_settings_page()
            tap_if_visible(Icons.SETTINGS_CLOCK_BUTTON)

    def open_settings_phone_list_page(self):
        if hmi.obj_exists(Icons.SETTINGS_PHONE_LIST_ADD_PHONE_BUTTON):
            logging.info('Settings phone page is already open')
        elif tap_if_visible(Icons.SETTINGS_PHONE_LIST_BUTTON):
            logging.info('Open settings phone page')
        else:
            self.open_settings_page()
            tap_if_visible(Icons.SETTINGS_PHONE_LIST_BUTTON)

    def open_settings_connectivity_page(self):
        if hmi.obj_exists(Text.SETTINGS_CONNECTIVITY_FEATURES_TEXT):
            logging.info('Settings connectivity page is already open')
        elif tap_if_visible(Icons.SETTINGS_CONNECTIVITY_BUTTON):
            logging.info('Open settings connectivity page')
        else:
            self.open_settings_page()
            tap_if_visible(Icons.SETTINGS_CONNECTIVITY_BUTTON)
