from functional_tests.pages.menlo.pages import AppsPage, SettingsPage


class PageSupervisor:
    def __init__(self):
        self.apps = AppsPage()
        self.settings_page = SettingsPage()
