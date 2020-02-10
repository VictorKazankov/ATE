from functional_tests.pages import hmi
from functional_tests.utils.sync4.constants import Text


def get_sound_text_object_from_settings_page():
    return hmi.wait_for_object(Text.SOUND_SETTINGS_BUTTON_TEXT)


def get_coordinates_of_sound_text_on_settings_page():
    target = get_sound_text_object_from_settings_page()
    # get left-up and right-down coordinates Sound text +- 2pc as a recognition distance buffer
    recognition_distance_buffer = 2
    x1 = int(target.x) - recognition_distance_buffer
    y1 = int(target.y) - recognition_distance_buffer
    x2 = int(target.x) + int(target.width) + recognition_distance_buffer
    y2 = int(target.y) + int(target.height) + recognition_distance_buffer
    return [x1, y1, x2, y2]


def get_coordinates_rectangle_for_empty_area():
    target = get_sound_text_object_from_settings_page()
    # get right-down coordinates Sound text
    # and use this point as left-up coordinate for rectangle in empty area
    empty_area_size = 5
    x1 = int(target.x) + int(target.width)
    y1 = int(target.y) + int(target.height)
    x2 = x1 + empty_area_size
    y2 = y1 + empty_area_size
    return [x1, y1, x2, y2]


def get_text_from_screen(x1, y1, x2, y2):
    return hmi.get_text(x1, y1, x2, y2)
