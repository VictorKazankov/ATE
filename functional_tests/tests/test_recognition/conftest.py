import pytest
from functional_tests.pages.sync3 import page_supervisor_sync3


@pytest.fixture(scope='module')
def audio_sync3(driver_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.open_audio_page()
    return api.audio_page


@pytest.fixture(scope='module')
def audio_sources_sync3(audio_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.open_audio_sources()
    return api.audio_page


@pytest.fixture(scope='module')
def audio_sirius_sync3(audio_sources_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.tap_siriusxm_button()
    api.audio_page.tap_clsvinyl_direction()
    return api.audio_page


@pytest.fixture(scope='function')
def audio_direct_tune_sync3(audio_sync3):
    api = page_supervisor_sync3.PageSupervisor()
    api.audio_page.open_direct_tune()
    yield api.audio_page
    api.audio_page.tap_cancel_text()
