import logging

from functional_tests.pages.hmi import touch_and_drag, wait_for_object, obj_exists
from functional_tests.pages.interaction import tap, tap_first_if_visible_else_second
from functional_tests.pages.menlo.base import SwipeError
from functional_tests.pages.menlo.constants import Icons, Text
from functional_tests.pages.menlo.pages import ControlsSettingsPage


class SettingsPage(ControlsSettingsPage):
    _load_indicators = [Icons.SETTINGS_VEHICLE_BUTTON, Text.SETTINGS_SOUND_BUTTON_TEXT]

    def open(self):
        if not self.is_active:
            ControlsSettingsPage().open()
            tap(Text.CONTROLS_TAB_TITLE_TEXT)
            tap(Text.SETTINGS_TAB_TITLE_TEXT)
            self.wait_for_page_to_load()
        logging.info('Open Settings tab page')
        return self

    @property
    def sound(self):
        return SettingsSoundPage()

    @property
    def clock(self):
        return SettingsClockPage()

    @property
    def phone_list(self):
        return SettingsPhoneListPage()

    @property
    def connectivity(self):
        return SettingsConnectivityPage()

    @property
    def display(self):
        return SettingsDisplayPage()

    @staticmethod
    def _swipe_down_to(name):
        element = wait_for_object(name)
        touch_and_drag(element, element.x, element.y, 0, -element.y)

    def swipe_down_until_exists(self, targets):
        def target_exists():
            return any(obj_exists(target) for target in targets)

        swipe_down_elements = [Text.SETTINGS_CONNECTIVITY_BUTTON_TEXT, Text.SETTINGS_DISPLAY_BUTTON_TEXT]
        if target_exists():
            return self
        for element in swipe_down_elements:
            self._swipe_down_to(element)
            if target_exists():
                return self
        raise SwipeError('Unable to swipe to any of {}'.format(targets))


class SettingsClockPage(SettingsPage):
    _load_indicators = [Text.SETTINGS_CLOCK_TITLE_TEXT, Icons.SETTINGS_CLOCK_PM_BUTTON_INACTIVE]

    # because of MAIN_VOL_ON_OFF is recognized as SETTINGS_CLOCK_BUTTON
    def open(self):
        if not self.is_active:
            SettingsPage().open()
            tap(Text.SETTINGS_CLOCK_BUTTON_TEXT)
            self.wait_for_page_to_load()
        logging.info('Open {}'.format(self.__class__.__name__))
        return self


class SettingsItemPageBase(SettingsPage):
    @property
    def _icon_to_open(self):
        raise NotImplementedError('{} icon to open is not assigned'.format(self.__class__.__name__))

    @property
    def _text_to_open(self):
        raise NotImplementedError('{} text to open is not assigned'.format(self.__class__.__name__))

    def open(self):
        if not self.is_active:
            if not tap_first_if_visible_else_second(self._icon_to_open, self._text_to_open):
                SettingsPage().open().swipe_down_until_exists([self._icon_to_open, self._text_to_open])
                tap_first_if_visible_else_second(self._icon_to_open, self._text_to_open)
            self.wait_for_page_to_load()
        logging.info('Open {}'.format(self.__class__.__name__))
        return self


class SettingsSoundPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_SOUND_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_SOUND_BUTTON
    _text_to_open = Text.SETTINGS_SOUND_BUTTON_TEXT


class SettingsPhoneListPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_PHONE_LIST_TITLE_TEXT, Icons.SETTINGS_PHONE_LIST_ADD_PHONE_BUTTON]
    _icon_to_open = Icons.SETTINGS_PHONE_LIST_BUTTON
    _text_to_open = Text.SETTINGS_PHONE_LIST_BUTTON_TEXT


class SettingsConnectivityPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_CONNECTIVITY_FEATURES_TEXT]
    _icon_to_open = Icons.SETTINGS_CONNECTIVITY_BUTTON
    _text_to_open = Text.SETTINGS_CONNECTIVITY_BUTTON_TEXT


class SettingsDisplayPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_DISPLAY_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_DISPLAY_BUTTON
    _text_to_open = Text.SETTINGS_DISPLAY_BUTTON_TEXT
