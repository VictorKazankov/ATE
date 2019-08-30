from vhat_client import (ModifierState, MouseButton, attachToApplication,
                         object, tapObject, touchAndDrag, waitForObject)


def attach_to_application():
    return attachToApplication('')


def wait_for_object(object, timeout=5000):
    return waitForObject(object, timeout)


def tap_object(object):
    tapObject(object, ModifierState.NONE, MouseButton.LEFT_BUTTON)


def obj_exists(name):
    return object().exists(name)


def touch_and_drag(object, x, y, dx, dy, modifier=None):
    if modifier:
        touchAndDrag(object, x, y, dx, dy, modifier)
    else:
        touchAndDrag(object, x, y, dx, dy)
