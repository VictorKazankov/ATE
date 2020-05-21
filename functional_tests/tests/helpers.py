import logging
import re
import time

import vhat_client
from vhat_client import CollectionMode

from functional_tests.pages import hmi
from functional_tests.utils import get_benchmark


def get_exist_result(name):
    """
    check of existence object and getting time recognition
    :param object name:
    :return:
    """
    # wait 1 sec for rendering page with object
    time.sleep(1)
    # get time recognition of object for adding it to report
    start = time.time()
    result = vhat_client.object().exists(name)
    # count recognition time for recognized only
    if result:
        get_benchmark.set_time(time.time() - start, name)
    return result


def touch_and_drag_page(object, x, y, dx, dy, modifier=None):
    hmi.touch_and_drag(object, x, y, dx, dy, modifier)
    time.sleep(3)
    logging.info('Drag is completed')


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
