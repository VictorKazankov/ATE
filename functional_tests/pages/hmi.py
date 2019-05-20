from functional_tests import ate
from functional_tests.pages.pages import (AudioPage, ClimatePage, HomePage,
                                          PhonePage, SettingsAudioPage,
                                          SettingsClockPage, SettingsPage)
from functional_tests.tests.test_recognition.helpers import Helpers


class HMI:
    def __init__(self):
        self.home_page = HomePage(self)
        self.climat_page = ClimatePage(self)
        self.audio_page = AudioPage(self)
        self.phone_page = PhonePage(self)
        self.settings_page = SettingsPage(self)
        self.settings_clock_page = SettingsClockPage(self)
        self.settings_audio_page = SettingsAudioPage(self)
        self.helpers = Helpers(self)

    def wait_for_object(self, object, timeout=1000):
        return ate.waitForObject(object, timeout)

    def mouse_press(self, object):
        return ate.tapObject(object)
