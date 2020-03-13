from functional_tests.pages.sync4.pages import SettingsPage
from functional_tests.sync4.test_data.test_data import wildcard
from functional_tests.utils.sync4.constants import Icons

icon = Icons.MAIN_PANEL_SETTINGS_BUTTON_ACTIVE

open_page_method = SettingsPage().open_settings_page

wildcard_test_data = [wildcard]
