import allure
import pytest
from allpairspy import AllPairs
from functional_tests.utils.sync4.constants import TASK_LINK
from vhat_client import Wildcard

from ..helpers import (check_wildcard_results_amount,
                       check_wildcard_results_content)
from .test_data import *

pytestmark = [pytest.mark.regression_sync4, pytest.mark.regression_sync3, pytest.mark.Wildcard]

path = 'test_wildcard'


@allure.testcase(TASK_LINK.format("VHAT-1684"), "VHAT-1684")
@pytest.mark.parametrize("sync_version,sync_build_version,sync_collection_mode,name,parent_screen", [
    values_list for values_list in AllPairs([
        sync_options,
        build_options,
        mode_options,
        name_options,
        parent_options
    ])

])
def test_wildcard_api(sync_version, sync_build_version,
                      sync_collection_mode, name, parent_screen):
    params = locals().copy()
    wildcard = Wildcard(params)
    # only 1 result is expected
    matched_objects = wildcard.getMatchObjects()
    check_wildcard_results_amount(matched_objects, params, 1)
    check_wildcard_results_content(matched_objects, params)


@allure.testcase(TASK_LINK.format("VHAT-1688"), "VHAT-1688")
def test_wildcard_by_properties():
    params = {"sync_version": '*',
              "sync_build_version": '*',
              "sync_collection_mode": CollectionMode.NONE,
              "name": '*',
              "parent_screen": '*'}
    wildcard = Wildcard(params)
    # first element is expected from test_data
    wildcard.name = name_options[0]
    wildcard.sync_collection_mode = mode_options[0]
    wildcard.sync_build_version = build_options[0]
    wildcard.sync_version = sync_options[0]
    wildcard.parent_screen = parent_options[0]
    params = {key: getattr(wildcard, key) for key in params.keys()}
    # only one result is expected using updated params
    matched_objects = wildcard.getMatchObjects()
    check_wildcard_results_amount(matched_objects, params, 1)
    check_wildcard_results_content(matched_objects, params)


@allure.testcase(TASK_LINK.format("VHAT-1689"), "VHAT-1689")
def test_wildcard_collection_mode_none():
    params = {"sync_version": '*',
              "sync_build_version": '*',
              "sync_collection_mode": CollectionMode.NONE,
              "name": '*'}
    wildcard = Wildcard(params)
    check_wildcard_results_amount(wildcard.getMatchObjects(),
                                  params, 0)

# TODO: uncomment and possibly update once question VHAT-1703 is answered
# @pytest.mark.parametrize("key", ['a', 1, "", "name", True])
# @pytest.mark.parametrize("value", [1, "", False])
# def test_wildcard_unexpected_dict_values(key, value):
#     params = {"sync_version": value,
#               "sync_build_version": '*',
#               "sync_collection_mode": CollectionMode.ANY,
#               key: value}
#     wildcard = Wildcard(params)
#     with pytest.raises(ValueError):
#         wildcard.getMatchObjects()
#         pass

# TODO: uncomment and possibly update once question VHAT-1703 is answered
# def test_wildcard_empty_dict():
#     wildcard = Wildcard({})
#     with pytest.raises(ValueError):
#         wildcard.getMatchObjects()
