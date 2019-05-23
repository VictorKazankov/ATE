"""Mock ATE API file"""


def attachToApplication(app, host, port):
    print('Connecting to {} app {}:{}'.format(app, host, port))


def detach():
    print("Disconnected")


def waitForObject(object, timeout=None):
    print("I am wait_for_object {}".format(object))
    x, y = 1, 2
    return x, y


def tapObject(object):
    print("I am tab_object")
    return True
