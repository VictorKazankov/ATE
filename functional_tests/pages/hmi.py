from vhat_client import (ModifierState, MouseButton, attachToApplication,
                         tapObject, waitForObject)


def attach_to_applicatio():
    return attachToApplication('')


def wait_for_object(object, timeout=5000):
    return waitForObject(object, timeout)


def tap_object(object):
    tapObject(object, ModifierState.NONE, MouseButton.LEFT_BUTTON)
