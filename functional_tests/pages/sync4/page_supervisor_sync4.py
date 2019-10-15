from functional_tests.pages.sync4.pages import (AppsPage, AudioPage,
                                                BluetoothPage, ClimatePage,
                                                ConnectionSettingsPage,
                                                FeaturesPage,
                                                GeneralSettingsPage, PhonePage,
                                                SettingsAudioPage,
                                                SettingsClockPage,
                                                SettingsPage)


class PageSupervisor:
    def __init__(self):
        self.apps_page = AppsPage()
        self.bluetooth_page = BluetoothPage()
        self.climate_page = ClimatePage()
        self.connection_settings_page = ConnectionSettingsPage()
        self.settings_page = SettingsPage()
        self.phone_page = PhonePage()
        self.settings_audio_page = SettingsAudioPage()
        self.features_page = FeaturesPage()
        self.general_settings_page = GeneralSettingsPage()
        self.settings_clock_page = SettingsClockPage()
        self.audio_page = AudioPage()
