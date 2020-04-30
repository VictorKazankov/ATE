from functional_tests.pages.sync4.page_supervisor_sync4 import PageSupervisor
from functional_tests.utils.sync4.constants import Text

# open method of the required page
open_method = PageSupervisor().settings_page.open_settings_page

# text to verify
check_object = Text.SOUND_SETTINGS_BUTTON_TEXT

# expected result
expected_text = 'Sound'
