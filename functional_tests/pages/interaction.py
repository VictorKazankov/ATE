import logging
from time import sleep

from functional_tests.pages import hmi
from vhat_client import LookupError as ObjectNotFoundError
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
    except ObjectNotFoundError:
        logging.info("Can`t found {} object".format(name))
        return False
    return True


def tap_on_coordinates(x, y):
    obj = object(x, y)
    hmi.tap_object(obj)
    sleep(2)
    logging.info("Tap on x:{}, y:{}".format(obj.x, obj.y))
