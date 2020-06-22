from functional_tests.pages.sync4.pages import SettingsPage
from functional_tests.sync4.test_data.test_data import wildcard
from functional_tests.utils.sync4.constants import Icons, Text

icon = Icons.MAIN_PANEL_SETTINGS_BUTTON_ACTIVE

open_page_method = SettingsPage().open_settings_page

text = Text.MAIN_PANEL_SETTINGS_TEXT

wildcard_test_data = [wildcard]

first_active_in_list = [
    "wildcard_first_active*", 70, 51
]

middle_active_in_list = [
    "wildcard_middle_active*", 68, 52
]

last_active_in_list = [
    "wildcard_last_active*", 70, 50
]
