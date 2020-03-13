import time

from vhat_client import (ModifierState, MouseButton, attachToApplication,
                         changeSyncIconDB, changeSyncMode,
                         getObjectsDataByPattern, getText, object, tapObject,
                         touchAndDrag, waitForObject)

from functional_tests.utils import wait_for_obj_benchmark
from functional_tests.utils.ATE_exception_handing import (
    catch_exception_lookup, catch_exception_video)


def attach_to_application():
    return attachToApplication('')


@catch_exception_video
@catch_exception_lookup
def wait_for_object(object_name, timeout=7000):
    start = time.time()
    obj = waitForObject(object_name, timeout)
    wait_for_obj_benchmark.set_time(time.time() - start, object_name)
    return obj


def tap_object(object):
    tapObject(object, ModifierState.NONE, MouseButton.LEFT_BUTTON)


@catch_exception_video
def obj_exists(name):
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
