import vhat_client

import pytest

from functional_tests.pages.hmi import waitForObject
from functional_tests.pages.interaction import tap_if_visible
from functional_tests.tests.import_helper_functions import data, Data

path = 'test_tap_object'

to_page_object = data(path, name='to_page_object')
from_page_object = data(path, name='from_page_object')
check_object = data(path, name='check_object')


@pytest.fixture(scope='module')
def open_page(attach_to_app):
    """
    opens predefined page
    """
    Data(path).open_page()


@pytest.fixture()
def get_state_audio_button_object(open_page):
    """
    check activity test page: if activity -> return current page object(for ex. Audio Page)
    otherwise switch to other page(for ex. Phone Page) ->
    previous page is already activated (in this case Audio Page) ->
    return previous page object(i.e. Audio Page)
    :param fixture of opening page:
    :return: vhat_client.object
    """
    try:
        return waitForObject(to_page_object)
    except vhat_client.LookupError:
        tap_if_visible(from_page_object)
        return waitForObject(to_page_object)
