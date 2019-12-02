import time

from functional_tests.utils import wait_for_obj_benchmark
from vhat_client import (ModifierState, MouseButton, attachToApplication,
                         changeSyncIconDB, changeSyncMode, object, tapObject,
                         touchAndDrag, waitForObject)


def attach_to_application():
    return attachToApplication('')


def wait_for_object(object_name, timeout=7000):
    start = time.time()
    obj = waitForObject(object_name, timeout)
    wait_for_obj_benchmark.set_time(time.time() - start, object_name)

    return obj


def tap_object(object):
    tapObject(object, ModifierState.NONE, MouseButton.LEFT_BUTTON)


def obj_exists(name):
    time.sleep(0.5)
    return object().exists(name)


def touch_and_drag(object, x, y, dx, dy, modifier=None):
    if modifier:
        touchAndDrag(object, x, y, dx, dy, modifier)
    else:
        touchAndDrag(object, x, y, dx, dy)


def change_sync_icon_db(sync, build):
    changeSyncIconDB(sync, build)


def change_sync_mode(collection_mode):
    changeSyncMode(collection_mode)
