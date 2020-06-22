from functional_tests.pages.sync3.pages import SettingsPage
from functional_tests.sync3.test_data.test_data import wildcard
from functional_tests.utils.sync3.constants import Icons, Text

icon = Icons.MAIN_SETTINGS_BUTTON_ACTIVE

open_page_method = SettingsPage().open_settings_page

text = Text.SETTINGS_BUTTON_PANEL_TEXT

wildcard_test_data = [wildcard]

first_active_in_list = [
    "first_active*", 62, 57
]

middle_active_in_list = [
    "middle_active*", 66, 61
]

last_active_in_list = [
    "last_active*", 60, 58
]
