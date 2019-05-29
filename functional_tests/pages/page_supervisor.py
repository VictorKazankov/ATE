from functional_tests.pages.pages import (AudioPage, ClimatePage, HomePage,
                                          PhonePage, SettingsAudioPage,
                                          SettingsClockPage,
                                          SettingsNavigationPage, SettingsPage)


class PageSupervisor:
    def __init__(self):
        self.home_page = HomePage()
        self.climat_page = ClimatePage()
        self.audio_page = AudioPage()
        self.phone_page = PhonePage()
        self.settings_page = SettingsPage()
        self.settings_clock_page = SettingsClockPage()
        self.settings_audio_page = SettingsAudioPage()
        self.settings_navigation_page = SettingsNavigationPage()
