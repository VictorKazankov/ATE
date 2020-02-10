import logging
import sys
import time

from functional_tests.utils import wait_for_obj_benchmark
from vhat_client import (ModifierState, MouseButton, VideoStreamingError,
                         attachToApplication, changeSyncIconDB, changeSyncMode,
                         getText, object, tapObject, touchAndDrag,
                         waitForObject)


def attach_to_application():
    return attachToApplication('')


def wait_for_object(object_name, timeout=7000):
    try:
        start = time.time()
        obj = waitForObject(object_name, timeout)
        wait_for_obj_benchmark.set_time(time.time() - start, object_name)
        return obj
    except VideoStreamingError:
        logging.info("Can`t found video stream")
        sys.exit(1)
    except LookupError:
        logging.info("Can`t found object")


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


def get_text(x1, y1, x2, y2):
    try:
        return getText(x1, y1, x2, y2)
    except VideoStreamingError:
        logging.info("Can`t found video stream")
        sys.exit(1)
