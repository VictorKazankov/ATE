from functional_tests.pages.sync3.pages import (AudioPage, ClimatePage,
                                                HomePage, MobileApppsPage,
                                                PhonePage, SettingsAudioPage,
                                                SettingsBluetoothPage,
                                                SettingsClockPage,
                                                SettingsNavigationPage,
                                                SettingsPage,
                                                SettingsRadioPage)


class PageSupervisor:
    def __init__(self):
        self.home_page = HomePage()
        self.climat_page = ClimatePage()
        self.audio_page = AudioPage()
        self.phone_page = PhonePage()
        self.mobile_apps_page = MobileApppsPage()
        self.settings_page = SettingsPage()
        self.settings_clock_page = SettingsClockPage()
        self.settings_audio_page = SettingsAudioPage()
        self.settings_navigation_page = SettingsNavigationPage()
        self.settings_bluetooth_page = SettingsBluetoothPage()
        self.settings_radio_page = SettingsRadioPage()
