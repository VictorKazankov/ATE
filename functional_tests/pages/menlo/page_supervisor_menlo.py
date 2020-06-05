from functional_tests.pages.menlo.apps import AppsPage
from functional_tests.pages.menlo.settings import SettingsPage, ControlsPage


class PageSupervisor:
    def __init__(self):
        self.apps = AppsPage()
        self.settings = SettingsPage()
        self.controls = ControlsPage()
