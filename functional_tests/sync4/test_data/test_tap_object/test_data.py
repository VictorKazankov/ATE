from functional_tests.pages.sync4.page_supervisor_sync4 import PageSupervisor
from functional_tests.utils.sync4.constants import Icons, Text

# open method of the required page
open_page = PageSupervisor().climate_page.open_climate_page

# icons to navigation
to_page_object = Icons.MAIN_AUDIO_BUTTON_INACTIVE
from_page_object = Icons.MAIN_PHONE_BUTTON_INACTIVE

# text to verify
check_object = Text.AUDIO_PAGE_SOURCES_TEXT
