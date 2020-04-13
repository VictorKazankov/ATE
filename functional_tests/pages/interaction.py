import logging
from time import sleep

from functional_tests.pages import hmi
from vhat_client import LookupError
from vhat_client import object


def tap(name):
    """ Tap on object if it visible. """
    obj = hmi.wait_for_object(name)
    hmi.tap_object(obj)
    sleep(2)
    logging.info("Tap on {} with coordinates x:{}, y:{}".format(name, obj.x, obj.y))


def tap_if_visible(name):
    """ Tap on object if it visible else return false."""
    try:
        tap(name)
    except LookupError:
        logging.info("Can`t found {} object".format(name))
        return False
    return True


def tap_on_coordinates(x, y):
    obj = object(x, y)
    hmi.tap_object(obj)
    sleep(2)
    logging.info("Tap on x:{}, y:{}".format(obj.x, obj.y))


def tap_first_if_visible_else_second(name1, name2):
    """ Tap on object if it visible else tap on 2nd object ."""
    try:
        tap(name1)
    except LookupError:
        logging.info("Can`t tap on [{}] object, so tap on [{}]".format(name1, name2))
        return tap_if_visible(name2)
    return True


def return_obj_if_exist(name):
    """ Return an object if it exist else None ."""
    try:
        return hmi.waitForObject(name)
    except LookupError:
        return None


def tap_until_visible(name):
    """ Tap on object until it visible ."""
    while True:
        try:
            tap(name)
        except LookupError:
            return
