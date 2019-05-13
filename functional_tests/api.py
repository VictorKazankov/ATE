from functional_tests import ate


def wait_for_object(object, timeout=1000):
    return ate.waitForObject(object, timeout)
