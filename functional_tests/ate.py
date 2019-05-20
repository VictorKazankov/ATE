"""Mock ATE API file"""


def attachToApplication(app, host, port):
    print('Connecting to {} app {}:{}'.format(app, host, port))


def detach():
    print("Disconnected")


def waitForObject(object, timeout):
    print("I am wait_for_object")


def tapObject(object):
    print("I am tab_object")
