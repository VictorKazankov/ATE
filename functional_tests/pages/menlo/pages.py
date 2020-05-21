import logging

from functional_tests.pages.interaction import tap
from functional_tests.pages.menlo.base import BasePage
from functional_tests.utils.menlo.constants import Icons, Text


class AppsPage(BasePage):
    _load_indicators = [Icons.APPS_PHONE_BUTTON]

    def open(self):
        if not self.is_active:
            tap(Icons.MAIN_APPS_BUTTON_INACTIVE)
            self.wait_for_page_to_load()
        logging.info('Open apps page')
        return self

    def close(self):
        if self.is_active:
            tap(Icons.MAIN_APPS_BUTTON_ACTIVE)
            logging.info('Close apps page')


class ControlsSettingsPage(BasePage):
    _load_indicators = [Icons.MAIN_SETTINGS_CONTROLS_BUTTON_ACTIVE, Text.CONTROLS_TAB_TITLE_TEXT]

    def open(self):
        if not self.is_active:
            tap(Icons.MAIN_SETTINGS_CONTROLS_BUTTON_INACTIVE)
            self.wait_for_page_to_load()
        logging.info('Open Controls/Settings page')
        return self

    @staticmethod
    def close():
        if ControlsSettingsPage().is_active:
            tap(Icons.MAIN_SETTINGS_CONTROLS_BUTTON_ACTIVE)
            logging.info('Close Controls/Settings page')
