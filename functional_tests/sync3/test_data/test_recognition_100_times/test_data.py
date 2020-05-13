from functional_tests.pages.sync3.page_supervisor_sync3 import PageSupervisor
from functional_tests.utils.sync3.constants import Icons, Text

# open method of the required page
open_page = PageSupervisor().home_page.open_home_page

# icon to verify
icon = Icons.HOME_PAGE_ICON

# text to verify
text = Text.NAVIGATION_FAULT_TEXT
