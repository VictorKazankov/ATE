from functional_tests.pages.sync3.pages import SettingsPage
from functional_tests.sync3.test_data.test_data import wildcard
from functional_tests.utils.sync3.constants import Icons

icon = Icons.MAIN_SETTINGS_BUTTON_ACTIVE

open_page_method = SettingsPage().open_settings_page

wildcard_test_data = [wildcard]
