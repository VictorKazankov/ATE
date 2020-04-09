import vhat_client

import pytest

from functional_tests.pages.hmi import waitForObject
from functional_tests.pages.interaction import tap_if_visible
from functional_tests.utils.sync4.constants import Icons as icons4
from functional_tests.utils.sync3.constants import Icons as icons3


@pytest.fixture()
def get_state_audio_button_object_sync4(driver_sync4):
    """
    check activity audio button: if inactive -> return audio object button
    otherwise switch to phone page -> audio object button is inactive -> return audio object button
    :param driver_sync4:
    :return: object
    """
    try:
        return waitForObject(icons4.MAIN_AUDIO_BUTTON_INACTIVE)
    except vhat_client.LookupError:
        tap_if_visible(icons4.MAIN_PHONE_BUTTON_INACTIVE)
        return waitForObject(icons4.MAIN_AUDIO_BUTTON_INACTIVE)


@pytest.fixture()
def get_state_audio_button_object_sync3(driver_sync3):
    """
    check activity audio button: if inactive -> return audio object button
    otherwise switch to phone page -> audio object button is inactive -> return audio object button
    :param driver_sync3:
    :return: object
    """
    try:
        return waitForObject(icons3.MAIN_AUDIO_BUTTON_INACTIVE)
    except vhat_client.LookupError:
        tap_if_visible(icons3.MAIN_PHONE_BUTTON_INACTIVE)
        return waitForObject(icons3.MAIN_AUDIO_BUTTON_INACTIVE)
