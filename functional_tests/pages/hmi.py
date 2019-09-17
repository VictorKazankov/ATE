import time

from vhat_client import (ModifierState, MouseButton, attachToApplication,
                         object, tapObject, touchAndDrag, waitForObject)

from functional_tests.utils import wait_for_obj_benchmark


def attach_to_application():
    return attachToApplication('')


def wait_for_object(object_name, timeout=5000):
    start = time.time()
    obj = waitForObject(object_name, timeout)
    wait_for_obj_benchmark.set_time(time.time() - start, object_name)

    return obj


def tap_object(object):
    tapObject(object, ModifierState.NONE, MouseButton.LEFT_BUTTON)


def obj_exists(name):
    return object().exists(name)


def touch_and_drag(object, x, y, dx, dy, modifier=None):
    if modifier:
        touchAndDrag(object, x, y, dx, dy, modifier)
    else:
        touchAndDrag(object, x, y, dx, dy)
