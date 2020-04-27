from functional_tests.pages.sync4.page_supervisor_sync4 import PageSupervisor
from functional_tests.utils.sync4.constants import Text, Icons

page_to_check_horizontal = PageSupervisor().settings_page.open_settings_page
page_to_check_vertical = PageSupervisor().settings_page.open_send_feedback

back_icon = Icons.BACK_BUTTON

marker_for_1st_settings_page = Text.CLOCK_SETTINGS_TEXT
marker_for_2nd_setting_page = Text.SETTINGS_VEHICLE_BUTTON_TEXT

vertical_item_bottom = Text.SETTINGS_GENERAL_FEEDBACK_VOICE
vertical_item_top = Text.SETTINGS_GENERAL_FEEDBACK_USER_INTERFACE
