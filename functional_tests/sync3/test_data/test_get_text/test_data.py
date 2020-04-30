from functional_tests.pages.sync3.page_supervisor_sync3 import PageSupervisor
from functional_tests.utils.sync3.constants import Text

# open method of the required page
open_method = PageSupervisor().settings_page.open_settings_page

# text to verify
check_object = Text.SETTINGS_SOUND_TEXT

# expected result
expected_text = 'Sound'
