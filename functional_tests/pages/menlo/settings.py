import logging

from functional_tests.pages.hmi import touch_and_drag, wait_for_object, obj_exists
from functional_tests.pages.interaction import tap, tap_first_if_visible_else_second, tap_if_visible
from functional_tests.pages.menlo.base import SwipeError, BasePage
from functional_tests.pages.menlo.constants import Icons, Text


class ControlsSettingsPage(BasePage):
    _load_indicators = [Icons.MAIN_SETTINGS_CONTROLS_BUTTON_ACTIVE, Text.CONTROLS_TAB_TITLE_TEXT]

    def open(self):
        if not self.is_active:
            tap(Icons.MAIN_SETTINGS_CONTROLS_BUTTON_INACTIVE)
            self.wait_for_page_to_load()
        logging.info('Open Controls/Settings page')
        return self

    def close(self):
        if ControlsSettingsPage().is_active:
            tap(Icons.MAIN_SETTINGS_CONTROLS_BUTTON_ACTIVE)
            logging.info('Close Controls/Settings page')
        return self

    @staticmethod
    def _swipe_down_to(name):
        element = wait_for_object(name)
        touch_and_drag(element, element.x, element.y, 0, -element.y)

    def swipe_down_using_elements(self, targets, swipe_down_elements):
        if not swipe_down_elements:
            raise RuntimeError('swipe down elements not specified')

        def target_exists():
            return any(obj_exists(target) for target in targets)

        if target_exists():
            return self
        for element in swipe_down_elements:
            self._swipe_down_to(element)
            if target_exists():
                return self
        raise SwipeError('Unable to swipe to any of {}'.format(targets))


class ControlsSettingsItemPageBase(ControlsSettingsPage):
    @property
    def _icon_to_open(self):
        raise NotImplementedError('{} icon to open is not assigned'.format(self.__class__.__name__))

    @property
    def _text_to_open(self):
        raise NotImplementedError('{} text to open is not assigned'.format(self.__class__.__name__))

    def open_using_base(self, base):
        if not self.is_active:
            if not tap_first_if_visible_else_second(self._icon_to_open, self._text_to_open):
                base.open().swipe_down_until_exists([self._icon_to_open, self._text_to_open])
                tap_first_if_visible_else_second(self._icon_to_open, self._text_to_open)
            self.wait_for_page_to_load()
        logging.info('Open {}'.format(self.__class__.__name__))
        return self


class ControlsPage(ControlsSettingsPage):
    _load_indicators = [Icons.CONTROLS_VALET_MODE_BUTTON_INACTIVE, Text.CONTROLS_ACCESS_BUTTON_TEXT]

    def open(self):
        if not self.is_active:
            ControlsSettingsPage().open()
            tap(Text.SETTINGS_TAB_TITLE_TEXT)
            tap(Text.CONTROLS_TAB_TITLE_TEXT)
            self.wait_for_page_to_load()
        logging.info('Open Controls tab page')
        return self

    @property
    def valet_mode(self):
        return ValetModePage()

    def swipe_down_until_exists(self, targets):
        raise RuntimeError('Swipe for Controls tab is not expected due to current build elements amount')


class ControlsItemPageBase(ControlsSettingsItemPageBase, ControlsPage):
    def open(self):
        self.open_using_base(ControlsPage())
        return self


class ValetModePage(ControlsItemPageBase):
    _load_indicators = [Text.CONTROLS_VALET_MODE_TITLE_TEXT]
    _text_to_open = Text.CONTROLS_VALET_MODE_BUTTON_TEXT
    _icon_to_open = Icons.CONTROLS_VALET_MODE_BUTTON_INACTIVE


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

    @property
    def radio(self):
        return SettingsRadioPage()

    @property
    def driver_assistance(self):
        return SettingsDriverAssistancePage()

    @property
    def vehicle(self):
        return SettingsVehiclePage()

    @property
    def general(self):
        return SettingsGeneralPage()

    @property
    def system_updates(self):
        return SettingsSystemUpdatesPage()

    @property
    def charge(self):
        return SettingsChargePage()

    @property
    def vehicle_hotspot(self):
        return SettingsVehicleHotspotPage()

    @property
    def personal_profiles(self):
        return SettingsPersonalProfilesPage()

    @property
    def ambient_light(self):
        return SettingsAmbientLightPage()

    @staticmethod
    def _swipe_down_to(name):
        element = wait_for_object(name)
        touch_and_drag(element, element.x, element.y, 0, -element.y)

    def swipe_down_until_exists(self, targets):
        self.swipe_down_using_elements(targets,
                                       [Text.SETTINGS_CONNECTIVITY_BUTTON_TEXT, Text.SETTINGS_DISPLAY_BUTTON_TEXT])


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


class SettingsItemPageBase(ControlsSettingsItemPageBase, SettingsPage):
    def open(self):
        self.open_using_base(SettingsPage())
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


class SettingsRadioPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_RADIO_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_RADIO_BUTTON
    _text_to_open = Text.SETTINGS_RADIO_BUTTON_TEXT


class SettingsDriverAssistancePage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_DRIVER_ASSISTANCE_TITLE_TEXT,
                        Text.SETTINGS_DRIVER_ASSISTANCE_REAR_VIEW_CAMERA_DELAY_TEXT]
    _icon_to_open = Icons.SETTINGS_DRIVER_ASSISTANCE_BUTTON
    _text_to_open = Text.SETTINGS_DRIVER_ASSISTANCE_BUTTON_TEXT

    # because when the Driver Assistance tab opens, a popup opens that hides menu items
    def open(self):
        if not self.is_active:
            tap(Text.SETTINGS_DRIVER_ASSISTANCE_BUTTON_TEXT)
            self.wait_for_page_to_load()
            tap_if_visible(Text.SETTINGS_DRIVER_ASSISTANCE_CLOSE_BUTTON_TEXT)
        logging.info('Open {}'.format(self.__class__.__name__))
        return self


class SettingsVehiclePage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_VEHICLE_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_VEHICLE_BUTTON
    _text_to_open = Text.SETTINGS_VEHICLE_BUTTON_TEXT


class SettingsGeneralPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_GENERAL_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_GENERAL_BUTTON
    _text_to_open = Text.SETTINGS_GENERAL_BUTTON_TEXT


class SettingsSystemUpdatesPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_SYSTEM_UPDATES_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_SYSTEM_UPDATES_BUTTON
    _text_to_open = Text.SETTINGS_SYSTEM_UPDATES_BUTTON_TEXT


class SettingsChargePage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_CHARGE_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_CHARGE_BUTTON
    _text_to_open = Text.SETTINGS_CHARGE_BUTTON_TEXT


class SettingsVehicleHotspotPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_VEHICLE_HOTSPOT_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_VEHICLE_HOTSPOT_BUTTON
    _text_to_open = Text.SETTINGS_VEHICLE_HOTSPOT_BUTTON_TEXT


class SettingsPersonalProfilesPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_PERSONAL_PROFILES_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_PERSONAL_PROFILES_BUTTON
    _text_to_open = Text.SETTINGS_PERSONAL_PROFILES_BUTTON_TEXT


class SettingsAmbientLightPage(SettingsItemPageBase):
    _load_indicators = [Text.SETTINGS_AMBIENT_LIGHT_TITLE_TEXT]
    _icon_to_open = Icons.SETTINGS_AMBIENT_LIGHT_BUTTON
    _text_to_open = Text.SETTINGS_AMBIENT_LIGHT_BUTTON_TEXT
