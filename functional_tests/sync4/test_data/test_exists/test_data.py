from functional_tests.pages.sync4.page_supervisor_sync4 import PageSupervisor
from functional_tests.utils.sync4.constants import Icons, Text

# open method of the required page
open_method = PageSupervisor().settings_page.open_settings_page

# icon to verify
icon = Icons.SETTINGS_ADD_PHONE_BUTTON

# text to verify
text = Text.MAIN_PANEL_SETTINGS_TEXT
