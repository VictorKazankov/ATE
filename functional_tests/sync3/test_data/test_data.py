from vhat_client import CollectionMode

from functional_tests.utils.sync3.constants import Icons

# Common test data

# test_get_objects_data_by_pattern and test_wait_for_object
wildcard = {
    "sync_version": "sync3",
    "sync_build_version": "version3.2_build17288_devtest",
    "sync_collection_mode": CollectionMode.ANY,
    # replacement to get at least 2 results (active and inactive)
    "name": Icons.MAIN_AUDIO_BUTTON_INACTIVE.replace('inac', '*')
}
