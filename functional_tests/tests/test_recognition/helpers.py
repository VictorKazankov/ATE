import logging

import allure
from functional_tests.pages import hmi
from vhat_client import LookupError


def get_result_recognition_in_loop(n, name, dict_attempts):
    for i in range(n):
        _try_to_recognize(dict_attempts, name, i + 1, n)
    return dict_attempts


@allure.step('"{n}" attempt from "{t}"')
def _try_to_recognize(dict_attempts, name, n, t):
    try:
        # if recognition is successful we increase our counter of success on one
        hmi.wait_for_object(name)
        dict_attempts['success'] += 1
        logging.info('{} object recognized {} times'.format(name, dict_attempts['success']))
    except LookupError:
        dict_attempts['fail'] += 1
        logging.warning('{} object not recognized {} times'.format(name, dict_attempts['fail']))


def get_result_recognition(name, dict_attempts):
    try:
        # if recognition is successful we increase our counter of success on one
        hmi.wait_for_object(name)
        dict_attempts['success'] += 1
        mark_state('PASS', name)
        logging.info('{} object recognized'.format(name))
    except LookupError:
        dict_attempts['fail'] += 1
        mark_state('FAIL', name)
        logging.warning('{} object not recognized'.format(name))


@allure.step('"{state}" to recognize "{name}" object')
def mark_state(state, name):
    pass
