import time
from vhat_client import ScreenPoint

from functional_tests.utils.ATE_exception_handing import (
    catch_exception_lookup, catch_exception_video)
from vhat_client import (ModifierState, MouseButton, attachToApplication,
                         captureFrames, changeSyncIconDB, changeSyncMode,
                         getImagesDiscrepancy, getObjectsDataByPattern,
                         getScreenshot, getText, object, tapObject,
                         touchAndDrag, waitForObject)


def attach_to_application():
    return attachToApplication('')


@catch_exception_video
@catch_exception_lookup
def wait_for_object(object_name, timeout=7000, top_left=ScreenPoint(0, 0), bottom_right=ScreenPoint(0, 0)):
    obj = waitForObject(object_name, timeout, top_left, bottom_right)
    return obj


def tap_object(element, modifier_state=ModifierState.NONE, mouse_button=MouseButton.LEFT_BUTTON):
    """
    wrapping method for vhat_client.tabObject
    :param element: object to tap
    :param modifier_state: ModifierState enum value
    :param mouse_button: MouseButton enum value
    """
    return tapObject(element, modifier_state, mouse_button)


@catch_exception_video
def obj_exists(name):
    """
    wrapper of object().exists() method
    :param name: name of the icon/text object to be recognized
    :return: the result of wrapped method - True if recognition was successful
    """
    # wait 1 sec for rendering page with object
    time.sleep(1)
    return object().exists(name)


def touch_and_drag(object, x, y, dx, dy, modifier=None):
    if modifier:
        touchAndDrag(object, x, y, dx, dy, modifier)
    else:
        touchAndDrag(object, x, y, dx, dy)


# TODO : 'InvalidSyncVersion' In case of the sync_version is incorrect."
#       'InvalidSyncBuildVersion' In case of the sync_build_version is incorrect."
def change_sync_icon_db(sync, build):
    changeSyncIconDB(sync, build)


# TODO :  'InvalidSyncCollectionMode' In case of the collection_mode is incorrect"
def change_sync_mode(collection_mode):
    changeSyncMode(collection_mode)


@catch_exception_video
def get_text(x1, y1, x2, y2):
    return getText(x1, y1, x2, y2)


def get_objects_data_by_pattern(*args, **kwargs):
    return getObjectsDataByPattern(*args, **kwargs)


def get_screenshot(filename, location):
    return getScreenshot(filename, location)


def get_images_discrepancy(path1, path2, *args, **kwargs):
    return getImagesDiscrepancy(path1, path2, *args, **kwargs)


@catch_exception_video
def capture_frames(interval, duration, top_left_coordinate, bottom_right_coordinate, path_directory):
    return captureFrames(interval, duration, top_left_coordinate, bottom_right_coordinate, path_directory)
