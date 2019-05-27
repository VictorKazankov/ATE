import logging

"""Mock ATE API file"""


def attachToApplication(app, host, port):
    logging.info('Connecting to {} app {}:{}'.format(app, host, port))


def detach():
    logging.info("Disconnected")


def waitForObject(object, timeout=None):
    logging.info("I am wait_for_object {}".format(object))
    x, y = 1, 2
    return x, y


def tapObject(object):
    logging.info("I am tap_object")
    return True
