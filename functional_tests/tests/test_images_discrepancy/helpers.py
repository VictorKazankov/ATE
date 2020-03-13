import time

from vhat_client import ScreenPoint

from functional_tests.pages.hmi import get_screenshot
from functional_tests.tests.test_images_discrepancy.conftest import folder_name

test_image_first = 'test_1.png'
test_image_second = 'test_2.png'


def get_path_screenshots(audio_page_open, climate_page_open):
    get_screenshot(test_image_first, folder_name)
    audio_page_open()
    time.sleep(1)
    get_screenshot(test_image_second, folder_name)
    path_1 = "/".join([folder_name, test_image_first])
    path_2 = "/".join([folder_name, test_image_second])
    climate_page_open()
    return path_1, path_2


def set_point(x, y):
    obj = ScreenPoint()
    obj.x = x
    obj.y = y
    return obj
