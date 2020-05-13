from functional_tests.utils.sync4.constants import Icons
from vhat_client import CollectionMode

# Common test data

# test_get_objects_data_by_pattern and test_wait_for_object
wildcard = {
    "sync_version": "sync4",
    "sync_build_version": "revision18_build19285_devtest",
    "sync_collection_mode": CollectionMode.ANY,
    # replacement to get at least 2 results (active and inactive)
    "name": Icons.MAIN_AUDIO_BUTTON_INACTIVE.replace('inac', '*')
}
