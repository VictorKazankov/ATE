from functional_tests.pages.sync4.pages import (AppsPage, AudioPage,
                                                ClimatePage, FeaturesPage,
                                                PhonePage, SettingsAudioPage,
                                                SettingsClockPage,
                                                SettingsPage)


class PageSupervisor:
    def __init__(self):
        self.apps_page = AppsPage()
        self.climate_page = ClimatePage()
        self.settings_page = SettingsPage()
        self.phone_page = PhonePage()
        self.settings_audio_page = SettingsAudioPage()
        self.features_page = FeaturesPage()
        self.settings_clock_page = SettingsClockPage()
        self.audio_page = AudioPage()
