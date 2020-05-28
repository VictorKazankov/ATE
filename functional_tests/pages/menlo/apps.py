import logging

from functional_tests.pages.hmi import obj_exists
from functional_tests.pages.interaction import tap, tap_first_if_visible_else_second
from functional_tests.pages.menlo.base import BasePage
from functional_tests.pages.menlo.constants import Icons, Text


class AppsPage(BasePage):
    _load_indicators = [Icons.APPS_SEATBELT_BUTTON, Text.APPS_TIRE_PRESSURE_TEXT]

    def open(self):
        if not self.is_active:
            tap(Icons.MAIN_APPS_BUTTON_INACTIVE)
            self.wait_for_page_to_load()
        logging.info('Open apps page')
        return self

    @property
    def is_active(self):
        return all(obj_exists(indicator) for indicator in self._load_indicators)

    @property
    def tire_pressure(self):
        return TirePressureAppPage()

    @property
    def trip_iod(self):
        return TripIodAppPage()

    def close(self):
        if AppsPage().is_active:
            tap(Icons.MAIN_APPS_BUTTON_ACTIVE)
            logging.info('Close apps page')
        return self


class AppPageBase(AppsPage):

    @property
    def _icon_to_open(self):
        raise NotImplementedError('{} icon to open is not assigned'.format(self.__class__.__name__))

    @property
    def _text_to_open(self):
        raise NotImplementedError('{} text to open is not assigned'.format(self.__class__.__name__))

    def open(self):
        if not self.is_active:
            AppsPage().open()
            tap_first_if_visible_else_second(self._text_to_open, self._icon_to_open)
            self.wait_for_page_to_load()
        logging.info('Open {}'.format(self.__class__.__name__))
        return self


class TirePressureAppPage(AppPageBase):
    _load_indicators = [Text.TIRE_PRESSURE_APP_TITLE_TEXT]
    _icon_to_open = Icons.APPS_TIRE_PRESSURE_BUTTON
    _text_to_open = Text.APPS_TIRE_PRESSURE_TEXT


class TripIodAppPage(AppPageBase):
    _load_indicators = [Text.TRIP_IOD_APP_TITLE_TEXT]
    _icon_to_open = Icons.APPS_TRIP_IOD_BUTTON
    _text_to_open = Text.APPS_TRIP_IOD_TEXT
