from functional_tests.pages.sync3.page_supervisor_sync3 import PageSupervisor
from functional_tests.utils.sync3.constants import Icons

page_to_check_horizontal = PageSupervisor().settings_page.open_settings_page
page_to_check_vertical = PageSupervisor().settings_page.open_setting_audio_page

back_icon = Icons.BACK_BUTTON

marker_for_1st_settings_page = Icons.SETTINGS_CLOCK_BUTTON
marker_for_2nd_setting_page = Icons.SETTINGS_GENERAL_BUTTON

vertical_item_top = 'Treble'
vertical_item_bottom = 'Bass'
