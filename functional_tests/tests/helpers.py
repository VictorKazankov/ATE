import logging
import re
import time

import allure
from vhat_client import CollectionMode
from vhat_client import LookupError as ObjectNotFoundError

from functional_tests.pages import hmi


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
    except ObjectNotFoundError:
        dict_attempts['fail'] += 1
        logging.warning('{} object not recognized {} times'.format(name, dict_attempts['fail']))


def get_result_recognition(name, dict_attempts):
    try:
        # if recognition is successful we increase our counter of success on one
        hmi.wait_for_object(name)
        dict_attempts['success'] += 1
        mark_state('PASS', name)
        logging.info('{} object recognized'.format(name))
    except ObjectNotFoundError:
        dict_attempts['fail'] += 1
        mark_state('FAIL', name)
        logging.warning('{} object not recognized'.format(name))


def get_exist_result(name):
    time.sleep(1)
    return hmi.obj_exists(name)


def touch_and_drag_page(object, x, y, dx, dy, modifier=None):
    hmi.touch_and_drag(object, x, y, dx, dy, modifier)
    time.sleep(3)
    logging.info('Drag is completed')


@allure.step('"{state}" to recognize "{name}" object')
def mark_state(state, name):
    pass


def check_wildcard_results_amount(wildcard_results, params, results_amount_expected):
    error_message_raw = 'Unexpected wildcard results amount for parameters {}. Expected: {}. Actual: {}'
    results_amount_actual = len(wildcard_results)
    assert results_amount_actual == results_amount_expected, \
        error_message_raw.format(params,
                                 results_amount_expected,
                                 results_amount_actual)


def is_collection_mode_match(expected, actual):
    if expected == CollectionMode.ANY:
        return actual in [CollectionMode.DAY, CollectionMode.NIGHT]
    else:
        return expected == actual


def is_wildcard_str_match(expected, actual):
    expected = expected
    expected_formatted = expected.replace('?', '.')
    expected_formatted = expected_formatted.replace('*', '.*')
    regex = re.compile(r'^{}$'.format(expected_formatted))
    return regex.match(actual)


def check_wildcard_results_content(results, wildcard_pattern):
    for item in results:
        check_related_attributes(item, **wildcard_pattern)


def check_related_attributes(obj, **kwargs):
    failed_matches = []
    error_message = 'Attribute {} wildcard {} mismatch. Actual value: {}'
    for attribute_name, value in kwargs.items():
        actual = getattr(obj, attribute_name)
        if attribute_name == "sync_collection_mode":
            matched = is_collection_mode_match(value, actual)
        else:
            matched = is_wildcard_str_match(value, actual)
        failed_matches.append(error_message.format(attribute_name, value, actual)) if not matched else None
    if failed_matches:
        raise AssertionError('Error: Following mismatches found: {}'.format(str(failed_matches)))
