from functional_tests.pages.sync3.page_supervisor_sync3 import PageSupervisor
from functional_tests.utils.sync3.constants import Icons, Text

# open method of the required page
open_page = PageSupervisor().home_page.open_home_page

# icons to navigation
to_page_object = Icons.MAIN_AUDIO_BUTTON_INACTIVE
from_page_object = Icons.MAIN_PHONE_BUTTON_INACTIVE

# text to verify
check_object = Text.SOURCES_TEXT
